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

/* Standard Header files */
//#include <math.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <stdbool.h>
//#include <stdlib.h>
//#include <time.h>

/* StartProject Library Headers */
#include "startproject.h"
#include "simplelinklibrary.h"

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>

/* SYS/BIOS Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
//#include <ti/sysbios/knl/Clock.h>
//#include <ti/sysbios/knl/Swi.h>
//#include <ti/sysbios/gates/GateHwi.h>
//#include <ti/sysbios/knl/Mailbox.h>
//#include <ti/sysbios/knl/Semaphore.h>

/* Peripheral Headers */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/WiFi.h>
//#include <ti/drivers/SPI.h>
//#include "driverlib/ssi.h"
//#include "driverlib/sysctl.h"

/* SimpleLink Wi-Fi Host Driver Header files */
#include <simplelink.h>
#include <netapp.h>
#include <netcfg.h>
#include <osi.h>
#include <fs.h>
#include <socket.h>
//#include <protocol.h>

#include "Board.h"

//*****************************************************************************
// Globals used by sockets and simplelink
//*****************************************************************************
uint8_t buffer[UDPPACKETSIZE];
bool deviceConnected = false;
bool ipAcquired = false;
uint32_t currButton;
uint32_t prevButton;

char returnPacket[128];

typedef struct
{
    unsigned long  ipV4;
    unsigned long  ipV4Mask;
    unsigned long  ipV4Gateway;
    unsigned long  ipV4DnsServer;
}_NetCfgIpV4Args_t;


//*****************************************************************************
// Globals for Date and Time
//*****************************************************************************
SlDateTime_t dateTime =  {0};

///////////////////////////////////////////////////////////////////////////////
// SimpleLink Functions
///////////////////////////////////////////////////////////////////////////////

//*****************************************************************************
//! SimpleLinkHttpServerCallback
//!
//! \brief This function handles callback for the HTTP server events
//!
//! \param[in]     	pServerEvent - Contains the relevant event information
//! \param[in]      pServerResponse - Should be filled by the user with the
//!					relevant response information
//!
//! \return 		None
//*****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pEvent, SlHttpServerResponse_t *pResponse)
{

    switch (pEvent->Event)
    {
        case SL_NETAPP_HTTPGETTOKENVALUE:
        {

        }
        break;

        case SL_NETAPP_HTTPPOSTTOKENVALUE:
        {

        }
        break;

        default:
        break;
    }
}

//****************************************************************************
//! HTTPServerTask
//!
//!	\brief OOB Application Main Task - Initializes SimpleLink Driver and
//!                                              Handles HTTP Requests
//! \param[in]              	pvParameters is the data passed to the Task
//!
//! \return	                	None
//
//****************************************************************************
//static void HTTPServerTask(void *pvParameters)
//{
//    //memset(g_ucSSID,'\0',AP_SSID_LEN_MAX);
//
//    //Read Device Mode Configuration
//    //ReadDeviceConfiguration();
//
//    //Connect to Network
//    //ConnectToNetwork();
//
//    //Handle Async Events
//    while(1)
//    {
//
//    }
//}

//*****************************************************************************
//! SimpleLinkSockEventHandler
//!
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    //
    // This application doesn't work w/ socket - Events are not expected
    //
       switch( pSock->Event )
    {
        case SL_SOCKET_TX_FAILED_EVENT:
            switch( pSock->EventData.status )
            {
                case SL_ECLOSE:
                    System_printf("[SOCK ERROR] - close socket (%d) operation "
                    "failed to transmit all queued packets\n\n",
                           pSock->EventData.sd);
                    System_printf("\n");
                    System_flush();
                    break;
                default:
                    System_printf("[SOCK ERROR] - TX FAILED : socket %d , reason"
                        "(%d) \n\n",
                           pSock->EventData.sd, pSock->EventData.status);
                    System_printf("\n");
                    System_flush();
            }
            break;

        default:
            System_printf("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",pSock->Event);
            System_flush();
    }
}

///*****************************************************************************
//! SimpleLinkWlanEventHandler
//!
//! SimpleLink Host Driver callback for handling WLAN connection or
//! disconnection events.
//******************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pArgs)
{
    switch (pArgs->Event) {
        case SL_WLAN_CONNECT_EVENT:
            deviceConnected = true;
            break;

        case SL_WLAN_DISCONNECT_EVENT:
            deviceConnected = false;
            break;

        default:
            break;
    }
}

