#include "Client.h"

using namespace std;


void Client::createSocket()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        cout << "Не удалось создать сокет!" << endl;
        exit(1);
    }
}

void Client::connectionToServer()
{
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

    if(connection == -1) {
        cout << "Не удалось установить соединение с сервером!" << endl;
        exit(1);
    }
}

void Client::closeSocket()
{
    close(socket_file_descriptor);
}

char* Client::readData()
{
    bzero(message, sizeof(message));
    ssize_t bytes = read(socket_file_descriptor, message, sizeof(message));

    return message;
}

void Client::writeData(string clientMessage)
{
    bzero(message, sizeof(message));
    ssize_t bytes = write(socket_file_descriptor, strcpy(message, clientMessage.c_str()), sizeof(message));
}
