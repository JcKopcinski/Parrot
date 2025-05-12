#pragma once

//Board name
#define CDP_BOARD_NAME "Raspberry Pi Zero 2W"

//OLED Display Settings
#define CDPCFG_OLED_NONE

//WIFI Settings
#define CDPCFG_WIFI_NONE

//Pinouts
struct RadioPinout {
	int nss;
	int dio1;
	int nrst;
	int busy;
	int spiBus;
};

constexpr RadioPinout PI_HAT_PINS = {
	.nss = 29,
	.dio1 = 27,
	.nrst = 1,
	.busy = 28,
	.spiBus = 0
};

//Unuused MBED Shield for now
constexpr RadioPinout MBED_SHIELD_PINS = {
	.nss = 10,
	.dio1 = 2,
	.nrst = 21,
	.busy = 0,
	.spiBus = 0
};

//get the Pinout
inline const RadioPinout& getPlatformPinout() {
	return PI_HAT_PINS;
}
