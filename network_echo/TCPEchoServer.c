#include <stdio.h>      //printf(), fprintf()
#include <sys/socket.h> //socket(), bind(), connect()
#include <arpa/inet.h>  //sockaddr_in, inet_ntoa()
#include <stdlib.h>     //atoi()
#include <string.h>     //memset()
#include <unistd.h>     //close()

#define MAXPENDING 5    //受信バッファのサイズ

void DieWithError(char *errorMessage); //エラー処理関数
void HandleTCPClient(int clntSocket);  //TCPクライアント処理関数

int main(int argc, char *argv[])
{
    int servSock;                    //サーバのソケットディスククリプタ
    int clntSock;                    //クライアントのソケットディスククリプタ
    struct sockaddr_in echoServAddr; //ローカルアドレス
    struct sockaddr_in echoClntAddr; //クライアントのアドレス
    unsigned short echoServPort;     //サーバポート
    unsigned int clntLen;      //クライアントのアドレス構造体の長さ

    //引数の数が正しいか確認
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);     //1つめの引数:サーバのIPアドレス(ドット10進表記)

    //着信接続用のソケットを作成
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    //ローカルのアドレス構造体を作成
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);

    //ローカルアドレスへバインド
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    //「接続要求をリスン中」というマークをソケットに付ける
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    while(1)
    {
        //入出力パラメータのサイズをセット
        clntLen = sizeof(echoClntAddr);

        //クライアントからの接続要求を待機
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");

        //clntSockはクライアントに接続済み
        printf("Handling chient %s\n", inet_ntoa(echoClntAddr.sin_addr));

        HandleTCPClient(clntSock);
    }
}
