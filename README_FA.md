<div align="center">

# 🌉 پل کلودفلر · Pol Cloudflare Bridge
### کلاینت اندروید و پل Cloudflare برای سرویس‌های هوش مصنوعی
**مرورگر داخلی · مدل آفلاین متنی · رابط سه‌بعدی**

<br/>

<a href="https://github.com/imgutss/pol-cloudflare-bridge/releases/download/v1.0.1/Pol-Cloudflare-Bridge.apk">
  <img src="https://img.shields.io/badge/دانلود%20مستقیم%20APK-نسخه%20v1.0.1-FF8A2A?style=for-the-badge&logo=android&logoColor=white" alt="دانلود APK" />
</a>
<a href="./README.md"><img src="https://img.shields.io/badge/English-Docs-2563EB?style=for-the-badge" alt="English Documentation" /></a>
<a href="https://github.com/imgutss/pol-cloudflare-bridge/stargazers"><img src="https://img.shields.io/badge/⭐%20Star-Support%20Project-F59E0B?style=for-the-badge" alt="Stars" /></a>

<br/><br/>
<img src="https://img.shields.io/badge/Android-8%2B-3DDC84?style=flat-square&logo=android&logoColor=white" />
<img src="https://img.shields.io/badge/Cloudflare-Workers-F38020?style=flat-square&logo=cloudflare&logoColor=white" />
<img src="https://img.shields.io/badge/Offline-ARM64%20CPU-8B5CF6?style=flat-square" />

</div>

---

<div dir="rtl">

## 🌟 معرفی

پل کلودفلر یک اپ اندرویدی با مرورگر داخلی، اتصال به سرویس‌های هوش مصنوعی و امکان گفت‌وگوی متنی با مدل‌های آفلاین است. مسیر مرورگر داخلی از طریق پروکسی محلی و ورکر کلودفلر برقرار می‌شود؛ آدرس ورکر و کلید پل قابل تنظیم هستند.

| قابلیت | توضیح |
| :--- | :--- |
| 🌐 **مرورگر داخلی** | باز کردن سرویس‌هایی مانند Gemini و Muse از داخل برنامه. |
| 🔧 **تنظیم دستی API** | ورود دستی آدرس، نام مدل و کلید API؛ انتخاب خودکار مدل انجام نمی‌شود. |
| 🤖 **مدل آفلاین متنی** | دریافت و اجرای مدل سازگار روی CPU دستگاه؛ مدل‌ها جداگانه دانلود می‌شوند. |
| 💬 **پاسخ تدریجی** | نمایش مرحله‌ای پاسخ آفلاین، نگه‌داشتن مدل بارگذاری‌شده بین پیام‌ها و نمایش Markdown. |
| 🗂️ **مدیریت مدل‌ها** | کارت‌های افقی و گزینهٔ حذف مدل آفلاین. |
| ⌨️ **کیبورد Gemini** | اصلاح مدیریت فضای کیبورد در مرورگر داخلی. |

## 🚀 دانلود و نصب

| نسخه | حجم APK | پیش‌نیاز | دانلود |
| :---: | :---: | :---: | :---: |
| **v1.0.1** | حدود ۱۶۸ مگابایت | اندروید ۸ یا بالاتر؛ مدل آفلاین به ARM64 نیاز دارد | [📥 دانلود مستقیم APK](https://github.com/imgutss/pol-cloudflare-bridge/releases/download/v1.0.1/Pol-Cloudflare-Bridge.apk) |

مدل‌های آفلاین داخل APK نیستند و به فضای ذخیره‌سازی و حافظهٔ کافی نیاز دارند. قبل از نصب، از داده‌های مهم پشتیبان بگیرید. اگر نسخهٔ قبلی با امضای متفاوت نصب شده باشد، اندروید ممکن است به‌روزرسانی مستقیم را نپذیرد؛ حذف برنامه می‌تواند داده‌های آن را پاک کند.

## ☁️ تنظیم اتصال

ورکر را با راهنمای پوشهٔ [`worker/`](./worker/) روی حساب Cloudflare خود مستقر کنید و آدرس و کلید آن را در تنظیمات برنامه وارد کنید. برای سرویس سفارشی، نشانی API، نام مدل و کلید همان سرویس را به‌صورت دستی وارد کنید.

> کشور یا IP خروجی Cloudflare ثابت و تضمین‌شده نیست و دسترسی به هر سرویس به شرایط آن سرویس و شبکه بستگی دارد. همهٔ مسیرهای برنامه یا تمام ارائه‌دهندگان برای عبور از تونل به‌صورت جامع تأیید نشده‌اند؛ برای adapter سفارشی، در نبود پل اتصال مستقیم جایگزین نمی‌شود.

## ℹ️ محدودیت‌های نسخه

- در این انتشار پردازش تصویر و دوربین آفلاین و شتاب‌دهی GPU وجود ندارد؛ گفت‌وگوی آفلاین متنی از CPU استفاده می‌کند.
- سازگاری، سرعت و مصرف حافظه به مدل و دستگاه بستگی دارد. کیبورد Gemini توسط کاربر روی دستگاه آزمایش شده، اما همهٔ دستگاه‌ها و نسخه‌های WebView آزمایش نشده‌اند.
- محتوای مخزن شامل کد و منابع نسخه‌های قبلی و قطعات تغییرات این نسخه است؛ **سورس کامل و فرایند بازتولید مستقل APK منتشرشده فعلاً در مخزن موجود نیست**.

📋 [تاریخچه تغییرات](./CHANGELOG.md) · 📦 [همهٔ انتشارها](https://github.com/imgutss/pol-cloudflare-bridge/releases) · 🔒 [امنیت](./SECURITY.md)

</div>

---
<div align="center"><b>توسعه‌داده‌شده توسط <a href="https://github.com/imgutss">Guts (imgutss)</a></b></div>
