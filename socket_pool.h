#ifndef SOCKET_POOL_H
#define SOCKET_POOL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== SOCKET POOL API ====================

/**
 * Khởi tạo pool socket UDP dùng cho burst telekill.
 * @param size  Số lượng socket trong pool (khuyến nghị: 12)
 */
void socketPool_init(int size);

/**
 * Giải phóng toàn bộ pool.
 */
void socketPool_destroy(void);

/**
 * Mượn một socket rảnh từ pool.
 * @return fd >= 0 nếu thành công, -1 nếu hết socket.
 */
int  socketPool_borrow(void);

/**
 * Trả socket về pool sau khi dùng xong.
 * @param fd  File descriptor đã mượn.
 */
void socketPool_return(int fd);

/**
 * Gửi UDP không block.
 * @param fd       Socket fd
 * @param dstIP    Địa chỉ IP đích (host byte order)
 * @param dstPort  Cổng đích
 * @param data     Dữ liệu cần gửi
 * @param len      Độ dài dữ liệu
 * @return true nếu gửi đủ byte.
 */
bool socketPool_sendUdp(int fd, uint32_t dstIP, int dstPort,
                        const uint8_t *data, size_t len);

/**
 * Burst gửi nhiều lần cùng một packet.
 * @param dstIP    IP đích (host byte order)
 * @param dstPort  Cổng đích
 * @param data     Dữ liệu
 * @param len      Độ dài
 * @param count    Số lần gửi
 * @param delayUs  Delay giữa các lần (micro-giây, 0 = không delay)
 */
void socketPool_burst(uint32_t dstIP, int dstPort,
                      const uint8_t *data, size_t len,
                      int count, long delayUs);

#ifdef __cplusplus
}
#endif

#endif // SOCKET_POOL_H
