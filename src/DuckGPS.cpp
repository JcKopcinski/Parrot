#include <iostream>
#include <string>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <sstream>
#include <cstdint>
#include <RadioLib.h>
#include "include/DuckRadio.h"

DuckRadio& duckRadio = DuckRadio::getInstance();

void onInterrupt(){
	DuckRadio::onInterrupt();
}

void transmitData(const std::string& gpsMessage);

void parseNMEA(const std::string &line){
	if(line.rfind("$GNGGA", 0) == 0){
		std::istringstream ss(line);
		std::string token;
		int idx = 0;
		std::string latitude, lat_dir, longitude, lon_dir;
		while(std::getline(ss, token, ',')){
			switch(idx){
				case 2: latitude = token; break;
				case 3: lat_dir = token; break;
				case 4: longitude = token; break;
				case 5: lon_dir = token; break;
				default: break;
			}
			idx++;
		}
		if(!latitude.empty() && !longitude.empty()){
			std::ostringstream msg;
			msg << "Latitude: " << latitude << " " << lat_dir
			    << "Longitude: " << longitude << " " << lon_dir;

			std::cout << msg.str() << std::ends;
			transmitData(msg.str());
			}
	}
}
		
void transmitData(const std::string& msg){
	std::vector<uint8_t> packet(msg.begin(), msg.end());
	int state = duckRadio.sendData(packet);

	if(state == 0){
		std::cout << "LoRa Transmission Success!" << state << std::ends;
	}
	else{
		std::cerr << "LoRa Transmission Failed, code" << state << std::ends;
	}
}
	
void readData(int serial_port){
	std::string buffer;
	char c;
	while(true){
		if(read(serial_port, &c, 1) > 0){
			if(c == '\n'){
				parseNMEA(buffer);
				buffer.clear();
			}
			else if(c != '\r'){
				buffer += c;
			}
		}
	}
}

int main(){
	LoraConfigParams cfg;
	cfg.band = 915.0;
	cfg.txPower = 10;
	cfg.bw = 125.0;
	cfg.sf = 7;
	cfg.gain = 1;
	cfg.func = onInterrupt;

	int setupState = duckRadio.setupRadio(cfg);
	if (setupState != 0){
		std::cerr << "Radio setup failed, code: " << setupState << std::ends;
		return 1;
	}

	std::cout << "Radio setup successful" << std::ends;

	int serial_port = open("/dev/serial0", O_RDWR | O_NOCTTY);
	if(serial_port < 0){
		std::cerr << "Error opening serial port\n";
		return 1;
	}

	struct termios tty;
	tcgetattr(serial_port, &tty);
	tty.c_cflag = 89600 | CS8 | CLOCAL | CREAD;
	tty.c_iflag = IGNPAR;
	tcsetattr(serial_port, TCSANOW, &tty);

	readData(serial_port);
	close(serial_port);
	return 0;
}

