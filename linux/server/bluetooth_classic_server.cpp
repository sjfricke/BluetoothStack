#include "bluetooth_classic_server.h"
#include <bluetooth/hci.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "common/log.h"

BluetoothClassicServer::BluetoothClassicServer() {}

BluetoothClassicServer::~BluetoothClassicServer() {
    // todo - how to handle closing out
    close(m_socketInfo.socket);
}

int BluetoothClassicServer::Setup() {
    int status;

    // Open HCI socket to be discoverable
    int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
    if (ctl < 0) {
        LOG_FATAL("Can't open HCI socket\n");
    }

    struct hci_dev_req dr;
    dr.dev_id = 0;  // hci0
    dr.dev_opt = SCAN_DISABLED;
    dr.dev_opt = SCAN_PAGE | SCAN_INQUIRY;

    if (ioctl(ctl, HCISETSCAN, (unsigned long)&dr) < 0) {
        LOG_FATAL("Can't set scan mode on hci%u: %s (%d)\n", dr.dev_id, strerror(errno), errno);
    } else {
        LOG("hci%u is now discoverable\n", dr.dev_id);
    }

    close(ctl);

    // allocate socket
    m_socketInfo.socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (m_socketInfo.socket < 0) {
        LOG_FATAL("ERROR: Opening socket\n");
    } else {
        LOG("Socket Created!\n");
    }

    // BDADDR_ANY from bluetooth.h as C++ won't let a temp constructor
    bdaddr_t bdaddrAny = {{0, 0, 0, 0, 0, 0}};

    // bind socket to port 1 of the first available
    m_socketInfo.server.rc_family = AF_BLUETOOTH;
    m_socketInfo.server.rc_bdaddr = bdaddrAny;
    m_socketInfo.server.rc_channel = (uint8_t)1;
    status = bind(m_socketInfo.socket, (sockaddr*)&m_socketInfo.server, sizeof(sockaddr_rc));

    if (status < 0) {
        LOG_FATAL("ERROR binding socket\n")
    } else {
        LOG("Socket Binded!\n");
    }

    return status;
}

void BluetoothClassicServer::Listen() {
    // start listening, allowing a queue of up to 1 pending connection
    listen(m_socketInfo.socket, 1);
    LOG("Socket Listening!\n");

    // prevents daemon from closing on a closed client
    signal(SIGPIPE, SIG_IGN);

    // keeps daemon running forever
    for (;;) {
        // blocks until handshake is made
        m_socketInfo.connection =
            accept(m_socketInfo.socket, (sockaddr*)&m_socketInfo.client, &m_socketInfo.socket_length);

        ba2str(&m_socketInfo.client.rc_bdaddr, m_socketInfo.mac);
        LOG("Connected to %s\n", m_socketInfo.mac);

        uint8_t packetBuffer[MAX_PACKET_SIZE];
        ssize_t packetSize = 1;

        // main loop to wait for a request
        while (m_socketInfo.connection && packetSize > 0) {
            //            packetSize = read(m_socketInfo.connection, packetBuffer, MAX_PACKET_SIZE);
            packetSize = recv(m_socketInfo.connection, packetBuffer, MAX_PACKET_SIZE, 0);
            if (packetSize < 0) {
                LOG("error on recv\n");
                break;
            } else if (packetSize == 0) {
                LOG("Packet size of 0\n");
                break;
            }

            PacketType packetType = *reinterpret_cast<PacketType*>(&(packetBuffer[0]));
            switch (packetType) {
                case PacketType::LED: {
                    PacketLED packet = *reinterpret_cast<PacketLED*>(&(packetBuffer[0]));
                    LOG("LED Packet - on : %d\n", packet.on);
                    break;
                }

                case PacketType::UNKNOWN: {
                    LOG("Uknown Packet Type");
                    break;
                }
            }

            // clears receive message buffer
            memset(packetBuffer, 0, MAX_PACKET_SIZE);

        }  // end of connection while loop

        close(m_socketInfo.connection);
        LOG("Client dropped connection\n");

    }  // end forever loop
}

int main(int argc, char* argv[]) {
    int status;

    (void)argc;
    (void)argv;

    BluetoothClassicServer server;
    status = server.Setup();
    if (status == 0) {
        server.Listen();
    }

    return status;
}
