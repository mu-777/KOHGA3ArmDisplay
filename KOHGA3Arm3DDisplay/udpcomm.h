/*------------------------------------------------
UPDí êMÉNÉâÉX UDPComm
------------------------------------------------*/
// udpcomm.h

#pragma once

#ifndef __UDPCOMM_H__
#define __UDPCOMM_H__


#include <winsock.h>
#define _WINSOCKAPI_

#define WSOCK_SELECT (WM_APP + 1)


class UDPComm {
public:
    UDPComm();  // UDPÇÃèâä˙âª
    ~UDPComm(); // UDPÇÃå„èàóù

    void InitializeAsServer(unsigned short port);
    void InitializeAsClient(char* ip_address, unsigned short port);
    void InitializeAsBroadcastClient(unsigned short port);
    void SetUnicastSend(char* ip_address, unsigned short port);
    void SetBroadcastSend(unsigned short port);
    void SetWSAAsyncEventFlag(HWND hWnd, long lEvent);

    SOCKET GetSocket(void);
    struct sockaddr_in GetServerAddrin();
    struct sockaddr_in GetMyAddrin();
    struct sockaddr_in GetFromAddrin();

    bool CheckReadFlag(void);
    bool CheckReadFlag(long msec);

    int SendData(void* pData, int size);
    int SendDataTo(void* pData, int size, struct sockaddr* saddr);
    int SendDataToFrom(void* pData, int size);
    int ReceiveData(void* pData, int size);
    void CloseSocket();

private:
    WSAData wsaData;
    SOCKET sock;
    struct sockaddr_in server_addrin;
    struct sockaddr_in my_addrin;
    struct sockaddr_in from_addrin;
    int fromlen;

    fd_set fds, readfds;
    int n;
    struct timeval tv;
};


#endif // #ifndef __UDPCOMM_H__
