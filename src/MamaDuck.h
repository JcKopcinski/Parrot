#ifndef MAMADUCK_H
#define MAMADUCK_H

#ifdef ARDUINO
#include <Arduino.h>
#endif //ARDUINO

#include "include/Duck.h"
#include "include/DuckUtils.h"

class MamaDuck : public Duck {
public:
  using Duck::Duck;

  ~MamaDuck() {}

  using rxDoneCallback = void (*)(std::vector<uint8_t> data );
  /**
   * @brief Register callback for handling data received from duck devices
   * 
   * The callback will be invoked if the packet needs to be relayed (i.e not seen before)
   * @param cb a callback to handle data received by the papa duck
   */
  void onReceiveDuckData(rxDoneCallback cb) { this->recvDataCallback = cb; }

  /**
   * @brief Provide the DuckLink specific implementation of the base `run()`
   * method.
   *
   */
  void run();

  /**
   * @brief Override the default setup method to match MamaDuck specific
   * defaults.
   *
   * In addition to Serial component, the Radio component is also initialized.
   * When ssid and password are provided the duck will setup the wifi related
   * components.
   *
   * @param deviceId required device unique id
   * @param ssid wifi access point ssid (defaults to an empty string if not
   * provided)
   * @param password wifi password (defaults to an empty string if not provided)
   * 
   * @returns DUCK_ERR_NONE if setup is successfull, an error code otherwise.
   */
   int setupWithDefaults(std::array<uint8_t,8> deviceId, std::string ssid = "",
                            std::string password = "");

  /**
   * @brief Get the DuckType
   * 
   * @returns the duck type defined as DuckType
   */
  int getType() {return DuckType::MAMA;}

  bool getDetectState();

private :
  rxDoneCallback recvDataCallback;
  void handleReceivedPacket();

  void handleCommand(const CdpPacket & packet);
  void handleDuckCommand(const CdpPacket & packet);
};

#endif
