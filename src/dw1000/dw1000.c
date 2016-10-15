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
