#ifndef BLUETOOTH_CLASSIC_CLIENT_H_
#define BLUETOOTH_CLASSIC_CLIENT_H_

#include "common/socket.h"

class BluetoothClassicClient {
   public:
    BluetoothClassicClient();
    ~BluetoothClassicClient();

    int Connect(char* pMac);
    int SendPacket(void* pData, size_t dataSize);

   private:
    ClientSocketInfo m_socketInfo;
};

#endif  // BLUETOOTH_CLASSIC_CLIENT_H_