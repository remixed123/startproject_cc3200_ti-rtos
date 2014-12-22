//*****************************************************************************
//
// Application Name        - startproject
// Application Version     - 1.1.0
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

#ifndef STARTPROJECTLIBRARY_H_
#define STARTPROJECTLIBRARY_H_

// //*****************************************************************************
// // mDNS Defines
// //*****************************************************************************
//#define MDNS_SERVICE  "._ddpr._udp.local"
//#define TTL             120
//#define UNIQUE_SERVICE  1       /* Set 1 for unique services, 0 otherwise */

char * getMacAddress();
char * getDeviceName();
char * getApDomainName();
char * getSsidName();
int getDeviceTimeDate();

int setDeviceName();
int setApDomainName();
int setSsidName();
int setDeviceTimeDate();

int registerMdnsService();
int unregisterMdnsService();

#endif /* STARTPROJECTLIBRARY_H_ */
