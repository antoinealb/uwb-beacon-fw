#include "dw1000.h"

#define HDR_RD (0 << 7)
#define HDR_WR (1 << 7)

#define HDR_SUB_INDEX (1 << 6)

void dw1000_init(dw1000_t *dev, void *arg,
                void (*acquire_cs)(void *, bool),
                void (*write)(void *, uint8_t *, size_t),
                void (*read)(void *, uint8_t *, size_t))
{
    dev->arg = arg;
    dev->acquire_cs = acquire_cs;
    dev->write = write;
    dev->read = read;
}

uint32_t dw1000_id_read(dw1000_t *dev)
{
    uint8_t command[] = {HDR_RD | 0x00};
    uint8_t answer[4];
    uint32_t res = 0;

    dev->acquire_cs(dev->arg, true);

    dev->write(dev->arg, command, sizeof(command));
    dev->read(dev->arg, answer, sizeof(answer));

    dev->acquire_cs(dev->arg, false);

    /* Convert ID from big endian. */
    res |= answer[0] << 0;
    res |= answer[1] << 8;
    res |= answer[2] << 16;
    res |= answer[3] << 24;

    return res;
}

void dw1000_gpio_configure(dw1000_t *dev, const int modes[DW1000_GPIO_COUNT])
{
    uint8_t command[] = {
        HDR_WR | 0x26,
        0x00,
        (modes[5] << 0) | (modes[6] << 2) | (modes[7] << 4) | (modes[8] << 6),
        (modes[1] << 0) | (modes[2] << 2) | (modes[3] << 4) | (modes[4] << 6),
        modes[0] << 6
    };

    dev->acquire_cs(dev->arg, true);
    dev->write(dev->arg, command, sizeof(command));
    dev->acquire_cs(dev->arg, false);
}
