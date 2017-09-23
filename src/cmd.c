#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <hal.h>
#include <chprintf.h>
#include <shell.h>
#include "mpu9250.h"

#define TEST_WA_SIZE        THD_WORKING_AREA_SIZE(256)
#define SHELL_WA_SIZE       THD_WORKING_AREA_SIZE(2048)

static void cmd_reboot(BaseSequentialStream *chp, int argc, char **argv)
{
    (void) chp;
    (void) argc;
    (void) argv;
    NVIC_SystemReset();
}

static void cmd_mpu_ping(BaseSequentialStream *chp, int argc, char **argv)
{
    (void) argc;
    (void) argv;

    /*
     * SPI2 configuration structure for MPU9250.
     * SPI2 is on APB2 @ 84MHz / 128 = 656.25kHz
     * CPHA=1, CPOL=1, 8bits frames, MSB transmitted first.
     */
    static SPIConfig spi_cfg = {
        .end_cb = NULL,
        .ssport = GPIOB,
        .sspad = GPIOB_IMU_CS_N,
        .cr1 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };
    spiStart(&SPID2, &spi_cfg);

    static mpu9250_t mpu6000;
    mpu9250_init(&mpu6000, &SPID2);

    if (mpu9250_ping(&mpu6000)) {
        chprintf(chp, "MPU is ready\r\n");
    } else {
        chprintf(chp, "MPU is not responding correctly\r\n");
    }
}

static void cmd_dw1000_ping(BaseSequentialStream *chp, int argc, char **argv)
{
    (void) argc;
    (void) argv;

    /*
     * SPI1 configuration structure for DW1000.
     * SPI1 is on APB2 @ 84MHz / 128 = 656.25kHz
     * CPHA=0, CPOL=0, 8bits frames, MSB transmitted first.
     */
    static SPIConfig spi_cfg = {
        .end_cb = NULL,
        .ssport = GPIOA,
        .sspad = GPIOA_UWB_CS_N,
        .cr1 = SPI_CR1_BR_2 | SPI_CR1_BR_0
    };
    spiStart(&SPID1, &spi_cfg);

    uint8_t txbuf[5] = {0x01};
    uint8_t rxbuf[5];

    chprintf(chp, "current state of the rst pin: %d\r\n", palReadPad(GPIOA, GPIOA_UWB_RST_N));

    //palClearPad(GPIOA, GPIOA_UWB_RST_N);
    //chThdSleepMilliseconds(100);
    //palSetPad(GPIOA, GPIOA_UWB_RST_N);
    //chThdSleepMilliseconds(100);

    spiSelect(&SPID1);
    spiExchange(&SPID1, 5, txbuf, rxbuf);
    spiUnselect(&SPID1);

    chprintf(chp, "Got the following answer:");
    int i;
    for (i = 0; i < 5; i++) {
        chprintf(chp, "0x%x ", rxbuf[i]);
    }
    chprintf(chp, "\n");
}


static ShellConfig shell_cfg;
const ShellCommand shell_commands[] = {
    {"reboot", cmd_reboot},
    {"mpu_ping", cmd_mpu_ping},
    {"dw", cmd_dw1000_ping},
    {NULL, NULL}
};


static THD_FUNCTION(shell_spawn_thd, p)
{
    BaseSequentialStream *io = (BaseSequentialStream *)p;
    thread_t *shelltp = NULL;

    shell_cfg.sc_channel = io;
    shell_cfg.sc_commands = shell_commands;

    shellInit();

    while (TRUE) {
        if (!shelltp) {
            shelltp = shellCreate(&shell_cfg, SHELL_WA_SIZE, NORMALPRIO);
        } else {
            if (chThdTerminatedX(shelltp)) {
                chThdRelease(shelltp);
                shelltp = NULL;
            }
        }
        chThdSleepMilliseconds(500);
    }
}

void shell_start(BaseSequentialStream *io)
{
    static THD_WORKING_AREA(wa, SHELL_WA_SIZE);
    chThdCreateStatic(wa, sizeof(wa), NORMALPRIO, shell_spawn_thd, io);
}
