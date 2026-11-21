// Experimental replacement for Pol's local inference JNI. CPU only.
// Requires llama.cpp revision 919794500475862f6768c353255469dc661cb734.
#include <jni.h>
#include <llama.h>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

namespace {
std::mutex engine_mutex, output_mutex;
std::atomic<bool> cancelled{false};
std::string output, cached_path;
llama_model *cached_model = nullptr;
std::once_flag backend_once;
void clear_model() { if (cached_model) llama_model_free(cached_model); cached_model=nullptr; cached_path.clear(); }
std::string jstr(JNIEnv *e, jstring v) {
 if (!v) throw std::runtime_error("Missing string");
 const char *p=e->GetStringUTFChars(v,nullptr); if (!p) throw std::runtime_error("Unable to read string");
 std::string r(p); e->ReleaseStringUTFChars(v,p); return r;
}
std::string bytes(JNIEnv *e,jbyteArray a) {
 if (!a) throw std::runtime_error("Missing message");
 jsize n=e->GetArrayLength(a); std::string v(n,'\0');
 if(n) e->GetByteArrayRegion(a,0,n,reinterpret_cast<jbyte*>(&v[0]));
 if(e->ExceptionCheck()) throw std::runtime_error("Unable to read message");
 if(v.find('\0')!=std::string::npos) throw std::runtime_error("NUL in message"); return v;
}
jbyteArray to_bytes(JNIEnv *e,const std::string &s) {
 jbyteArray a=e->NewByteArray((jsize)s.size());
 if(a && !s.empty()) e->SetByteArrayRegion(a,0,(jsize)s.size(),reinterpret_cast<const jbyte*>(s.data())); return a;
}
// Strip an incomplete trailing UTF-8 character when a token splits one.
size_t utf8_prefix(const std::string &s) {
 if(s.empty()) return 0;
 size_t i=s.size()-1; while(i>0 && (static_cast<unsigned char>(s[i])&0xc0)==0x80) --i;
 auto c=static_cast<unsigned char>(s[i]); size_t n=c<0x80?1:(c&0xe0)==0xc0?2:(c&0xf0)==0xe0?3:(c&0xf8)==0xf0?4:1;
 return s.size()-i<n?i:s.size();
}
bool abort_decode(void *) { return cancelled.load(); }
void fail(JNIEnv *e,const char *message) { if(!e->ExceptionCheck()) { auto c=e->FindClass("java/lang/IllegalStateException"); if(c) e->ThrowNew(c,message); } }
}
extern "C" JNIEXPORT void JNICALL Java_app_pol_local_PolNative_cancel(JNIEnv *,jclass) {cancelled=true;}
extern "C" JNIEXPORT jbyteArray JNICALL Java_app_pol_local_PolNative_peek(JNIEnv *e,jclass) {
 std::lock_guard<std::mutex> guard(output_mutex); return to_bytes(e,output.substr(0,utf8_prefix(output)));
}
extern "C" JNIEXPORT void JNICALL Java_app_pol_local_PolNative_unload(JNIEnv *,jclass) {
 std::lock_guard<std::mutex> guard(engine_mutex); clear_model();
}
extern "C" JNIEXPORT jbyteArray JNICALL Java_app_pol_local_PolNative_generate(JNIEnv *e,jclass,jstring path,jobjectArray roles,jobjectArray texts,jint threads) {
 std::lock_guard<std::mutex> guard(engine_mutex);
 try {
  // The Java caller must reset the cancel flag before starting the worker thread.
  {std::lock_guard<std::mutex> out(output_mutex); output.clear();}
  std::string file=jstr(e,path);
  if(!roles || !texts) throw std::runtime_error("Missing chat history");
  int n=e->GetArrayLength(roles); if(n<1 || n>32 || n!=e->GetArrayLength(texts)) throw std::runtime_error("Invalid chat history");
  std::vector<std::string> rs,cs; rs.reserve(n); cs.reserve(n);
  for(int i=0;i<n;++i){auto r=(jstring)e->GetObjectArrayElement(roles,i); auto t=(jbyteArray)e->GetObjectArrayElement(texts,i);rs.push_back(jstr(e,r));cs.push_back(bytes(e,t));e->DeleteLocalRef(r);e->DeleteLocalRef(t);}
  std::call_once(backend_once,[]{llama_backend_init();});
  if(!cached_model || cached_path!=file) {
   clear_model(); auto mp=llama_model_default_params(); mp.n_gpu_layers=0;
   cached_model=llama_model_load_from_file(file.c_str(),mp);
   if(!cached_model) throw std::runtime_error("Could not load local model"); cached_path=file;
  }
  if(cancelled) return to_bytes(e,"");
  std::vector<llama_chat_message> msgs; for(int i=0;i<n;++i) msgs.push_back({rs[i].c_str(),cs[i].c_str()});
  const char *tmpl=llama_model_chat_template(cached_model,nullptr); if(!tmpl) throw std::runtime_error("Model has no chat template");
  int len=llama_chat_apply_template(tmpl,msgs.data(),msgs.size(),true,nullptr,0);
  if(len<0 || len>1000000) throw std::runtime_error("Unsupported chat template");
  std::vector<char> buf(len+1); int written=llama_chat_apply_template(tmpl,msgs.data(),msgs.size(),true,buf.data(),buf.size());
  if(written<0 || written>len) throw std::runtime_error("Could not format chat");
  std::string prompt(buf.data(),written);
  char arch[128]={}; llama_model_meta_val_str(cached_model,"general.architecture",arch,sizeof(arch));
  if(std::string(arch).find("qwen3")!=std::string::npos && prompt.find("<think>")==std::string::npos) prompt+="<think>\n\n</think>\n\n";
  auto vocab=llama_model_get_vocab(cached_model);
  int count=llama_tokenize(vocab,prompt.data(),(int)prompt.size(),nullptr,0,true,true); if(count>=0 || count==INT32_MIN) throw std::runtime_error("Could not tokenize");
  count=-count; if(count>1792) throw std::runtime_error("Chat is too long for the local model context");
  std::vector<llama_token> tokens(count); if(llama_tokenize(vocab,prompt.data(),prompt.size(),tokens.data(),count,true,true)!=count) throw std::runtime_error("Tokenization failed");
  auto cp=llama_context_default_params(); cp.n_ctx=2048; cp.n_batch=512; cp.n_ubatch=512; cp.n_threads=std::max(1,(int)threads); cp.n_threads_batch=cp.n_threads; cp.abort_callback=abort_decode;
  auto ctx=llama_init_from_model(cached_model,cp); if(!ctx) throw std::runtime_error("Could not allocate model context");
  struct ContextGuard {llama_context *p; ~ContextGuard(){llama_free(p);}} cg{ctx};
  for(int off=0;off<count && !cancelled;off+=512){int l=std::min(512,count-off);if(llama_decode(ctx,llama_batch_get_one(tokens.data()+off,l))!=0 && !cancelled) throw std::runtime_error("Prompt evaluation failed");}
  auto sampler=llama_sampler_chain_init(llama_sampler_chain_default_params()); if(!sampler) throw std::runtime_error("Could not create sampler");
  struct SamplerGuard {llama_sampler *p; ~SamplerGuard(){llama_sampler_free(p);}} sg{sampler};
  llama_sampler_chain_add(sampler,llama_sampler_init_top_k(40)); llama_sampler_chain_add(sampler,llama_sampler_init_top_p(0.95f,1)); llama_sampler_chain_add(sampler,llama_sampler_init_temp(0.7f)); llama_sampler_chain_add(sampler,llama_sampler_init_dist(LLAMA_DEFAULT_SEED));
  for(int i=0;i<256 && !cancelled;++i) {
   auto token=llama_sampler_sample(sampler,ctx,-1); if(llama_vocab_is_eog(vocab,token)) break;
   char small[256]; int size=llama_token_to_piece(vocab,token,small,sizeof(small),0,false);
   std::string piece; if(size>=0) piece.assign(small,size); else {std::vector<char> large(-size); int z=llama_token_to_piece(vocab,token,large.data(),large.size(),0,false);if(z<0) throw std::runtime_error("Token conversion failed");piece.assign(large.data(),z);}
   {std::lock_guard<std::mutex> out(output_mutex); output+=piece;}
   if(llama_decode(ctx,llama_batch_get_one(&token,1))!=0 && !cancelled) throw std::runtime_error("Generation failed");
  }
  std::lock_guard<std::mutex> out(output_mutex); return to_bytes(e,output.substr(0,utf8_prefix(output)));
 } catch(const std::exception &ex){fail(e,ex.what());return nullptr;} catch(...){fail(e,"Unknown local inference error");return nullptr;}
}
extern "C" JNIEXPORT void JNICALL Java_app_pol_local_PolNative_prepare(JNIEnv *,jclass) {cancelled=false; std::lock_guard<std::mutex> out(output_mutex);output.clear();}
