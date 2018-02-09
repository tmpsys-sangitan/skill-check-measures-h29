#include <stdio.h>                      // printf(), fprintf()
#include <sys/socket.h>                 // socket(), connect(), send(), recv()
#include <arpa/inet.h>                  // sockaddr_in, inet_addr()
#include <stdlib.h>                     // atoi()
#include <string.h>                     // memset()
#include <unistd.h>                     // close()

#define RCVBUFSIZE 32                   // 受信バッファのサイズ
#define QUITMES "quit"                  // 終了コマンド

void DieWithError(char *errorMessage);  // エラー処理関数

int main(int argc, char *argv[])
{
    int sock;                       //ソケットディスククリプタ
    struct sockaddr_in echoServAddr;//エコーサーバのアドレス
    unsigned short echoServPort;    //エコーサーバのポート番号
    char *servIP;                   //サーバのIPアドレス(ドット10進表記)
    char *echoString;               //エコーサーバに送信する文字列
    char echoBuffer[RCVBUFSIZE];    //エコー文字列用のバッファ
    unsigned int echoStringLen;     //エコーする文字列のサイズ
    int bytesRcvd, totalBytesRcvd;  //1回のrecv()で読取られるバイト数と全バイト数

    // [変更] 引数の数が正しいか確認
    if ((argc < 2) || (argc > 3))
    {
        fprintf(stderr, "Usage: %s <Server IP> [<Echo Port>]\n", argv[0]);
        exit(1);
    }
    servIP = argv[1];          //1つめの引数:サーバのIPアドレス(ドット10進表記)
    // [削除] 2つめの引数:エコー文字列
    // [変更] 指定のポート番号があれば使用、なければポート7
    if (argc == 3)
        echoServPort = atoi(argv[2]);
    else
        echoServPort = 7;

    //TCPによる信頼性の高いストリームソケットを作成
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    //サーバのアドレス構造体を作成
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);
    //エコーサービスへの接続を確立
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");
    while (1) {
        //[追加] 文字列入力
        printf("CS>");
        scanf("%s", echoString);
        //[追加]終了判定
        if (strcmp(echoString, "quit") == 0)
        {
            break;
        }

        //入力データの長さを調べる
        echoStringLen = strlen(echoString);
        //文字列をサーバに送信
        if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
            DieWithError("sent() senta differentnumber of bytes than expected");
        //同じ文字列をサーバから受信
        totalBytesRcvd = 0;
        printf("Received: ");
        //エコーされた文字列を表示するための準備
        while (totalBytesRcvd < echoStringLen)
        {
            //バッファサイズに達するまでサーバからのデータを受信する
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
                DieWithError("recv() failed or connection closed prematurely");
            totalBytesRcvd += bytesRcvd;  //総バイト数
            echoBuffer[bytesRcvd] = '\0'; //文字列の終了
            printf(echoBuffer);           //エコーバッファの表示
        }
        printf("\n"); //最後の改行を出力
    }
    close(sock);
    exit(0);
}
