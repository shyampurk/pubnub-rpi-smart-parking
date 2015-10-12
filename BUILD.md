#BUILD AND INSTALL

##ASSUMPTIONS

1. POSIX build setup is expected to be available either on Raspberry Pi directly or on a linux build server.
2. Arduino IDE should be installed for building the sensor controller to be executed in ATMega8.
3. Cordova should be installed on the build system for building the mobile app. Also Android 21 needs to be installed from Android package installer.

##UART CONFIGURATION ON RASPBERRY Pi

Step 1: Configure UART or Raspberry Pi ( Optional step, not required for latest raspbian version 4.1 + )

Edit /etc/inittab .This file has the command to enable the login prompt and this needs to be disabled. 
If you do not find this file and your raspbian kernel version is 4.1+ then you can ignore this step. Else follow the steps below

 a) To edit the file run, 
 
 		nano /etc/inittab

 b) Move to the end of the file until you find

		T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100

Disable it by adding a # character to the beginning. Save the file.

		#T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100


Step 2: Edit /boot/cmdline.txt to avoid the bootup information which is sent to the Serial port during the Raspberry Pi Boot up by disabling it

To edit the file run, 

		nano /boot/cmdline.txt

The contents of the file look like this:

		dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait

Remove all references to ttyAMA0 (which is the name of the serial port). 

The file will now look like this:

		dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait


Step 3: Reboot the Raspberry Pi

In order you enable the changes you have made, you will need to reboot the Raspberry Pi

		sudo shutdown -r now 


Step 4: Install the minicom to the Raspberry Pi to ensure that the uart is ready to used by the program

To install the minicom, just run
	
		sudo apt-get install minicom

Step 5: Once installation is done , perform the following operation to verify UART communication

Connect your PC to the Raspberry Pi serial port using an appropriate serial port adapter and wiring, then open Putty or a similar serial terminal program on PC side. Setup a connection using the serial port at 9600 baud.

Now run minicom on the Raspberry Pi using

minicom -b 9600 -o -D /dev/ttyAMA0

What you type into the minicom terminal screen should appear on the serial PC terminal and vice versa.


##MASTER CONTROLLER BUILD

Steps to be followed to Build the Master software of Smart Parking System on Raspberry Pi

Step 1: Clone this repository pubnub-rpi-smart-parking from the github
		
		git clone https://github.com/shyampurk/pubnub-rpi-smart-parking.git

Step 2: Change the directory to the pubnub-rpi-smart-parking

		cd pubnub-rpi-smart-parking

Step 3:	Clone the c-core library from the pubnub

		git clone https://github.com/pubnub/c-core.git


Step 4: Modify the Pubnub Publish and Subscribe Keys in the Rpi-Server/pubnub_hc04.c

Step 5: To Build using the Makefile, just run

		make

Step 6:	You should see the executable file named /pubnub_hc04

##SENSOR CONTROLLER BUILD
Steps to be followed to build and upload the sensor controller or ATMega8 

Before preceeding, we need to install Arduino IDE
Installing the Arduino IDE

    - Download and Install Latest Version of Arduino IDE from 

    				[https://www.arduino.cc/en/Main/Software]

    - Start Arduino IDE and Plug the Development Board



Step 1: Open the Device/HC-SR04.ino from this repo in Arduino IDE

Step 2: Select the Board from Tools - > Board - > Arduino Uno 

Step 3: Select the USB Port from Tools - > Port - > [/dev/ttyXXX]

Step 4: Upload the Code to the ATmega8 AVR Controller


##MOBILE APP BUILD
Steps to be followed to Build and Run the Android App for Smart Parking System

Step 1 : Change the directory to the pubnub-rpi-smart-parking/app-android

		 cd pubnub-rpi-smart-parking/app-android

Step 2 : Build the .apk file using,
		
		 cordova build android 

Step 3 : Once the .apk file is build successfully, you will find the app at this path

		 ./platforms/android/ant-build/MainActivity-debug.apk

Step 4 : Install the App in the Android Phone. 
