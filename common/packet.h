#ifndef COMMON_PACKET_H_
#define COMMON_PACKET_H_

#define MAX_PACKET_SIZE 32

enum class PacketType { UNKNOWN = 0, LED };

struct PacketLED {
    PacketType packetType = PacketType::LED;
    bool on;
};

#endif  // COMMON_PACKET_H_