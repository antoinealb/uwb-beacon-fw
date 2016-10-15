#ifndef DW1000_MOCKS_H_
#define DW1000_MOCKS_H_

#ifdef __cplusplus
extern "C" {
#endif

void mock_write(void *arg, uint8_t *buffer, size_t len);
void mock_read(void *arg, uint8_t *buffer, size_t len);
void mock_cs_acquire(void *arg, bool acquired);

#ifdef __cplusplus
}
#endif

#endif
