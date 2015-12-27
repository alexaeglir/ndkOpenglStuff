/**
 * Created by alex on 12/13/15.
 */
package com.aeglir.testapp1;
import android.app.Activity;
import android.os.Bundle;
import android.view.Surface;
public class App1Activity extends Activity
{
    static {
        System.loadLibrary("App1");
    }
    public static com.aeglir.testapp1.GLView m_View = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        onCreateNative();
        m_View = new com.aeglir.testapp1.GLView(getApplication());
        setContentView(m_View);
    }
    public static native void onCreateNative();
    public static native void SetSurface(Surface surface);
    public static native void SetSurfaceSize(int width, int height);
    public static native void DrawFrame();
};
