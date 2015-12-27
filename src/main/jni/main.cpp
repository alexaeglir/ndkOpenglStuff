//
// Created by alex on 12/22/15.
//
#include <stdlib.h>

#ifdef ANDROID
#include "Wrappers_Android.h"
#endif
unsigned  char *g_FrameBuffer;

void OnStart()
{
    g_FrameBuffer = (unsigned char *)malloc(ImageWidth*ImageHeight*4);
}
void OnDrawFrame()
{
    int x,y;

    for(y= 0; y < ImageHeight;y++){
        for(x = 0; x < ImageWidth; x++){
            int c = (x ^ y)&0xff;
            ((unsigned char*)g_FrameBuffer)[y*ImageWidth+x] = (c <<16) | (c<<8) | (c<<0) | 0xff000000;
        }
    }
}

