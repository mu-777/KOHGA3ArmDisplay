// udpcomm.cpp

//#pragma comment(lib,"wsock32.lib")    // WinSock1.1(32bit)ライブラリと静的にリンク
#pragma comment(lib,"ws2_32.lib")   // WinSock2(32bit)ライブラリと静的にリンク

#include "udpcomm.h"
#include "stdio.h"


//コンストラクタ
UDPComm::UDPComm() {
    /*
        // winsock2の初期化
        if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
            MessageBox( NULL, TEXT("winsock2 init ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK );
        }

        // UDPソケットの作成
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == INVALID_SOCKET) {
            MessageBox( NULL, TEXT("socket making ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK );
        }

        FD_ZERO(&readfds); // fd_setの初期化
        FD_SET(sock, &readfds); // selectで待つ読み込みソケットを登録
        // 0.001秒でタイムアウト
        tv.tv_sec = 0;
        tv.tv_usec = 1;

        fromlen = (int)sizeof(from_addrin);
    */
}


//デストラクタ
UDPComm::~UDPComm() {
    if (sock != 0) {
        // 通信セッションの終了
        closesocket(sock);

        // winsock2の終了処理
        WSACleanup();
    }
}


void UDPComm::CloseSocket() {
    // 通信セッションの終了
    closesocket(sock);

    // winsock2の終了処理
    WSACleanup();
}


