#pragma once
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>

using namespace std;

#define MESSAGE_LENGTH 1024
#define PORT 7777

class Server
{
public:
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int sockert_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];

    void createSocket();
    void bindSocket();
    void closeSocket();
    void dataReceivingServer();
    char* readData();
    void writeData(string serverMessage);    
};