///*****************************************************************************
//! SimpleLinkNetAppEventHandler
//!
//! SimpleLink Host Driver callback for asynchoronous IP address events.
//******************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pArgs)
{
    switch (pArgs->Event) {
        case SL_NETAPP_IPV4_ACQUIRED:
            ipAcquired = true;
            break;

        default:
            break;
    }
}

///*****************************************************************************
//! smartConfigFxn
//!
//! Starts the Smart Config process which allows the user to tell the CC3100
//! which AP to connect to, using a smart phone app. Downloads available here:
//! http://www.ti.com/tool/smartconfig
//******************************************************************************
void smartConfigFxn()
{
  uint8_t policyVal;

  // Turn LED to Yellow to indicate that device is in smartconfig mode
  GPIO_write(Board_LED0, Board_LED_OFF); //Blue
  GPIO_write(Board_LED1, Board_LED_ON); //Green
  GPIO_write(Board_LED2, Board_LED_ON); //Red

  /* Set auto connect policy */
  sl_WlanPolicySet(SL_POLICY_CONNECTION,
                   SL_CONNECTION_POLICY(1, NULL, NULL, NULL, NULL),
                   &policyVal,
                   sizeof(policyVal));

  /* Start SmartConfig using unsecured method. */
  sl_WlanSmartConfigStart(NULL, SMART_CONFIG_CIPHER_NONE, NULL, NULL,
                          NULL, NULL, NULL, NULL);
}


