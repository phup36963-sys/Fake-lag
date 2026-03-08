#ifndef GHOST_TELEKILL_H
#define GHOST_TELEKILL_H

#include <jni.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== VERSION ====================
#define GHOST_TELEKILL_VERSION "4.0.0"
#define GHOST_TELEKILL_BUILD_DATE "2026-03-08"

// ==================== LIMITS ====================
#define MAX_SAVED_MOVE      32
#define SOCKET_POOL_SIZE    12
#define PORT_WHITELIST_MAX  64
#define PORT_DYNAMIC_CAP    128
#define DAME_TABLE_SIZE     256
#define MOVE_LOG_INTERVAL   3000   // ms
#define POST_SYNC_WINDOW_MS 2000   // ms

// ==================== SOCKET POOL ====================
typedef struct {
    int   fd;
    bool  inUse;
    long  lastUsedSec;
} SocketEntry;

// ==================== JNI EXPORTS ====================
JNIEXPORT void JNICALL
Java_com_zew_fakelag_GhostNative_init(
    JNIEnv *env, jclass clazz,
    jint szCtrlMax, jint szDameSmallMin, jint szDameSmallMax,
    jint szMoveMin, jint szMoveMax, jint szLargeMin, jint szLargeMax,
    jint leakInterval, jint dameBurst, jlong burstDelayUs, jint restoreCount,
    jbooleanArray dameTbl, jintArray portWL
);

JNIEXPORT void JNICALL
Java_com_zew_fakelag_GhostNative_destroy(JNIEnv *env, jclass clazz);

JNIEXPORT jbyteArray JNICALL
Java_com_zew_fakelag_GhostNative_handleOutbound(
    JNIEnv *env, jclass clazz, jbyteArray packet, jint len
);

JNIEXPORT jbyteArray JNICALL
Java_com_zew_fakelag_GhostNative_handleInbound(
    JNIEnv *env, jclass clazz, jbyteArray packet, jint len
);

JNIEXPORT void JNICALL
Java_com_zew_fakelag_GhostNative_triggerTelekill(JNIEnv *env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_zew_fakelag_GhostNative_sendKeepalive(JNIEnv *env, jclass clazz);

#ifdef __cplusplus
}
#endif

#endif // GHOST_TELEKILL_H
