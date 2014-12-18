Example Introduction
--------------------
This example project provides a starting point for using the "full" TI-RTOS 
implementation with the CC3200 LaunchPad. This is different to the TI-RTOS 
implementation in the CC3200 SDK, in that it not only includes SYS/BIOS 
components on TI-RTOS, but also utilises the peripheral and wifi drivers
that are supplied with TI-RTOS.

This has the advantage of making the project more self contained and easy to
distribute. It also makes for porting to other TI-RTOS supported MCUs and 
Processors much easier, including later incarnations of the CC3200. Hence
you code will be simplier to upgrade, when that time comes.

Example Summary
---------------

The project toggles the LEDs on the CC3200 LaunchPad depending on UDP Packet 
received. See details on how to use below.

The program loads the startproject Task statically. This can be changed via the
cfg script tab of the startproject.cfg

Setup Steps
-----------

You will need to have Code Composure Studio (CCS) v6 or later installed and 
TI-RTOS for SimpleLink will also need to be installed. This can be done via 
App Center, which can be accessed from within CCS via the View menu (CCS App
Center)

It is assumed that you have the CC3200 SDK installed in the default location on
the c: drive. The version that is used is CC3200SDK_1.0.0. if you are using a
later version of the SDK, then you will need to update references to the project.

If you have difficulties, you can find additional details on setting up the 
project to work with TI-RTOS at this webpage:
http://processors.wiki.ti.com/index.php/TI-RTOS_CC3200Wireless

Peripherals Exercised
---------------------
Board_LED0 - CC3200_LP_LED_D7 - Red LED
Board_LED1 - CC3200_LP_LED_D6 - Orange LED
Board_LED2 - CC3200_LP_LED_D5 - Green LED

Board_BUTTON1 - CC3200_LP_SW3 - Button SW3

WIFI - Wi-Fi is used in this example

Example Usage
-------------
Run the example. All the 3 LEDS (Green, Orange and Red will turn on) to indicate
that the program has entered main. 

You can connect to the Wi-Fi network using either Station or Access Point modes.
By default, it is set up to use SmartConfig in Station Mode (you will need to 
download the SimpleLink App from Apple App Store or Google Play). If you wish you
can also set it up in Access Point mode, by holding down Button (SW3) when starting
the LaunchPad (or you can edit the code appropriately)

Green light is on when in Station Mode (unless you altered code)
Green light is off when in AccessPoint Mode (unless you altered code)

Once connected to the network, you can now toggle LEDs by sending UDP packets to 
the CC3200 LaunchPad on port 4000

COntrol LED by Sending UDP Packet
--------------------------------

The following data sent on UDP port 4000 will control the specific LED.

Send FF00 to turn Red light off
Send FF11 to turn Red light on
Send EE00 to turn Orange light off
Send EE11 to turn Orange light on

Tool To Use To Send UDP Packets
-------------------------------

There are many tools available to send UDP packets. An easy one that I have 
used successfully and is free is the UDP Test Tool from SCT - 
https://www.simplecomtools.com/productcart/pc/viewPrd.asp?idproduct=6& 

Alternatively you can use http://www.httpdebugger.com/ which also enables
packet tracing.

There are many UDP packet senders for every platform available.

