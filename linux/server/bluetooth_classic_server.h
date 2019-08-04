#ifndef BLUETOOTH_CLASSIC_SERVER_H_
#define BLUETOOTH_CLASSIC_SERVER_H_

#include "common/packet.h"
#include "common/socket.h"

class BluetoothClassicServer {
   public:
    BluetoothClassicServer();
    ~BluetoothClassicServer();

    int Setup();
    void Listen();

   private:
    ServerSocketInfo m_socketInfo;
};

#endif  // BLUETOOTH_CLASSIC_SERVER_H_