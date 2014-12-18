/*
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CC3200_LP.h"

#define Board_initDMA                CC3200_LP_initDMA
#define Board_initGeneral            CC3200_LP_initGeneral
#define Board_initGPIO               CC3200_LP_initGPIO
#define Board_initI2C                CC3200_LP_initI2C
#define Board_initSDSPI              CC3200_LP_initSDSPI
#define Board_initSPI                CC3200_LP_initSPI
#define Board_initUART               CC3200_LP_initUART
#define Board_initWatchdog           CC3200_LP_initWatchdog

#define Board_LED_ON                 CC3200_LP_LED_ON
#define Board_LED_OFF                CC3200_LP_LED_OFF
#define Board_LED0                   CC3200_LP_LED_D7
#define Board_LED1                   CC3200_LP_LED_D6
#define Board_LED2                   CC3200_LP_LED_D5
#define Board_BUTTON0                CC3200_LP_SW2
#define Board_BUTTON1                CC3200_LP_SW3

#define Board_gpioCallbacks0         CC3200_LP_gpioPortA1Callbacks
#define Board_gpioCallbacks1         CC3200_LP_gpioPortA2Callbacks

#define Board_I2C0                   CC3200_LP_I2C0
#define Board_I2C_TMP                CC3200_LP_I2C0

#define Board_SDSPI0                 CC3200_LP_SDSPI0

#define Board_SPI0                   CC3200_LP_SPI0

#define Board_UART0                  CC3200_LP_UART0
#define Board_UART1                  CC3200_LP_UART1

#define Board_WATCHDOG0              CC3200_LP_WATCHDOG0

/* Board specific I2C addresses */
#define Board_TMP006_ADDR            (0x41)

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
