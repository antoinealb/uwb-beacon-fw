#ifndef DW1000_H
#define DW1000_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define DW1000_GPIO_COUNT 9
#define DW1000_GPIOx_MODE_GPIO 0
#define DW1000_GPIO0_MODE_RXOKLED 1
#define DW1000_GPIO1_MODE_SFDLED 1
#define DW1000_GPIO2_MODE_RXLED 1
#define DW1000_GPIO3_MODE_TXLED 1
#define DW1000_GPIO4_MODE_EXTPA 1
#define DW1000_GPIO5_MODE_EXTTXE 1
#define DW1000_GPIO6_MODE_EXTRXE 1
#define DW1000_GPIO7_MODE_SYNC 1
#define DW1000_GPIO8_MODE_IRQ 1

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
void dw1000_gpio_configure(dw1000_t *dev, const int modes[DW1000_GPIO_COUNT]);

#ifdef __cplusplus
}
#endif

#endif
