#include "bluetooth_classic_client.h"
#include <stdio.h>
#include <unistd.h>
#include "common/log.h"
#include "common/packet.h"

BluetoothClassicClient::BluetoothClassicClient() {}

BluetoothClassicClient::~BluetoothClassicClient() {
    // todo - how to handle closing out
    close(m_socketInfo.socket);
}

int BluetoothClassicClient::Connect(char* pMac) {
    int status;

    strncpy(m_socketInfo.mac, pMac, MAX_MAC_SIZE);

    // allocate socket
    m_socketInfo.socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (m_socketInfo.socket < 0) {
        LOG("ERROR: Opening socket\n");
        return -1;
    } else {
        LOG("Socket Created!\n");
    }

    // bind socket to port 1 of the first available
    m_socketInfo.server.rc_family = AF_BLUETOOTH;
    m_socketInfo.server.rc_channel = (uint8_t)1;
    str2ba(m_socketInfo.mac, &m_socketInfo.server.rc_bdaddr);

    status = connect(m_socketInfo.socket, (sockaddr*)&m_socketInfo.server, m_socketInfo.socket_length);
    if (status < 0) {
        LOG("Connection error\n")
        return status;
    } else {
        LOG("Connected!\n");
    }

    return status;
}

int BluetoothClassicClient::SendPacket(void* pData, size_t dataSize) {
    int status;

    status = send(m_socketInfo.socket, pData, dataSize, 0);
    if (status < 0) {
        LOG("Send failed\n");
    }

    return status;
}

int main(int argc, char* argv[]) {
    int status;

    (void)argv;

    // current pi's mac
    char defaultMac[MAX_MAC_SIZE] = "B8:27:EB:05:03:AE";

    // first checks arguments
    if (argc < 2) {
        LOG("Need to pass MAC, now will use default %s\n", defaultMac);
    } else {
        strncpy(defaultMac, argv[2], MAX_MAC_SIZE);
    }

    BluetoothClassicClient client;
    status = client.Connect(defaultMac);

    if (status >= 0) {
        PacketLED packet;
        packet.on = true;
        status = client.SendPacket(&packet, sizeof(PacketLED));

        packet.on = false;
        status = client.SendPacket(&packet, sizeof(PacketLED));
    }

    return status;
}