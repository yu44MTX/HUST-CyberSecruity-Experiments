 //编译命令：g++ mytftp.cpp -o mytftp -lwsock32 -std=c++11; ./mytftp
#include <cstdio>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <winbase.h>
#include <ctime>
#include <iostream>

// #define LOCAL

#define MAX_FILENAME_LEN 100
#define LIMIT_T 30//超时时间
#define BUFLEN 1024//缓冲区大小
const char* MODE[3]={"\0", "netascii","octet"};//传输模式

// 记录日志：日志通常用于记录错误、成功等大的消息
// @param: msg：需要记录的消息；flag：是否输出，0否1是
void log(const char* msg){

    FILE* fp = fopen("tftplog.txt","a");
    // 当前时间
    time_t t;
    time(&t);
    char stime[100];
    strcpy(stime, ctime(&t));
    *(strchr(stime, '\n')) = '\0';
    fprintf(fp, "[ %s ] ", stime);

    fprintf(fp, msg);
    printf(msg);
    fclose(fp);
    return;
}

// 接收包，包含了重传机制
int RecvPkt(SOCKET clientSocket, char* sendBuf, int datalen, char* recvBuf, int& nrc, sockaddr_in addr,sockaddr_in& newaddr, int expnum){
    int len = sizeof(addr);
    char str[20];
    switch (sendBuf[1]){
        case 1: strcpy(str, "RRQ ACK");break;
        case 2: strcpy(str, "WRQ ACK");break;
        case 3: strcpy(str, "packet");break;
        case 4: strcpy(str, "packet ACK");break;
        default: memcpy(str, 0, 20);break;
    }
    struct timeval tv;
    fd_set readfds;//定义文件描述符集合
    FD_ZERO(&readfds);//清空文件描述符集合
    FD_SET(clientSocket, &readfds);//将套接字加入文件描述符集合
    tv.tv_sec = LIMIT_T;//设置超时时间
    for(int i = 1; i <= 10; i++){
        select(clientSocket, &readfds, NULL, NULL, &tv);//监听套接字
        if(FD_ISSET(clientSocket, &readfds)){//判断套接字是否可读
            if ((nrc = recvfrom(clientSocket, recvBuf, BUFLEN, 0, (struct sockaddr*)&newaddr, &len)) > 0){//接收数据
                if(((unsigned char)recvBuf[2] << 8) + (unsigned char)recvBuf[3]==expnum)//判断是否为期望的数据包
                    return 1;
            }
        }
        printf("Recieve %s time out: %d times\n", str, i);
        sendto(clientSocket, sendBuf, datalen, 0, (struct sockaddr*)&addr, len);//重发数据包
    }
    return 0;
}

// 获取socket
int getSocket(){
    WSADATA wsaData;
    SOCKET clientSocket;
    int nRC = WSAStartup(0x0101, &wsaData);
    if(nRC){
        printf("Client initialize winsock error!\n");
        return 0;
    }
    //创建 client socket
    clientSocket = socket(AF_INET,SOCK_DGRAM,0);//UDP
    if(clientSocket == INVALID_SOCKET){
        printf("Client create socket error!\n");
        WSACleanup();
        return 0;
    }
    unsigned long Opt = 1;
    ioctlsocket(clientSocket, FIONBIO, &Opt);//设置为非阻塞模式
    return clientSocket;
}

// ip和port
sockaddr_in getAddress(const char* ip, int port){
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    return addr;
}

// 下载文件
int Download(SOCKET clientSocket){

    int mode, datalen, res;
    unsigned short seq;
    char filename[MAX_FILENAME_LEN];
    char sendBuf[BUFLEN];
    char recvBuf[BUFLEN];
    char logBuf [BUFLEN];
    memset(sendBuf, 0, BUFLEN);
    memset(recvBuf, 0, BUFLEN);
    memset(logBuf , 0, BUFLEN);

    FILE* fp;
    clock_t start,end;
    start = clock();//开始计时

    sockaddr_in addr = getAddress("127.0.0.1", 69);
    sockaddr_in newaddr;

    printf("Please enter the FULL NAME of the file you want to DOWNLOAD\n");
    scanf("%s", filename);
    printf("Please choose the METHOD for downloading files: 1. netascii 2. octet\n");
    scanf("%d", &mode);

    if (mode == 1) fp = fopen(filename, "w");
    else           fp = fopen(filename, "wb");
    // RRQ
    int nrc;    // 收到的文件大小
    datalen = sprintf(sendBuf, "%c%c%s%c%s%c", '\0', 1, filename, '\0', MODE[mode], '\0');
    sprintf(logBuf, "send RRQ for file: %s\n", filename);
    log(logBuf);
    res = sendto(clientSocket, sendBuf, datalen, 0, (sockaddr*)&addr, sizeof(addr));
    res = RecvPkt(clientSocket, sendBuf, datalen, recvBuf, nrc, addr, newaddr, 1);
    if (res == 0){
        log("Recieve RRQ ACK time out!\n");
        return 0;
    }
    if (recvBuf[1] == 5){
        sprintf(logBuf, "Recieve RRQ ACK failed; Error code: %d; Error message: %s\n", recvBuf[3], recvBuf + 4);
        log(logBuf);
        return 0;
    }

    addr = newaddr;
    int filesize = 0;
    while(1){
        filesize += nrc - 4;
        seq = (((unsigned char)recvBuf[2] << 8) + (unsigned char)recvBuf[3]) % 65536;  // 数据包序号
        sprintf(sendBuf, "%c%c%c%c", '\0', 4, recvBuf[2], recvBuf[3]);
        fwrite(recvBuf + 4, 1, nrc - 4, fp);
        printf("Recieving packet %d\n", seq);
        sendto(clientSocket, sendBuf, 4, 0, (sockaddr*)&addr, sizeof(addr));
        if(nrc < 516) break;    // 最后一个包
        else{                   // 非最后一个包
            res = RecvPkt(clientSocket, sendBuf, datalen, recvBuf, nrc, addr, newaddr, seq + 1);
            if (res == 0){
                sprintf(logBuf, "Recieve packet %d time out!\n", seq);
                log(logBuf);
                return 0;
            }
            if (recvBuf[1] == 5){
                sprintf(logBuf, "Receive packet %d failed; error code: %d; Error message: %s\n", seq, recvBuf[3], recvBuf + 4);
                log(logBuf);
                return 0;
            }
        }
    }
    fclose(fp);
    end = clock();
    sprintf(logBuf, "SUCCESS receiving file:%s    mode:%s    speed:%lfKB/s\n", filename, MODE[mode], filesize /(((double)(end - start) / CLOCKS_PER_SEC)) / 1024);
    log(logBuf);
    return 1;
}

