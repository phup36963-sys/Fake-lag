#ifndef PACKET_UTILS_H
#define PACKET_UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== VEC3 (tọa độ 3D) ====================
typedef struct {
    float x, y, z;
} Vec3;

// ==================== FLOAT READ / WRITE (Little Endian) ====================

/**
 * Đọc float 4 byte little-endian từ buffer.
 */
static inline float pkt_readFloatLE(const uint8_t *buf, int off) {
    uint32_t v = (uint32_t)buf[off]
               | ((uint32_t)buf[off+1] <<  8)
               | ((uint32_t)buf[off+2] << 16)
               | ((uint32_t)buf[off+3] << 24);
    float f;
    __builtin_memcpy(&f, &v, 4);
    return f;
}

/**
 * Ghi float 4 byte little-endian vào buffer.
 */
static inline void pkt_writeFloatLE(uint8_t *buf, int off, float val) {
    uint32_t v;
    __builtin_memcpy(&v, &val, 4);
    buf[off]   =  v        & 0xFF;
    buf[off+1] = (v >>  8) & 0xFF;
    buf[off+2] = (v >> 16) & 0xFF;
    buf[off+3] = (v >> 24) & 0xFF;
}

// ==================== POSITION HELPERS ====================

/**
 * Đọc Vec3 từ packet tại offset cho trước.
 */
static inline Vec3 pkt_readVec3(const uint8_t *buf, int off) {
    Vec3 v;
    v.x = pkt_readFloatLE(buf, off);
    v.y = pkt_readFloatLE(buf, off + 4);
    v.z = pkt_readFloatLE(buf, off + 8);
    return v;
}

/**
 * Ghi Vec3 vào packet tại offset.
 */
static inline void pkt_writeVec3(uint8_t *buf, int off, Vec3 v) {
    pkt_writeFloatLE(buf, off,     v.x);
    pkt_writeFloatLE(buf, off + 4, v.y);
    pkt_writeFloatLE(buf, off + 8, v.z);
}

/**
 * Kiểm tra tọa độ có hợp lệ (trong map bounds, không NaN/Inf).
 */
static inline bool pkt_isPosValid(Vec3 p) {
    if (isnan(p.x) || isnan(p.y) || isnan(p.z)) return false;
    if (isinf(p.x) || isinf(p.y) || isinf(p.z)) return false;
    if (p.x < -15000.0f || p.x > 15000.0f) return false;
    if (p.y <  -500.0f  || p.y >  3000.0f) return false;
    if (p.z < -15000.0f || p.z > 15000.0f) return false;
    return true;
}

// ==================== DISTANCE ====================

static inline float pkt_dist2D(float x1, float z1, float x2, float z2) {
    float dx = x1 - x2, dz = z1 - z2;
    return sqrtf(dx*dx + dz*dz);
}

static inline float pkt_dist3DSq(Vec3 a, Vec3 b) {
    float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

static inline float pkt_dist3D(Vec3 a, Vec3 b) {
    return sqrtf(pkt_dist3DSq(a, b));
}

// ==================== IP HEADER HELPERS ====================

/**
 * Kiểm tra gói có phải IPv4/UDP không.
 * @param data  Con trỏ đầu gói raw IP
 * @param len   Tổng độ dài gói
 */
static inline bool pkt_isIPv4UDP(const uint8_t *data, int len) {
    if (len < 28) return false;
    if ((data[0] >> 4) != 4) return false;   // IPv4
    if (data[9] != 17) return false;          // UDP
    return true;
}

/**
 * Trả về độ dài IP header (bytes).
 */
static inline int pkt_ipHdrLen(const uint8_t *data) {
    return (data[0] & 0x0F) * 4;
}

/**
 * Trả về payload UDP và độ dài.
 * @param data       Raw IP packet
 * @param outLen     [out] Độ dài payload
 * @return Con trỏ tới payload UDP (hoặc NULL nếu quá ngắn)
 */
static inline const uint8_t *pkt_udpPayload(const uint8_t *data, int totalLen, int *outLen) {
    int ihl = pkt_ipHdrLen(data);
    if (totalLen < ihl + 8) { *outLen = 0; return NULL; }
    int udpLen = (data[ihl+4]<<8) | data[ihl+5];
    *outLen = udpLen - 8;
    return data + ihl + 8;
}

#ifdef __cplusplus
}
#endif

#endif // PACKET_UTILS_H
