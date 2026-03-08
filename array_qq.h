#ifndef ARRAY_QQ_H
#define ARRAY_QQ_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== ARRAYQQ – RING BUFFER CHO MOVEMENT PACKETS ====================
// Lưu tối đa MAX_SAVED_MOVE packet movement thật.
// Khi đầy → ghi đè packet cũ nhất (ring).

#ifndef MAX_SAVED_MOVE
#  define MAX_SAVED_MOVE 32
#endif

typedef struct {
    uint8_t  *data;
    size_t    len;
} MovePkt;

typedef struct {
    MovePkt  slots[MAX_SAVED_MOVE];
    int      head;      // Vị trí ghi tiếp theo
    int      count;     // Số packet đang lưu (0–MAX_SAVED_MOVE)
} ArrayQQ;

// ==================== API ====================

/**
 * Khởi tạo ArrayQQ (zero-init).
 */
void aqq_init(ArrayQQ *q);

/**
 * Giải phóng toàn bộ bộ nhớ trong queue.
 */
void aqq_destroy(ArrayQQ *q);

/**
 * Xóa hết dữ liệu nhưng giữ struct.
 */
void aqq_clear(ArrayQQ *q);

/**
 * Thêm một packet vào ring buffer (copy dữ liệu).
 * @return true nếu thành công.
 */
bool aqq_push(ArrayQQ *q, const uint8_t *data, size_t len);

/**
 * Trả về số packet đang có trong queue.
 */
static inline int aqq_count(const ArrayQQ *q) { return q->count; }

/**
 * Trả về con trỏ đọc packet thứ i (0 = cũ nhất).
 * Không cấp phát thêm bộ nhớ.
 */
const MovePkt *aqq_get(const ArrayQQ *q, int i);

#ifdef __cplusplus
}
#endif

#endif // ARRAY_QQ_H