// 上传文件
int Upload(SOCKET clientSocket){

    int mode, datalen, res;
    unsigned short seq;
    char filename[MAX_FILENAME_LEN];    // 文件名
    char sendBuf[BUFLEN];               // 发送缓冲区
    char recvBuf[BUFLEN];               // 接收缓冲区
    char logBuf [BUFLEN];               // 日志缓冲区
    memset(sendBuf, 0, BUFLEN);
    memset(recvBuf, 0, BUFLEN);
    memset(logBuf , 0, BUFLEN);

    FILE* fp;
    clock_t start,end;                  // 用于记录上传文件所花时间
    start = clock();//开始计时

    sockaddr_in addr = getAddress("127.0.0.1", 69);    // 服务器ip和端口
    sockaddr_in newaddr;                            // 用于存储服务器返回的ip和端口

    printf("Please enter the FULL NAME of the file you want to UPLOAD\n");
    scanf("%s", filename);
    printf("Please choose the METHOD for uploading files: 1. netascii 2. octet\n");
    scanf("%d", &mode);

    if (mode == 1) fp = fopen(filename, "r");   // netascii
    else           fp = fopen(filename, "rb");  // octet
    if(fp == NULL){                             // 如果文件打开错误
        sprintf(logBuf, "Open file failed: %s\n", filename);
        log(logBuf);
        return 0;
    }
    // WRQ
    int nrc;
    datalen = sprintf(sendBuf, "%c%c%s%c%s%c", '\0', 2, filename, '\0', MODE[mode], '\0');  // 构造WRQ包
    sprintf(logBuf, "send WRQ for file: %s\n", filename);
    log(logBuf);    // 日志
    res = sendto(clientSocket, sendBuf, datalen, 0, (sockaddr*)&addr, sizeof(addr));        // 发送WRQ包
    res = RecvPkt(clientSocket, sendBuf, datalen, recvBuf, nrc, addr, newaddr, 0);          // 接收ACK
    if (res == 0){  // 超时
        log("Recieve WRQ ACK time out!\n");
        return 0;
    }
    if (recvBuf[1] == 5){   // 返回了Error包
        sprintf(logBuf, "Recieve WRQ ACK failed; Error code: %d; Error message: %s\n", recvBuf[3], recvBuf + 4);
        log(logBuf);
        return 0;
    }

    // 发送data
    addr = newaddr;
    int filesize = 0;
    while(1){
        seq = (((unsigned char)recvBuf[2] << 8) + (unsigned char)recvBuf[3] + 1) % 65536;   // 待发序号 = ACK序号 + 1
        sprintf(sendBuf, "%c%c%c%c", '\0', 3, seq >> 8, seq & 0xff);
        datalen = 4 + fread(sendBuf + 4, 1, 512, fp);                                       // 构造Data包
        filesize += datalen - 4;    // 存储文件大小
        printf("sending packet %d\n", seq);
        res = sendto(clientSocket, sendBuf, datalen, 0, (sockaddr*)&addr, sizeof(addr));    // 发送Data
        res = RecvPkt(clientSocket, sendBuf, datalen, recvBuf, nrc, addr, newaddr, seq);    // 接收ACK
        if (res == 0){  // 超时
            sprintf(logBuf, "Recieve ACK%d time out!\n", seq);
            log(logBuf);
            return 0;
        }
        if (recvBuf[1] == 5){   // 反回了Error包
            sprintf(logBuf, "Receive ACK%d failed; error code: %d; Error message: %s\n", seq, recvBuf[3], recvBuf + 4);
            log(logBuf);
            return 0;
        }
        if(datalen < 516) break;    // 如果数据发完了，退出
    }
    fclose(fp);         // 关闭文件
    end = clock();      // 记录结束时间，用于下面一行计算吞吐率
    sprintf(logBuf, "SUCCESS sending file:%s    mode:%s    speed:%lfKB/s\n", filename, MODE[mode], filesize /(((double)(end - start) / CLOCKS_PER_SEC)) / 1024);
    log(logBuf);
    return 1;
}


int main(void){
    int opt;
    FILE* f;
    #ifdef LOCAL
    freopen("input.txt", "r", stdin);
    #endif
    SOCKET clientSocket = getSocket();
    while(1){
        printf("+------------------------------+\n");
        printf("1. Upload            2. Download\n");
        printf("0. Close TFTP Client\n");
        printf("+------------------------------+\n");

        if(scanf("%d",&opt) <= 0) break;
        if     (opt == 1) Upload(clientSocket), printf("\n\n\n");
        else if(opt == 2) Download(clientSocket), printf("\n\n\n");
        else if(opt == 0) break;
        else continue;
    }
    closesocket(clientSocket);
    WSACleanup();
}