void UDPComm::InitializeAsServer(unsigned short port) {
    // winsock2の初期化
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        MessageBox(NULL, TEXT("winsock2 init ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }

    // UDPソケットの作成
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        MessageBox(NULL, TEXT("socket making ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }

    FD_ZERO(&readfds); // fd_setの初期化
    FD_SET(sock, &readfds); // selectで待つ読み込みソケットを登録
    // 0.001秒でタイムアウト
    tv.tv_sec = 0;
    tv.tv_usec = 1;

    fromlen = (int)sizeof(from_addrin);



    // ここでserver（自分）のアドレスを設定する
    memset(&server_addrin, 0, sizeof(server_addrin));
    server_addrin.sin_family = AF_INET;
    server_addrin.sin_port = htons(port);
    server_addrin.sin_addr.S_un.S_addr = INADDR_ANY; //addrin.sin_addr.s_addr = htonl(INADDR_ANY);

    my_addrin = server_addrin;

    // ソケットにローカルアドレスを関連付ける
    if (bind(sock, (struct sockaddr*)&server_addrin, (int)sizeof(server_addrin)) == SOCKET_ERROR) {
        MessageBox(NULL, TEXT("socket binding ERROR in InitializeAsServer.\nClosing socket."), TEXT("UDP"), MB_ICONERROR | MB_OK);
        CloseSocket();
    } else {
    }
}


void UDPComm::InitializeAsClient(char* ip_address, unsigned short port) {
    // winsock2の初期化
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        MessageBox(NULL, TEXT("winsock2 init ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }

    // UDPソケットの作成
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        MessageBox(NULL, TEXT("socket making ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }

    FD_ZERO(&readfds); // fd_setの初期化
    FD_SET(sock, &readfds); // selectで待つ読み込みソケットを登録
    // 0.001秒でタイムアウト
    tv.tv_sec = 0;
    tv.tv_usec = 1;

    fromlen = (int)sizeof(from_addrin);


    // ユニキャストに設定
    SetUnicastSend(ip_address, port);


    // 以下のbindはclientにはやる必要はないかも

    // 自分(client)のアドレスのバインド
    // ソケットにローカルアドレスを関連付ける
    memset(&my_addrin, 0, sizeof(my_addrin));
    my_addrin.sin_family = AF_INET;
    my_addrin.sin_port = 0; // システムにお任せという意味
    my_addrin.sin_addr.S_un.S_addr = INADDR_ANY; //addrin.sin_addr.s_addr = htonl(INADDR_ANY);

    // ソケットにローカルアドレスを関連付ける
    // 適当なアドレスがソケットに割り当てられる
    if (bind(sock, (struct sockaddr*)&my_addrin, (int)sizeof(my_addrin)) == SOCKET_ERROR) {
        MessageBox(NULL, TEXT("socket binding ERROR in InitializeAsClient.\nClosing socket."), TEXT("UDP"), MB_ICONERROR | MB_OK);
        CloseSocket();
    } else {
    }
}


void UDPComm::InitializeAsBroadcastClient(u_short port) {
    // winsock2の初期化
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        MessageBox(NULL, TEXT("winsock2 init ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }

    // UDPソケットの作成
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        MessageBox(NULL, TEXT("socket making ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }

    FD_ZERO(&readfds); // fd_setの初期化
    FD_SET(sock, &readfds); // selectで待つ読み込みソケットを登録
    // 0.001秒でタイムアウト
    tv.tv_sec = 0;
    tv.tv_usec = 1;

    fromlen = (int)sizeof(from_addrin);


    // ブロードキャストに設定
    SetBroadcastSend(port);

    // 以下のbindはclientにはやる必要はないかも

    // 自分(client)のアドレスのバインド
    // ソケットにローカルアドレスを関連付ける
    memset(&my_addrin, 0, sizeof(my_addrin));
    my_addrin.sin_family = AF_INET;
    my_addrin.sin_port = 0; // システムにお任せという意味
    my_addrin.sin_addr.S_un.S_addr = INADDR_ANY; //addrin.sin_addr.s_addr = htonl(INADDR_ANY);

    // ソケットにローカルアドレスを関連付ける
    // 適当なアドレスがソケットに割り当てられる
    if (bind(sock, (struct sockaddr*)&my_addrin, (int)sizeof(my_addrin)) == SOCKET_ERROR) {
        MessageBox(NULL, TEXT("socket binding ERROR in InitializeAsClient.\nClosing socket."), TEXT("UDP"), MB_ICONERROR | MB_OK);
        CloseSocket();
    } else {
    }
}


void UDPComm::SetUnicastSend(char* ip_address, unsigned short port) {
    // ここでserver（相手）のアドレスを設定する
    server_addrin.sin_family = AF_INET;
    server_addrin.sin_port = htons(port);

    if (inet_addr(ip_address) != -1) {
        server_addrin.sin_addr.S_un.S_addr = inet_addr(ip_address);
    } else {
        MessageBox(NULL, TEXT("ERRO in SetUnicastSend"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }
}


void UDPComm::SetBroadcastSend(unsigned short port) {
    BOOL yes = 1;

    // ここでserver（相手）のアドレスを設定する
    server_addrin.sin_family = AF_INET;
    server_addrin.sin_port = htons(port);

    server_addrin.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
}



void UDPComm::SetWSAAsyncEventFlag(HWND hWnd, long lEvent) {
    // WSAAsyncSelect(s, hDlg, MY_MSG, FD_CONNECT | FD_READ | FD_CLOSE)
    // ソケット関連イベントをウィンドウメッセージで通知されるようにする
    if (WSAAsyncSelect(sock, hWnd, WSOCK_SELECT, lEvent) == SOCKET_ERROR) {
        MessageBox(NULL, TEXT("SetWSAAsyncEventFlag ERROR"), TEXT("UDP"), MB_ICONERROR | MB_OK);
    }
}


SOCKET UDPComm::GetSocket(void) {
    return sock;
}


struct sockaddr_in UDPComm::GetServerAddrin() {
    return server_addrin;
}


struct sockaddr_in UDPComm::GetMyAddrin() {
    return my_addrin;
}


struct sockaddr_in UDPComm::GetFromAddrin() {
    return from_addrin;
}



bool UDPComm::CheckReadFlag(void) {

    fds = readfds;
    //  memcpy(&fds, &readfds, sizeof(fd_set)); // 読み込み用fd_setの初期化

    // Windowsでは，select関数の第1引数は，unixのselect関数に合わせているだけで，意味はない
    //n = select(0, &fds, NULL, NULL, &tv); // fdsに設定されたソケットが読み込み可能になるまで待つ
    n = select((int)sock + 1, (fd_set*)&fds, NULL, NULL, &tv); // fdsに設定されたソケットが読み込み可能になるまでtvだけ待つ

    if (n == 0) { // select でタイムアウトした
        return false;
    } else if (n == -1) { // select error
        return false;
    }
    if (FD_ISSET(sock, &fds)) { // sockに読み込み可能データがある場合
        return true;
    }

    return false;
}


bool UDPComm::CheckReadFlag(long msec) {
    struct timeval ltv;

    // msecミリ秒でタイムアウト
    ltv.tv_sec = 0;
    ltv.tv_usec = msec;

    memcpy(&fds, &readfds, sizeof(fd_set)); // 読み込み用fd_setの初期化
    //n = select(0, &fds, NULL, NULL, &ltv); // fdsに設定されたソケットが読み込み可能になるまで待つ
    n = select((int)sock + 1, &fds, NULL, NULL, &ltv); // fdsに設定されたソケットが読み込み可能になるまで待つ

    if (n == 0) { // タイムアウトした
        return false;
    } else if (n == -1) { // select error
        return false;
    }
    if (FD_ISSET(sock, &fds)) { // sockに読み込み可能データがある場合
        return true;
    }

    return false;
}


int UDPComm::SendData(void* pData, int size) {
    // データ送信
    int n = sendto(sock, (const char*)pData, size, 0, (struct sockaddr*)&server_addrin, sizeof(server_addrin));

    // エラーメッセージは出さないようにした
    //if (n != size) {
    //  char text[50];

    //  DWORD lasterror = GetLastError( );
    //  if (lasterror == 10054) return n;
    //  itoa(lasterror,text,10);
    //  MessageBox( NULL, TEXT(text), TEXT("SendData GetLastError value"), MB_ICONERROR | MB_OK );

    //  MessageBox( NULL, TEXT("ERROR at SendData"), TEXT("UDP"), MB_ICONERROR | MB_OK );
    //  //CloseSocket ();
    //}

    return n;
}


int UDPComm::SendDataTo(void* pData, int size, struct sockaddr* saddr) {
    // データ送信
    int n = sendto(sock, (const char*)pData, size, 0, saddr, sizeof(struct sockaddr));

    // エラーメッセージは出さないようにした
    //if (n != size) {
    //  char text[50];

    //  DWORD lasterror = GetLastError( );
    //  itoa(lasterror,text,10);
    //  MessageBox( NULL, TEXT(text), TEXT("SendDataTo GetLastError value"), MB_ICONERROR | MB_OK );
    //  if (lasterror == 10093) return n;


    //  //MessageBox( NULL, TEXT("ERROR at SendData"), TEXT("UDP"), MB_ICONERROR | MB_OK );
    //  //CloseSocket ();
    //}

    return n;
}


int UDPComm::SendDataToFrom(void* pData, int size) {
    // データ送信
    int n = sendto(sock, (const char*)pData, size, 0, (struct sockaddr*)&from_addrin, sizeof(from_addrin));

    // エラーメッセージは出さないようにした
    //if (n != size) {
    //  MessageBox( NULL, TEXT("ERROR at SendData"), TEXT("UDP"), MB_ICONERROR | MB_OK );
    //  //CloseSocket ();
    //}

    return n;
}


int UDPComm::ReceiveData(void* pData, int size) {
    // データ受信
    memset(pData, 0, sizeof(pData));
    //  int n = recv (sock, (char*)pData, size, 0);
    int n = recvfrom(sock, (char*)pData, size, 0, (struct sockaddr*)&from_addrin, &fromlen);

    if (n == SOCKET_ERROR) {
        printf("UDP ERROR!\n");
    }


    // エラーメッセージは出さないようにした
    //if (n == SOCKET_ERROR) {
    //  char text[50];

    //  DWORD lasterror = GetLastError( );
    //  //if (lasterror == 10054) return n;
    //  itoa(lasterror,text,10);
    //  MessageBox( NULL, TEXT(text), TEXT("ReceivedData GetLastError value"), MB_ICONERROR | MB_OK );

    //  MessageBox( NULL, TEXT("SOCKET_ERROR at ReceiveData"), TEXT("UDP"), MB_ICONERROR | MB_OK );
    //  //CloseSocket ();


    //  itoa(n,text,10);
    //  MessageBox( NULL, TEXT(text), TEXT("ReceivedData returned value"), MB_ICONERROR | MB_OK );
    //}

    return n;
}
