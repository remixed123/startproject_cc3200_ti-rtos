/*
 * startprojectlibrary.h
 *
 *  Created on: 22/12/2014
 *      Author: glenn
 */

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
