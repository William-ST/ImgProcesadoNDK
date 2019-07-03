//
// Created by William Sulca Talavera on 2019-07-01.
//
#include "com_imgprocesadondk_ImgProcesadoNDK.h"
#include <android/log.h>
#include <android/bitmap.h>
#define LOG_TAG "libimgprocesadondk"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} rgba;
/*Conversion a grises por pixel*/
JNIEXPORT void JNICALL Java_com_imgprocesadondk_ImgProcesadoNDK_convertirGrises
(JNIEnv *env, jobject obj, jobject bitmapcolor, jobject bitmapgris) { AndroidBitmapInfo infocolor;
void *pixelscolor;
AndroidBitmapInfo infogris;
    void *pixelsgris;
    int ret;
    int y;
    int x;
    LOGI("convertirGrises");
    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if ((ret = AndroidBitmap_getInfo(env, bitmapgris, &infogris)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
    infocolor.width, infocolor.height, infocolor.stride, infocolor.format, infocolor.flags);
    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }
    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
        infogris.width, infogris.height, infogris.stride, infogris.format, infogris.flags);
    if (infogris.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env,bitmapgris,&pixelsgris)) <0) {
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }

    // modificacion pixeles en el algoritmo de escala grises
    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *grisline = (rgba *) pixelsgris;
        for (x = 0; x < infocolor.width; x++) {
            float output = (line[x].red + line[x].green + line[x].blue)/3;
            if (output > 255) output = 255;
                grisline[x].red = grisline[x].green = grisline[x].blue =
                (uint8_t) output;
                grisline[x].alpha = line[x].alpha;
        }
        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelsgris = (char *) pixelsgris + infogris.stride;
    }
    LOGI("unlocking pixels");
        AndroidBitmap_unlockPixels(env, bitmapcolor);
        AndroidBitmap_unlockPixels(env, bitmapgris);

}

JNIEXPORT void JNICALL
Java_com_imgprocesadondk_ImgProcesadoNDK_convertirSepia(JNIEnv *env, jobject instance,
                                                      jobject bitmapcolor, jobject bitmapgris) {
    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infogris;
    void *pixelsgris;
    int ret;
    int y;
    int x;
    LOGI("convertirSepia");

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if ((ret = AndroidBitmap_getInfo(env, bitmapgris, &infogris)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
         infocolor.width, infocolor.height,
         infocolor.stride, infocolor.format, infocolor.flags);
    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
         infogris.width, infogris.height, infogris.stride,
         infogris.format, infogris.flags);
    if (infogris.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    if ((ret = AndroidBitmap_lockPixels(env,bitmapgris,&pixelsgris)) <0){
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }
    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *grisline = (rgba *) pixelsgris;
        for (y = 0; y < infocolor.height; y++) {
            rgba *line = (rgba *) pixelscolor;
            rgba *grisline = (rgba *) pixelsgris;
            for (x = 0; x < infocolor.width; x++) {

                grisline[x].red =  (line[x].red * .393) + (line[x].green * .769) + (line[x].blue * .189);
                int valSepia = (line[x].red * .393) + (line[x].green * .769) + (line[x].blue * .189);
                if (valSepia > 255) {
                    grisline[x].red = 255;
                }
                grisline[x].green = (line[x].red * .349) + (line[x].green * .686) + (line[x].blue * .168);
                    valSepia = (line[x].red * .349) + (line[x].green * .686) + (line[x].blue * .168);
                    if (valSepia > 255) {
                        grisline[x].green = 255;
                    }
                        grisline[x].blue = (line[x].red * .272) + (line[x].green * .534) + (line[x].blue * .131);
                        valSepia = (line[x].red * .272) + (line[x].green * .534) + (line[x].blue * .131);
                        if (valSepia > 255) {
                            grisline[x].blue = 255;
                        }
                grisline[x].alpha = line[x].alpha;
            }
            pixelscolor = (char *) pixelscolor + infocolor.stride;
            pixelsgris = (char *) pixelsgris + infogris.stride;
        }
        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelsgris = (char *) pixelsgris + infogris.stride;
    }
    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapgris);
}

JNIEXPORT void JNICALL
Java_com_imgprocesadondk_ImgProcesadoNDK_onAddFrame(JNIEnv *env, jobject instance,
                                                     jobject bitmapcolor, jobject bitmapgris) {
    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infogris;
    void *pixelsgris;
    int ret;
    int y;
    int x;
    LOGI("agregar marco");

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if ((ret = AndroidBitmap_getInfo(env, bitmapgris, &infogris)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
         infocolor.width, infocolor.height,
         infocolor.stride, infocolor.format, infocolor.flags);
    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
         infogris.width, infogris.height, infogris.stride,
         infogris.format, infogris.flags);
    if (infogris.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap no es formato RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    if ((ret = AndroidBitmap_lockPixels(env,bitmapgris,&pixelsgris)) <0){
        LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
    }

    int pixelFrame = 10;
    for (y = 0; y < infocolor.height; y++) {
        rgba *line = (rgba *) pixelscolor;
        rgba *grisline = (rgba *) pixelsgris;
        for (y = 0; y < infocolor.height; y++) {
            rgba *line = (rgba *) pixelscolor;
            rgba *grisline = (rgba *) pixelsgris;
            for (x = 0; x < infocolor.width; x++) {
                grisline[x].red =   line[x].red;
                grisline[x].green = line[x].green ;
                grisline[x].blue = line[x].blue ;
                grisline[x].alpha = line[x].alpha;
                if (y<=pixelFrame ){
                    grisline[x].red =   0;
                    grisline[x].green = 0 ;
                    grisline[x].blue = 0 ;
                    grisline[x].alpha = line[x].alpha;

                }
                if (y >= infocolor.height - pixelFrame ){
                    grisline[x].red =   0;
                    grisline[x].green = 0 ;
                    grisline[x].blue = 0 ;
                    grisline[x].alpha = line[x].alpha;

                }

                if (x<=pixelFrame){
                    grisline[x].red =   0;
                    grisline[x].green = 0 ;
                    grisline[x].blue = 0 ;
                    grisline[x].alpha = line[x].alpha;

                }
                if (x >= infocolor.width - pixelFrame ){
                    grisline[x].red =   0;
                    grisline[x].green = 0 ;
                    grisline[x].blue = 0 ;
                    grisline[x].alpha = line[x].alpha;

                }

            }


            pixelscolor = (char *) pixelscolor + infocolor.stride;
            pixelsgris = (char *) pixelsgris + infogris.stride;
        }
        pixelscolor = (char *) pixelscolor + infocolor.stride;
        pixelsgris = (char *) pixelsgris + infogris.stride;
    }
    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapgris);
}
