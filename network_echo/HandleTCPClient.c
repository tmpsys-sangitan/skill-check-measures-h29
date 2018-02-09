#include <stdio.h>                  //printf(), fprintf()
#include <sys/socket.h>             //socket(), connect(), send(), recv()
#include <unistd.h>                 //close()
#define RCVBUFSIZE 32               //受信バッファのサイズ

void DieWithError(char *errorMessage);                   //エラー処理関数

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];               //エコー文字列用のバッファ
    int recvMsgSize;                           //受信メッセージのサイズ

    //クライアントからの受信メッセージ
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    //受信した文字列を送信し、転送が終了していなければ次を受信する
    while (recvMsgSize > 0)                     //0は転送の終了を意味する
    {
        // [追加] SV>付与
        if (send(clntSocket, "SV>", 3, 0) != 3)
            DieWithError("send() failed");

        //メッセージをクライアントにエコーバックする
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        //受信するデータが残っていないか確認する
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0))<0)
            DieWithError("recv() failed");
    }

    //クライアントソケットをクローズする
    close(clntSocket);
}
