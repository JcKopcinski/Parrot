/**
 * @file DuckUtils.h
 * @brief This file is internal to CDP and provides some common utility methods.
 * @version
 * @date 2020-09-16
 *
 * @copyright
 */

#ifndef DUCKUTILS_H_
#define DUCKUTILS_H_

#include "cdpcfg.h"
#include "../compat_random.h"

#ifdef ARDUINO
#include "arduino-timer.h"
#include <Arduino.h>
#include <EEPROM.h>
#else
#pragma once
#include "../Timer.h" //this will swap the custom timer version for linux
		      //systems into the external timer for ARDUINO systems.
#include <zlib.h>
#include <string>
#endif //ARDUINO


#include <string>
#include <vector>
#include "../DuckError.h"
#include <array>
#include <cstdint>

namespace duckutils {

#ifdef ARDUINO	
extern Timer<> duckTimer;
/**
 * @brief Get a timer instance.
 * 
 * @returns A Timer instance.
 */
Timer<> getTimer();
#else
Timer<>& getTimer();
#endif //ARDUINO
extern bool detectState;

std::string getCDPVersion();

/**
 * @brief Convert a string to upper case.
 *
 * @param str the string to convert
 * @returns A string in upper case.
 */
std::string toUpperCase(std::string str);

/**
 * @brief Convert a string to a uint8_t array.
 *
 * @param str the string to convert
 * @returns A vector of bytes.
 */
#ifdef ARDUINO
std::vector<uint8_t> stringToByteVector(const String& str);
#endif //ARDUINO
/**
 * @brief Creates a uint8_t array with random alpha numerical values.
 *
 * @param length the array length
 * @param bytes array of bytes of the specified length
 */ 
void getRandomBytes(int length, uint8_t* bytes);

/**
 * @brief Create a uuid string.
 *
 * @param length the length of the UUID (defaults to CDPCFG_UUID_LEN)
 * @returns A string representing a unique id.
 */
std::string createUuid(int length = CDPCFG_UUID_LEN);

/**
 * @brief Convert a uint8_t array into a hex string.
 * 
 * @param data a uint8_t array to convert
 * @param size the size of the array
 * @returns A string representating the by array in hexadecimal.
 */
std::string convertToHex(uint8_t* data, int size);

/**
 * @brief Convert a vector into an ASCII string.
 * 
 * @param vec A vector to convert
 * @returns A std::string representing the uint8_t array in ASCII.
 * 
 */
template<typename T>
std::string toString(const std::vector<T>& vec) {
    std::string result;
    for (const auto& element : vec) {
        if (!std::isprint(element)) {
            return "ERROR: Non-printable character";
        }
        result += static_cast<char>(element);
    }
    return result;
}

/**
 * @brief Convert a vector into an ASCII string.
 *
 * @param arr A vector to convert
 * @returns A std::string representing the uint8_t array in ASCII.
 *
 */
template<typename T,size_t S>
std::string toString(const std::array<T,S>& arr) {
    std::string result;
    for (const auto& element : arr) {
        if (!std::isprint(element)) {
            return "ERROR: Non-printable character";
        }
        result += static_cast<char>(element);
    }
    return result;
}

/**
 * @brief Convert an array into hex for sending over http or display.
 *
 * @param arr an array to convert
 * @returns A std::string representing the uint8_t array in ASCII.
 *
 */
template<typename T,size_t S>
std::string arrayToHexString(const std::array<T,S>& arr) {
    std::string buf = ""; // static to avoid memory leak
    buf.clear();
    buf.reserve(S * 2); // 2 digit hex
    const char* cs = "0123456789ABCDEF";
    for (int i = 0; i < S; i++) {
        uint8_t val = arr[i];
        buf += cs[(val >> 4) & 0x0F];
        buf += cs[val & 0x0F];
    }
    return buf;
}



/**
 * @brief Compare two vectors with regard to both size and contents.
 * 
 * @param a The first vector
 * @param b The second vector
 * @returns True if a and b have the same size and contents, false if not.
 */
template<typename T>
bool isEqual(const std::vector<T> & a, const std::vector<T> & b) {
  if (a.size() != b.size()) {
    return false;
  }
  return std::equal(a.begin(), a.end(), b.begin());
}

    template<typename T,size_t S>
    bool isEqual(const std::array<T,S> & a, const std::array<T,S> & b) {
        if (a.size() != b.size()) {
            return false;
        }
        return std::equal(a.begin(), a.end(), b.begin());
    }

/**
 * @brief Convert a uint8_t array to unsigned 32 bit integer.
 * 
 * @param data uint8_t array to convert
 * @returns a 32 bit unsigned integer.
 */
uint32_t toUint32(const uint8_t* data);

bool getDetectState();
bool flipDetectState();

/**
 * @brief Save / Write Wifi credentials to EEPROM
 *
 * @param ssid        the ssid of the WiFi network
 * @param password    password to join the network
 * @return DUCK_ERR_NONE if successful, an error code otherwise.
 */
int saveWifiCredentials(std::string ssid, std::string password);

/**
 * @brief Load WiFi SSID from EEPROM
 *
 * @returns A string representing the WiFi SSID
 */
std::string loadWifiSsid();

/**
 * @brief Load WiFi password from EEPROM
 *
 * @returns A string representing the WiFi password
 */
std::string loadWifiPassword();

} // namespace duckutils
#endif
