#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main(void)
{
    printf("----------TCP CLIENT----------\n\n");

    WSADATA winsock_data;
    struct sockaddr_in TCP_server_addres;
    SOCKET TCP_client_socket;
    char sender_buffer[512] = "Hello from client!";
    int sender_buffer_len = sizeof(sender_buffer) / sizeof(sender_buffer[0]);
    char recive_buffer[512] = {0};
    int recive_buffer_len = sizeof(recive_buffer) / sizeof(recive_buffer[0]);

    if (WSAStartup(MAKEWORD(2,2), &winsock_data)) {
        fprintf(stderr, "WSAStartup faild\n");
        return 1;
    }

    TCP_client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (TCP_client_socket == INVALID_SOCKET) {
        fprintf(stderr, "TCP client socket creation faild. %d\n", WSAGetLastError());
        return 1;
    }
    printf("TCP client socket creation success\n");


    TCP_server_addres.sin_family = AF_INET;
    if (InetPtonA(AF_INET, "127.0.0.1", &TCP_server_addres.sin_addr) != 1) {
        fprintf(stderr, "InetPtonA faild\n");
        return 1;
    }
    TCP_server_addres.sin_port = htons(42069);

    if (connect(TCP_client_socket, (SOCKADDR*)&TCP_server_addres, sizeof(TCP_server_addres)) == SOCKET_ERROR) {
        fprintf(stderr, "Connection faild. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Connection success\n");

    if (recv(TCP_client_socket, recive_buffer, recive_buffer_len, 0) == SOCKET_ERROR) {
        fprintf(stderr, "Reciving falid. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Reciving success\nReceived:\n%s\n", recive_buffer);

    printf("\nPRESS ANYTHING TO SEND\n");
    fgetc(stdin);

    if (send(TCP_client_socket, sender_buffer, sender_buffer_len, 0) == SOCKET_ERROR) {
        fprintf(stderr, "Sending falid. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Sending success\n");


    if (closesocket(TCP_client_socket) == SOCKET_ERROR) {
        fprintf(stderr, "Closing sever socket falid. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Closing success\n");

    if (WSACleanup() == SOCKET_ERROR) {
        fprintf(stderr, "Cleanup falid. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Cleanup success\n");

    printf("\nPRESS ANYTHING\n");
    fgetc(stdin);

    return 0;
}