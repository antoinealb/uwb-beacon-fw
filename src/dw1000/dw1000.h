#ifndef DW1000_H
#define DW1000_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
    void *arg;
    void (*acquire_cs)(void *arg, bool acquire);
    void (*write)(void *arg, uint8_t *buffer, size_t len);
    void (*read)(void *arg, uint8_t *buffer, size_t len);
} dw1000_t;

void dw1000_init(dw1000_t *dev, void *arg,
                void (*acquire_cs)(void *, bool),
                void (*write)(void *, uint8_t *, size_t),
                void (*read)(void *, uint8_t *, size_t));

uint32_t dw1000_id_read(dw1000_t *dev);

#ifdef __cplusplus
}
#endif

#endif
