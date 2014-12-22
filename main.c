//*****************************************************************************
//
// Application Name        - startproject
// Application Version     - 1.0.0
// Application Modify Date - 18th of December 2014
// Application Developer   - Glenn Vassallo
// Application Contact	   - contact@swiftsoftware.com.au
// Application Repository  - https://github.com/remixed123/startproject
//
// Application Overview    - This example project provides a starting
//                           point for using the "full" TI-RTOS implementation
//                           with the CC3200 LaunchPad. This is different
//                           to the TI-RTOS implementation in the CC3200
//                           SDK, in that it not only includes SYS/BIOS
//                           components on TI-RTOS, but also utilises the
//                           peripheral and wifi drivers that are supplied
//                           with TI-RTOS.
//
// Application Details     - https://github.com/remixed123/startproject/readme.txt
//
//
// Further Details         - If you would like to chat about your next CC3200 project
//                           then feel free contact us at contact@swiftsoftware.com.au
//
//*****************************************************************************

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"

///* SimpleLink Wi-Fi Host Driver Header files */
//#include <simplelink.h>
#include <osi.h>

/* Spawn Task Priority */
int SPAWN_TASK_PRI = 3;

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the StartProject example\nSystem provider is set"
                  " to SysMin. Halt the target to view any SysMin contents in"
                  " ROV.\n\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Turn off All LEDs. It will be used as a connection indicator */
    GPIO_write(Board_LED0, Board_LED_ON); //Red
    GPIO_write(Board_LED1, Board_LED_ON); //Orange
    GPIO_write(Board_LED2, Board_LED_ON); //Green

    /*
     * The SimpleLink Host Driver requires a mechanism to allow functions to
     * execute in temporary context.  The SpawnTask is created to handle such
     * situations.  This task will remain blocked until the host driver
     * posts a function.  If the SpawnTask priority is higher than other tasks,
     * it will immediately execute the function and return to a blocked state.
     * Otherwise, it will remain ready until it is scheduled.
     */
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRI);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
