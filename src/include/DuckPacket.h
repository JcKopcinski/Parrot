#ifndef DUCKPACKET_H_
#define DUCKPACKET_H_

#include "../CdpPacket.h"

#ifdef ARDUINO
#include "Arduino.h"
#include <CRC32.h>
#endif //ARDUINO

#include "DuckUtils.h"
#include "cdpcfg.h"
#include "bloomfilter.h"
#include <array>
#include <cstdint>
/**
 * @brief Use this DUID to send to all PapaDucks
 * 
 */
static std::array<uint8_t,8> ZERO_DUID = {0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00};

/**
 * @brief Use this DUID to be received by every duck in the network
 * 
 */
static std::array<uint8_t,8> BROADCAST_DUID = {0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFF, 0xFF};

/**
 * @brief Use this DUID to be received by every duck in the network
 * 
 */
static std::array<uint8_t,8> PAPADUCK_DUID = {0x50, 0x61, 0x70, 0x61,
                                           0x44, 0x75, 0x63, 0x6B};

/**
 * @brief Encapsulate the protocol packet attributes and operations
 * 
 */
class DuckPacket {

public:

    /**
     * @brief Construct a new Duck Packet object.
     * 
     */
    DuckPacket() {
        buffer.reserve(256);
    }

    /**
     * @brief Construct a new Duck Packet object.
     * 
     * @param duid a duck device unique id
     * @param filter a bloom filter
     */
        DuckPacket(std::array<uint8_t,8> duid)
        : duid(duid) {
            buffer.reserve(256);
            }

    ~DuckPacket() {}
    /**
     * @brief Set device Id.
     *
     * @param duid a duck device unique id
     */
    void setDeviceId(std::array<uint8_t,8> duid) { this->duid = duid; }

    /**
     * @brief Get device Id.
     *
     * @returns a duck device unique id
     */
    std::array<uint8_t,8> getDeviceId() { return this->duid = duid; }
    /**
     * @brief Build a packet from the given topic and provided uint8_t buffer.
     *
     * @param targetDevice the target device DUID to receive the message
     * @param topic a message topic
     * @param app_data a uint8_t buffer that contains the packet data section
     * @returns DUCK_ERR_NONE if the operation was successful, otherwise an error code.
     */
    int prepareForSending(BloomFilter *filter, const std::array<uint8_t,8> targetDevice,
                          uint8_t duckType, uint8_t topic, std::vector<uint8_t> app_data);

    /**
     * @brief Update a received packet if it needs to be relayed in the mesh.
     * 
     * @param filter The bloom filter describing what packets have already been seen
     * @param dataBuffer buffer containing the packet data
     * @returns true if the packet needs to be relayed
     * @returns false if the packet does not need to be replayed
     */
    bool prepareForRelaying(BloomFilter *filter, std::vector<uint8_t> dataBuffer);
    
    /**
     * @brief Get the Cdp Packet uint8_t array.
     * 
     * @returns a array of bytes representing the cdp packet
     */
    std::vector<uint8_t> getBuffer() { return buffer;}

    /**
     * @brief Resets the packet uint8_t buffer.
     *
     */
    void reset() {
      buffer.clear();
    }

    uint8_t getTopic() { return buffer[TOPIC_POS]; }

  private: 
    std::array<uint8_t,8> duid;
    std::vector<uint8_t> buffer;
    static void getUniqueMessageId(BloomFilter * filter, uint8_t message_id[MUID_LENGTH]);

};

#endif

