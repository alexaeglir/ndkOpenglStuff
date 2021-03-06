//
// Created by alex on 12/13/15.
//

#include "Wrappers_Android.h"
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"App1",__VA_ARGS__))

static const char g_vShaderStr[] =
        "#version 100\n"
        "precision highp float;\n"
        "attribute vec3 vPosition;\n"
        "attribute vec3 vCoords;\n"
        "varying vec2 Coords;\n"
        "void main()\n"
        "{\n"
        "   Coords = vCoords.xy;\n"
        "   gl_Position = vec4(vPosition,1.0);\n"
        "}\n";
static const char g_fShaderStr[] =
        "#version 100\n"
        "precision highp float;\n"
        "varying vec2 Coords;\n"
        "uniform sampler2D Texture0;\n"
        "void main()\n"
        "{\n"
        "   gl_FragColor = texture2D(Texture0,Coords);\n"
        "}\n";
static GLuint g_ProgramObject = 0;

static GLuint LoadShader(GLenum type,const char *shaderSrc)
{
        GLuint shader = glCreateShader(type);
        glShaderSource(shader,1,&shaderSrc,NULL);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
        GLsizei MaxLength = 0;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&MaxLength);
        char *InfoLog = new char[MaxLength];
        glGetShaderInfoLog(shader,MaxLength,&MaxLength,InfoLog);
        LOGI("Shader info log: %s\n",InfoLog);
        return shader;
}
static void GLDebug_LoadStaticProgramObject()
{
        if(g_ProgramObject == 0){
                GLuint vertexShader = LoadShader(GL_VERTEX_SHADER,g_vShaderStr);
                GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER,g_fShaderStr);

                g_ProgramObject = glCreateProgram();
                glAttachShader(g_ProgramObject,vertexShader);
                glAttachShader(g_ProgramObject,fragmentShader);

                glLinkProgram(g_ProgramObject);
                GLint linked;
                glGetProgramiv(g_ProgramObject,GL_LINK_STATUS,&linked);
                GLsizei length = 0;
                GLsizei MaxLength = 0;
                glGetProgramiv(g_ProgramObject,GL_INFO_LOG_LENGTH,&MaxLength);
                char *InfoLog = new char[MaxLength];
                glGetProgramInfoLog(g_ProgramObject,MaxLength,&length,InfoLog);
                LOGI("InfoLog:%s\n",InfoLog);
                int Texture = glGetUniformLocation(g_ProgramObject,"Texture0");
                glUseProgram(g_ProgramObject);
                glUniform1i(Texture,0);
        }
}
static void GLDebug_RenderTriangle()
{
        const GLfloat vVertices[] = { -1.0f,-1.0f,0.0f,
                                      -1.0f,1.0f,0.0f,
                                      1.0f,-1.0f,0.0f,
                                      -1.0f,1.0f,0.0f,
                                      1.0f,-1.0f,0.0f,
                                      1.0f,1.0f,0.0f,
                                    };
        const GLfloat vCoords[] = { 0.0f,0.0f,0.0f,
                                     0.0f,1.0f,0.0f,
                                     1.0f,0.0f,0.0f,
                                     0.0f,1.0f,0.0f,
                                     1.0f,0.0f,0.0f,
                                     1.0f,1.0f,0.0f,
                                   };
        glUseProgram(g_ProgramObject);
        GLint Loc1 = glGetAttribLocation(g_ProgramObject,"vPosition");
        GLint Loc2 = glGetAttribLocation(g_ProgramObject,"vCoords");
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glVertexAttribPointer(Loc1,3,GL_FLOAT,GL_FALSE,0,vVertices);
        glVertexAttribPointer(Loc2,3,GL_FLOAT,GL_FALSE,0,vCoords);
        glEnableVertexAttribArray(Loc1);
        glEnableVertexAttribArray(Loc2);

        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES,0,0);
        glUseProgram(0);
        glDisableVertexAttribArray(Loc1);
        glDisableVertexAttribArray(Loc2);

}
int g_Width = 0;
int g_Height = 0;
GLuint g_Texture = 0;

extern unsigned char *g_FrameBuffer;

extern "C" {
    JNIEXPORT void JNICALL Java_com_aeglir_testapp1_App1Activity_onCreateNative(JNIEnv *env, jobject obj )
    {
      OnStart();
    }
        JNIEXPORT void JNICALL Java_com_aeglir_testapp1_App1Activity_SetSurface(JNIEnv *env, jclass clazz,jobject javaSurface )
        {
        }
        JNIEXPORT void JNICALL Java_com_aeglir_testapp1_App1Activity_SetSurfaceSize(JNIEnv *env, jclass clazz,int Width,int Height)
        {
                LOGI("Surface size %i x %i",Width,Height);
                g_Width = Width;
                g_Height = Height;
                GLDebug_LoadStaticProgramObject();

                glGenTextures(1,&g_Texture);
                glBindTexture(GL_TEXTURE_2D,g_Texture);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ImageWidth,ImageHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,g_FrameBuffer);
        }
        JNIEXPORT void JNICALL Java_com_aeglir_testapp1_App1Activity_DrawFrame(JNIEnv *env, jobject obj )
        {
LOGI("nDrawFrame");
        OnDrawFrame();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,g_Texture);
        glTexSubImage2D(GL_TEXTURE_2D,0,0,0,ImageWidth,ImageHeight,GL_RGBA,GL_UNSIGNED_BYTE,g_FrameBuffer);
        GLDebug_RenderTriangle();
        }
}