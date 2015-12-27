/**
 * Creat ed by alex on 12/22/15.
 */
package com.aeglir.testapp1;
import android.content.Context;
//import android.opengl.EGLExt;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.graphics.PixelFormat;

//import com.aeglir.testapp1.App1Activity;

import javax.microedition.khronos.egl.EGL10;
import  javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;
public class GLView extends GLSurfaceView
{
    private static final String TAG ="GLView";

    public GLView(Context context)
    {
        super(context);
        init(16, 0);
    }
    private void init(int depth,int stencil)
    {
        this.getHolder().setFormat(PixelFormat.RGB_888);
        setEGLContextFactory(new ContextFactory());
        setEGLConfigChooser(new ConfigChooser(8, 8, 8, 0, depth, stencil));
        setRenderer(new Renderer());
    }
    private static class ContextFactory implements GLSurfaceView.EGLContextFactory
    {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl,EGLDisplay display,EGLConfig eglConfig)
        {
            Log.w(TAG, "Creating OpenGl ES 2.0 context");
            checkEglError("Before eglCreateContext",egl);
            int[] attrib_list ={EGL_CONTEXT_CLIENT_VERSION,2,EGL10.EGL_NONE};
            EGLContext context = egl.eglCreateContext(display,eglConfig,EGL10.EGL_NO_CONTEXT,attrib_list);
            checkEglError("After eglCreateContext",egl);
            return context;
        }
        public void destroyContext(EGL10 egl,EGLDisplay display,EGLContext context)
        {
            egl.eglDestroyContext(display,context);
        }
    }
    private static void checkEglError(String prompt,EGL10 egl)
    {
        int error;

        while((error = egl.eglGetError()) != EGL10.EGL_SUCCESS){
            Log.e(TAG,String.format("%s: EGL error 0x%x",prompt,error));
        }
    }
    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];

        public ConfigChooser(int r,int g,int b,int a,int depth,int stencil)
        {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }
        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 ={
                EGL10.EGL_RED_SIZE,5,
                EGL10.EGL_GREEN_SIZE,6,
                EGL10.EGL_BLUE_SIZE,5,
                EGL10.EGL_ALPHA_SIZE,0,
                EGL10.EGL_DEPTH_SIZE,16,
                EGL10.EGL_STENCIL_SIZE,0,
                EGL10.EGL_SAMPLE_BUFFERS,0,
                EGL10.EGL_SAMPLES,0,
                EGL10.EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
                EGL10.EGL_NONE,EGL10.EGL_NONE
        };
        public EGLConfig chooseConfig(EGL10 egl,EGLDisplay display)
        {
            int[] numConfig = new int[1];
            egl.eglChooseConfig(display,s_configAttribs2,null,0,numConfig);
            int numConfigs = numConfig[0];
            if(numConfigs <= 0){
                throw new IllegalArgumentException("No Config match configSpec");
            }
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display,s_configAttribs2,configs,numConfigs,numConfig);
            return chooseBestConfig(egl,display,configs);//return the best configs
        }
        public EGLConfig chooseBestConfig(EGL10 egl,EGLDisplay display,EGLConfig[] configs)
        {
            for(EGLConfig config:configs){
                int d = findConfigAttr(egl,display,config,EGL10.EGL_DEPTH_SIZE,0);
                int s = findConfigAttr(egl,display,config,EGL10.EGL_STENCIL_SIZE,0);
                if(d < mDepthSize || s < mStencilSize)
                    continue;//we need at least our required depth and stencil
                int r = findConfigAttr(egl,display,config,EGL10.EGL_RED_SIZE,0);
                int g = findConfigAttr(egl,display,config,EGL10.EGL_GREEN_SIZE,0);
                int b = findConfigAttr(egl,display,config,EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttr(egl,display,config,EGL10.EGL_ALPHA_SIZE,0);

                if(r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }
        private int findConfigAttr(EGL10 egl,EGLDisplay display,EGLConfig config,int attribute,int deff)
        {
            if(egl.eglGetConfigAttrib(display,config,attribute,mValue)){
                return mValue[0];
            }
            return deff;
        }
    }
    private static class Renderer implements GLSurfaceView.Renderer
    {
        public void onDrawFrame(GL10 gl)
        {
            com.aeglir.testapp1.App1Activity.DrawFrame();
        }
        public void onSurfaceChanged(GL10 gl,int width, int height)
        {
            com.aeglir.testapp1.App1Activity.SetSurfaceSize(width,height);
        }
        public void onSurfaceCreated(GL10 gl,EGLConfig config)
        {
            com.aeglir.testapp1.App1Activity.SetSurface(com.aeglir.testapp1.App1Activity.m_View.getHolder().getSurface());
        }


    }

}