//*****************************************************************************
//! startproject
//!
//! This is the main program that establishes connectivity and then runs a program
//! loop wait to receive packets
//!
//! Task for this function is created statically. See the project's .cfg file.
//*****************************************************************************
Void startproject(UArg arg0, UArg arg1)
{
    unsigned char len = sizeof(SlNetCfgIpV4Args_t);
    unsigned char dhcpIsOn = 1;
    SlNetCfgIpV4Args_t ipV4 = {0};
    int               nbytes;
    int               status;
    int				  iretVal;
    int               selectRes;
    int               slSocket;
    fd_set            readSet;
    timeval           timeout;
    sockaddr_in       localAddr;
    sockaddr_in       client_addr;
    socklen_t         addrlen = sizeof(client_addr);
//    int			      bytesSent;
    int				  configValue;

    ULong       currButton;
    ULong		connectionButton;
    ULong       prevButton = 0;

    /* Turn Red and Orange LED OFF. It will be used as a connection indicator */
    GPIO_write(Board_LED0, Board_LED_OFF); //Red
    GPIO_write(Board_LED1, Board_LED_OFF); //Orange
    GPIO_write(Board_LED2, Board_LED_ON); //Green

    /*
     * Host driver starts the network processor.
     *
     * sl_Start returns the network processor operating mode:
     *      ROLE_STA (0x00): configured as a station
     *      ROLE_AP  (0x02): configured as an access point
     */

    configValue = sl_Start(NULL, NULL, NULL);
    if (configValue < 0) {
        System_abort("Could not initialize WiFi");
    }

    //Set Device Time and Date
    dateTime.sl_tm_day =   (_u32)21;          // Day of month (DD format) range 1-31
    dateTime.sl_tm_mon =   (_u32)12;          // Month (MM format) in the range of 1-12
    dateTime.sl_tm_year =  (_u32)2014;        // Year (YYYY format)
    dateTime.sl_tm_hour =  (_u32)20;          // Hours in the range of 0-23
    dateTime.sl_tm_min =   (_u32)30;          // Minutes in the range of 0-59
    dateTime.sl_tm_sec =   (_u32)15;          // Seconds in the range of  0-59

    iretVal = setDeviceTimeDate();
    if (iretVal < 0)
    {
    	System_printf("Failed to set Device Time and Date\n");
    	System_flush();
    }

    //Get Device Time and Date
    iretVal = getDeviceTimeDate();
    if (iretVal < 0)
    {
    	System_printf("Failed to get Device Time and Date\n");
    	System_flush();
    }
    else // Print out the Date and Time
    {
        System_printf("Day: %d", dateTime.sl_tm_day);
        System_printf(" | Month: %d", dateTime.sl_tm_mon);
        System_printf(" | Year: %d", dateTime.sl_tm_year);
        System_printf(" | Hour: %d", dateTime.sl_tm_hour);
        System_printf(" | Minute: %d", dateTime.sl_tm_min);
        System_printf(" | Second: %d", dateTime.sl_tm_sec);
        System_printf("\n\n");
    	System_flush();
    }

    //Set device name. Maximum length of 33 characters
    iretVal = setDeviceName();
    if (iretVal < 0)
    {
    	System_printf("Failed to set Device Name\n");
    	System_flush();
    }

    // Set the AP domain name
    iretVal = setApDomainName();
    if (iretVal < 0)
    {
    	System_printf("Failed to set AP Domain Name\n");
    	System_flush();
    }

    // Holding down button SW3 while booting will switch between Station or AccessPoint Modes
    // It also sets the AP mode as open security and gives it the standard AP name StartProjectAP
    connectionButton = GPIO_read(Board_BUTTON1);
    if(connectionButton != 0)
    {

		if (configValue == ROLE_STA) {

			/* Change mode to access point */
			configValue = sl_WlanSetMode(ROLE_AP);

			// Set SSID name for AP mode
			iretVal = setSsidName();
    	    if (iretVal < 0)
    	    {
    	    	System_printf("Failed to set SSID Name\n");
    	    	System_flush();
    	    }

			//unsigned char  str[33] = "StartProjectAP";
			//unsigned short  length = strlen((const char *)str);
			//sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID, length, str);

			// Set security type for AP mode
			//Security options are:
			//Open security: SL_SEC_TYPE_OPEN
			//WEP security:  SL_SEC_TYPE_WEP
			//WPA security:  SL_SEC_TYPE_WPA
			unsigned char  val = SL_SEC_TYPE_OPEN;
			sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SECURITY_TYPE, 1, (unsigned char *)&val);

			//Set Password for for AP mode (for WEP or for WPA) example:
			//Password - for WPA: 8 - 63 characters
			//           for WEP: 5 / 13 characters (ascii)
//			unsigned char  strpw[65] = "passWORD";
//			unsigned short  len = strlen((const char *)strpw);
//			memset(strpw, 0, 65);
//			memcpy(strpw, "passWORD", len);
//			sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_PASSWORD, len, (unsigned char *)strpw);

			/* Restart the network processor */
			configValue = sl_Stop(0);
			configValue = sl_Start(NULL, NULL, NULL);
		}
		else if  (configValue == ROLE_AP) {
			/* Change mode to wireless station */
			configValue = sl_WlanSetMode(ROLE_STA);

			/* Restart the network processor */
			configValue = sl_Stop(0);
			configValue = sl_Start(NULL, NULL, NULL);
		}
    }

    // We are in Station Mode, so need to connect to Wifi Router/Access Point
    if (configValue == ROLE_STA)
    {
        /* Turn Red and Orange LED OFF. It will be used as a connection indicator */
        GPIO_write(Board_LED0, Board_LED_OFF); //Red
        GPIO_write(Board_LED1, Board_LED_OFF); //Orange
        GPIO_write(Board_LED2, Board_LED_ON); //Green

		/*
		 * Wait for the WiFi to connect to an AP. If a profile for the AP in
		 * use has not been stored yet, press Board_BUTTON0 to start SmartConfig.
		 */
		while ((deviceConnected != true) || (ipAcquired != true)) {
			/*
			 *  Start SmartConfig if a button is pressed. This could be done with
			 *  GPIO interrupts, but for simplicity polling is used to check the
			 *  button.
			 */

			currButton = GPIO_read(Board_BUTTON1);
			if((currButton == 0) && (prevButton != 0))
			{
				smartConfigFxn();
			}
			prevButton = currButton;
			Task_sleep(50);
		}
    }

    // Set the color of LED to indicate which mode we are in
    if (configValue == ROLE_STA)
    {
    	System_printf("Device is in Station Mode\n");
    	System_flush();

        // Turn Green LED on to indicate that device is connected in Station Mode
        GPIO_write(Board_LED0, Board_LED_OFF); //Red
        GPIO_write(Board_LED1, Board_LED_OFF); //Orange
        GPIO_write(Board_LED2, Board_LED_ON); //Green

        // Get the IP Address details.
        sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO,&dhcpIsOn,&len,(_u8 *)&ipV4);
    }
    else
    {
    	System_printf("Device is in Access Point Mode\n");
    	System_flush();

        // Turn Green LED off to indicate that device is in AP mode
        GPIO_write(Board_LED0, Board_LED_OFF); //Red
        GPIO_write(Board_LED1, Board_LED_OFF); //Orange
        GPIO_write(Board_LED2, Board_LED_OFF); //Green

        // Get the IP Adress details
        sl_NetCfgGet(SL_IPV4_AP_P2P_GO_GET_INFO,&dhcpIsOn,&len,(_u8 *)&ipV4);
    }

    // Display the IP Address details
    System_printf("DHCP is %s | IP %d.%d.%d.%d | MASK %d.%d.%d.%d | GW %d.%d.%d.%d | DNS %d.%d.%d.%d\n",
            (dhcpIsOn > 0) ? "ON" : "OFF",
            SL_IPV4_BYTE(ipV4.ipV4,3),SL_IPV4_BYTE(ipV4.ipV4,2),SL_IPV4_BYTE(ipV4.ipV4,1),SL_IPV4_BYTE(ipV4.ipV4,0),
            SL_IPV4_BYTE(ipV4.ipV4Mask,3),SL_IPV4_BYTE(ipV4.ipV4Mask,2),SL_IPV4_BYTE(ipV4.ipV4Mask,1),SL_IPV4_BYTE(ipV4.ipV4Mask,0),
            SL_IPV4_BYTE(ipV4.ipV4Gateway,3),SL_IPV4_BYTE(ipV4.ipV4Gateway,2),SL_IPV4_BYTE(ipV4.ipV4Gateway,1),SL_IPV4_BYTE(ipV4.ipV4Gateway,0),
            SL_IPV4_BYTE(ipV4.ipV4DnsServer,3),SL_IPV4_BYTE(ipV4.ipV4DnsServer,2),SL_IPV4_BYTE(ipV4.ipV4DnsServer,1),SL_IPV4_BYTE(ipV4.ipV4DnsServer,0));
    System_flush();

    /* Register mDNS */
    iretVal = registerMdnsService();
    if (iretVal < 0)
    {
    	System_printf("mDNS Failed to Register\n");
    	System_flush();
    }
    else
    {
    	System_printf("mDNS Service %s on Port %d Successfully Registered\n", MDNS_SERVICE, UDPPORT);
    	System_flush();
    }

    /* Create a UDP socket */
    slSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (slSocket < 0)
    {
        System_printf("Error: socket not created.");
        Task_exit();
    }

    memset((char *)&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(0);
    localAddr.sin_port = htons(UDPPORT);

    status = bind(slSocket, (const sockaddr *)&localAddr, sizeof(localAddr));
    if (status < 0)
    {
        System_printf("Error: bind failed.");
        close(slSocket);
        Task_exit();
    }

    /* Loop in case remote application terminates the connection */
    while (1)
    {
        FD_ZERO(&readSet);
        FD_SET(slSocket, &readSet);

        /* Set up 1.5sec timeout for select() - May want to reduce to 1/2sec */
        //memset(&timeout, 0, sizeof(timeval));
        timeout.tv_sec = 1; //TIMEOUT;
        timeout.tv_usec = 500000;

        /* Wait for the reply. If timeout, assume UDP packet dropped */
        selectRes = select(slSocket + 1, &readSet, NULL, NULL, &timeout);

        if ((selectRes > 0) && (selectRes != -1))
        {
            if(FD_ISSET(slSocket, &readSet))
            {
                nbytes = recvfrom(slSocket, buffer, UDPPACKETSIZE, 0,(sockaddr *)&client_addr, &addrlen);
                if (nbytes > 0)
                {
                	if (((uint8_t*)buffer)[0] == 0xFF) // We have a Red light packet
                	{
                		if (((uint8_t*)buffer)[1] == 0x00) // off
                		{
                			GPIO_write(Board_LED0, Board_LED_OFF); //Red
                		}
                		else if (((uint8_t*)buffer)[1] == 0x11) // on
                		{
                			GPIO_write(Board_LED0, Board_LED_ON); //Red
                		}
                	}
                	else if (((uint8_t*)buffer)[0] == 0xEE) // We have a Orange light packet
                	{
                		 if (((uint8_t*)buffer)[1] == 0x00) // Status Query Packet Response
                		 {
                			 GPIO_write(Board_LED1, Board_LED_OFF); //Red
                		 }
                		 else if (((uint8_t*)buffer)[1] == 0x11)
                		 {
                			 GPIO_write(Board_LED1, Board_LED_ON); //Red
                		 }
                	}

                	// Send UDP Response to Client - Remove to send response
//                	char statusPacket[40] = "";
//                	strcat(statusPacket, "Packet Received: ");
//                	strcat(statusPacket, (const char *)buffer);
//
//                	const char sp[40];
//                	int spLen = strlen(statusPacket);
//                	memcpy((void *)sp, (const char *)statusPacket, spLen);
//
//                	bytesSent = sendto(slSocket, sp, spLen, 0, (sockaddr *)&client_addr, sizeof(client_addr));
                }
            }
        }
        else if (selectRes == -1)
        {
            System_printf("Closing socket 0x%x.\n", slSocket);
            close(slSocket);
            Task_exit();
        }
    }
}
