package app.pol.bridge;

import android.graphics.Insets;
import android.net.Uri;
import android.os.Build;
import android.view.View;
import android.view.WindowInsets;
import android.webkit.WebView;
import java.util.Collections;
import java.util.Set;
import java.util.WeakHashMap;

/**
 * MainActivity already applies system-bar and IME insets as root padding on
 * Android 11+. Do not pass the same insets a second time into the Gemini
 * WebView. Keep notifications flowing (zero, do not consume). This is not a
 * focus-forcing keyboard workaround.
 */
public final class GeminiInsets {
    private static final Set<WebView> INSTALLED = Collections.newSetFromMap(new WeakHashMap<WebView, Boolean>());
    private GeminiInsets() {}
    private static boolean isGemini(String url) {
        try { Uri uri = Uri.parse(url); return "https".equalsIgnoreCase(uri.getScheme()) && "gemini.google.com".equalsIgnoreCase(uri.getHost()); }
        catch (Exception ignored) { return false; }
    }
    public static void onPageStarted(WebView web, String url) {
        if (Build.VERSION.SDK_INT < 30 || web == null) return;
        boolean target = isGemini(url);
        boolean installed = INSTALLED.contains(web);
        if (target && !installed) {
            web.setOnApplyWindowInsetsListener(new View.OnApplyWindowInsetsListener() {
                @Override public WindowInsets onApplyWindowInsets(View view, WindowInsets insets) {
                    int handled = WindowInsets.Type.systemBars() | WindowInsets.Type.ime();
                    return new WindowInsets.Builder(insets).setInsets(handled, Insets.NONE).build();
                }
            });
            INSTALLED.add(web);
            web.requestApplyInsets();
        } else if (!target && installed) {
            web.setOnApplyWindowInsetsListener(null);
            INSTALLED.remove(web);
            web.requestApplyInsets();
        }
    }
}
