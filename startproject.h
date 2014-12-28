//*****************************************************************************
//
// Application Name        - startproject
// Application Version     - 1.2.0
// Application Modify Date - 23rd of December 2014
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
// Further Details         - If you would like to chat about your next CC3200 project
//                           then feel free contact us at contact@swiftsoftware.com.au
//
//*****************************************************************************

#ifndef STARTPROJECT_H_
#define STARTPROJECT_H_

/* XDCtools Header files */
#include <xdc/std.h>

/* SimpleLink Wi-Fi Host Driver Header files */
#include <simplelink.h>
#include <netapp.h>
//#include <netcfg.h>
//#include <osi.h>
//#include <fs.h>

 //*****************************************************************************
 // Device Defines
 //*****************************************************************************
//#define DEVICE_VERSION			"1.0.0"
//#define DEVICE_MANUFACTURE		"SWIFT_SOFTWARE"
//#define DEVICE_NAME 			"STARTPROJECT"
//#define DEVICE_MODEL			"CC3200"
//#define DEVICE_AP_DOMAIN_NAME	"startproject.net"
//#define DEVICE_SSID_AP_NAME		"StartProjectAP"

 //*****************************************************************************
 // mDNS Defines
 //*****************************************************************************
//#define MDNS_SERVICE  "._control._udp.local"
//#define TTL             120
//#define UNIQUE_SERVICE  1       /* Set 1 for unique services, 0 otherwise */

 //*****************************************************************************
 // SimpleLink/WiFi Defines
 //*****************************************************************************
//#define UDPPORT         4000 /* Port number to which the connection is bound */
//#define UDPPACKETSIZE   1024
//#define SPI_BIT_RATE    14000000
//
//#define TIMEOUT 5

 //*****************************************************************************
 // Multitasking Defines
 //*****************************************************************************
#define OOB_TASK_PRIORITY				(1)
#define OSI_STACK_SIZE					(1024)

 //*****************************************************************************
 // Assert Define
 //*****************************************************************************
#define ASSERT_ON_ERROR(line_number, error_code) \
            {\
                /* Handling the error-codes is specific to the application */ \
                if (error_code < 0) return error_code; \
                /* else, continue w/ execution */ \
            }


//*****************************************************************************
// Server Event Defines
//*****************************************************************************
/* Server Events */
#define SL_NETAPP_HTTPGETTOKENVALUE     1
#define SL_NETAPP_HTTPPOSTTOKENVALUE    2

#define SL_NETAPP_IPV4_ACQUIRED    1

//*****************************************************************************
// Date and Time Global
//*****************************************************************************
//extern SlDateTime_t dateTime;

//*****************************************************************************
// Result/Error Type
//*****************************************************************************
//typedef enum {
//	SP_SUCCESS = 0,
//	SP_ERROR = -1,
//	Sp_UNKNOWN_ERROR = -100
//} spresult_t;


#endif /* STARTPROJECT_H_ */
