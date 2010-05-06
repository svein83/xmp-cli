/* Simple interface adaptor for jni */

#include <stdlib.h>
#include <jni.h>
#include "xmp.h"

/* #include <android/log.h> */


extern struct xmp_drv_info drv_smix;

static xmp_context ctx;
static struct xmp_options *opt;
static struct xmp_module_info mi;

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_init(JNIEnv *env, jobject obj)
{
	xmp_drv_register(&drv_smix);
	ctx = xmp_create_context();
	xmp_init(ctx, 0, NULL);
	opt = xmp_get_options(ctx);
	opt->verbosity = 0;

	opt->freq = 44100;
	opt->resol = 16;
	opt->outfmt &= ~XMP_FMT_MONO;
	opt->flags |= XMP_CTL_ITPT | XMP_CTL_FILTER;

	if (xmp_open_audio(ctx) < 0) {
		xmp_deinit(ctx);
		xmp_free_context(ctx);
		return -1;
	}

	return 0;
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_deinit(JNIEnv *env, jobject obj)
{
	xmp_close_audio(ctx);
	xmp_deinit(ctx);
	xmp_free_context(ctx);

	return 0;
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_loadModule(JNIEnv *env, jobject obj, jstring name)
{
	const char *filename;
	int res;

	filename = (*env)->GetStringUTFChars(env, name, NULL);
	/*__android_log_print(ANDROID_LOG_DEBUG, "libxmp", "%s", filename);*/
	res = xmp_load_module(ctx, (char *)filename);
	(*env)->ReleaseStringUTFChars(env, name, filename);

	if (res < 0)
		return -1;
	xmp_get_module_info(ctx, &mi);

	return 0;
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_releaseModule(JNIEnv *env, jobject obj)
{
	xmp_release_module(ctx);
	return 0;
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_startPlayer(JNIEnv *env, jobject obj)
{
	return xmp_player_start(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_endPlayer(JNIEnv *env, jobject obj)
{
	xmp_player_end(ctx);
	return 0;
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_playFrame(JNIEnv *env, jobject obj)
{
	return xmp_player_frame(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_softmixer(JNIEnv *env, jobject obj)
{
	return xmp_smix_softmixer(ctx);
}

JNIEXPORT jshortArray JNICALL
Java_org_helllabs_android_xmp_Xmp_getBuffer(JNIEnv *env, jobject obj, jint size)
{
	jshortArray a;
	short *b;

	size /= 2;
	if ((a = (*env)->NewShortArray(env, size)) == NULL)
		return NULL;

	b = (short *)xmp_smix_buffer(ctx);
	(*env)->SetShortArrayRegion(env, a, 0, size, b);

	return a;
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_nextOrd(JNIEnv *env, jobject obj)
{
	return xmp_ord_next(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_prevOrd(JNIEnv *env, jobject obj)
{
	return xmp_ord_prev(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_setOrd(JNIEnv *env, jobject obj, jint n)
{
	return xmp_ord_set(ctx, n);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_stopModule(JNIEnv *env, jobject obj)
{
	return xmp_mod_stop(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_restartModule(JNIEnv *env, jobject obj)
{
	return xmp_mod_restart(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_stopTimer(JNIEnv *env, jobject obj)
{
	return xmp_timer_stop(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_restartTimer(JNIEnv *env, jobject obj)
{
	return xmp_timer_restart(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_incGvol(JNIEnv *env, jobject obj)
{
	return xmp_gvol_inc(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_decGvol(JNIEnv *env, jobject obj)
{
	return xmp_gvol_dec(ctx);
}

JNIEXPORT jint JNICALL
Java_org_helllabs_android_xmp_Xmp_seek(JNIEnv *env, jobject obj, jlong time)
{
#if 0
        int i, t;
        struct xmp_player_context *p = &((struct xmp_context *)ctx)->p;

        _D("seek to %ld, total %d", time, xmp_cfg.time);

        for (i = 0; i < xmp_cfg.mod_info.len; i++) {
                t = p->m.xxo_info[i].time;

                _D("%2d: %ld %d", i, time, t);

                if (t > time) {
                        int a;
                        if (i > 0)
                                i--;
                        a = xmp_ord_set(ctx, i);
                        ipb->output->flush(p->m.xxo_info[i].time);
                        break;
                }
        }
#endif

	return 0;
}
