#ifndef PORT_MANAGER_H
#define PORT_MANAGER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== PORT MANAGER API ====================

/**
 * Khởi tạo danh sách port động.
 * @param capacity  Số port tối đa có thể học (khuyến nghị: 128)
 */
void portMgr_init(int capacity);

/**
 * Giải phóng bộ nhớ port manager.
 */
void portMgr_destroy(void);

/**
 * Nạp whitelist port tĩnh từ mảng.
 * @param ports  Mảng port
 * @param count  Số lượng
 */
void portMgr_loadWhitelist(const int *ports, int count);

/**
 * Tự động học một port mới từ traffic.
 * Nếu port chưa có trong danh sách và còn chỗ → thêm vào.
 * @param port  Giá trị port (1–65535)
 */
void portMgr_learn(int port);

/**
 * Kiểm tra port có được phép không.
 * @param port  Giá trị cần kiểm tra
 * @return true nếu nằm trong whitelist hoặc đã được học.
 */
bool portMgr_isAllowed(int port);

/**
 * Xóa toàn bộ port đã học (giữ whitelist tĩnh).
 */
void portMgr_clearLearned(void);

/**
 * Trả về số port đã học.
 */
int portMgr_learnedCount(void);

#ifdef __cplusplus
}
#endif

#endif // PORT_MANAGER_H
