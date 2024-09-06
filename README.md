# EE456
# LoRa 1262 Project

**Class:** EE456  
**Project Name:** LoRa 1262

This project utilizes the SX1262 chip and Raspberry Pi to transmit and receive data using LoRa and FSK modulation.

***********VERY IMPORTANT***********
- Enable SPI before attempting to transmit or receive.   
Run the following commands:                              
`sudo raspi-config`                                      
Navigate to `Interfacing Options` -> `SPI` and enable it.
Reboot your Raspberry Pi.                                
**********************************************************

## Installation Instructions

--Run  sudo apt update && sudo apt install -y cmake git


1. Clone the GitHub Repository for EE456: 
`git clone https://github.com/BenDuval/EE456.git`
-- Navigate into the 'EE456' directory.
3. Navigate to 'RadioLib/examples/NonAudrino/Raspberry/'
-- run
   mkdir build
   cd build
   cmake ..
   make
4. Navigate to 'WiringPi' directory:
-- run
   ./build 
6. Build the Project: 
Navigate to the `src` directory: 
`cd ~/EE456/src/` 
--run
 cmake ../src   ***cmake must be ran after any changes to CMakeLists.txt (such as adding a new executable!)
then run
make         **** make must be ran after all changes to .cpp files**

## Wiring Diagram for WiringPi Library & SX1262 MBED Shield
- NSS: GPIO 8, WPI# 10  
- Reset: GPIO 5, WPI# 21  
- DI01: GPIO 27, WPI# 2  
- Busy: GPIO 17, WPI# 0  
Ensure your wiring matches the above configuration for proper operation with the RPI and MBED shield.

## Wiring Diagram for WiringPi Library & SX1262 LORAWAN Pi Hat 
- NSS: GPIO 21, WPI# 29  
- Reset: GPIO 18, WPI# 1  
- DI01: GPIO 16, WPI# 27  
- Busy: GPIO 20, WPI# 28  
Ensure your wiring matches the above configuration for proper operation with the RPI and SX1262 LORAWAN Pi Hat.

## Running the Examples

1. Transmit Example: 
Run the `LoRaTX.cpp` example to confirm transmission without errors using: `sudo ./LoRaTX.cpp`.
2. Receive Example: 
Run the `LoRaRX.cpp` example to confirm initialization without any errors: `sudo ./LoRaRX.cpp`. Check for errors and verify successful reception.

## Additional Resources

- [RadioLib Documentation](https://jgromes.github.io/RadioLib/index.html)
- [RadioLib GitHub Repository](https://github.com/jgromes/RadioLib)
- [WiringPi Pinout](https://pinout.xyz/pinout/wiringpi)
- [WiringPi GitHub Repository](https://github.com/WiringPi/WiringPi)
- [Waveshare SX1262 LoRaWAN Hat](https://www.waveshare.com/sx1262-lorawan-hat.htm)
- [Semtech SX1262 Product Page](https://www.semtech.com/products/wireless-rf/lora-connect/sx1262)
