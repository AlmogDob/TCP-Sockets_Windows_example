#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main(void)
{
    printf("----------TCP SERVER----------\n\n");

    WSADATA winsock_data;
    SOCKET TCP_server_socket;
    struct sockaddr_in TCP_server_addres;
    struct sockaddr_in TCP_client_addres;
    int TCP_client_addres_len = sizeof(TCP_client_addres);
    char sender_buffer[512] = "Hello from server!";
    int sender_buffer_len = sizeof(sender_buffer) / sizeof(sender_buffer[0]);
    char recive_buffer[512] = {0};
    int recive_buffer_len = sizeof(recive_buffer) / sizeof(recive_buffer[0]);

    if (WSAStartup(MAKEWORD(2,2), &winsock_data)) {
        fprintf(stderr, "WSAStartup faild\n");
        return 1;
    }

    TCP_server_addres.sin_family = AF_INET;
    if (InetPtonA(AF_INET, "127.0.0.1", &TCP_server_addres.sin_addr) != 1) {
        fprintf(stderr, "InetPtonA faild\n");
        return 1;
    }
    TCP_server_addres.sin_port = htons(42069);
    TCP_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (TCP_server_socket == INVALID_SOCKET) {
        fprintf(stderr, "TCP server socket creation faild. %d\n", WSAGetLastError());
        return 1;
    }
    printf("TCP server socket creation success\n");

    if (bind(TCP_server_socket, (SOCKADDR*)&TCP_server_addres, sizeof(TCP_server_addres)) == SOCKET_ERROR) {
        fprintf(stderr, "Binding faild. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Binding success\n");

    if (listen(TCP_server_socket, 2) == SOCKET_ERROR) {
        fprintf(stderr, "Listening faild. %d\n", WSAGetLastError());
        return 1;
    }
    printf("Listening success\n");

    for (;;) {
        SOCKET client = accept( TCP_server_socket, (SOCKADDR *)&TCP_client_addres, &TCP_client_addres_len);
        printf("Waiting for response form the client...\n");

        if (client == INVALID_SOCKET) {
            fprintf(stderr, "accept failed. %d\n", WSAGetLastError());
            break; // or break if you want to stop the server
        }

        // Handle client (blocking)
        send(client, sender_buffer, sender_buffer_len, 0);

        int n = recv(client, recive_buffer, recive_buffer_len - 1, 0);
        if (n > 0) {
            recive_buffer[n] = '\0';
            printf("Received: %s\n", recive_buffer);
        }

        closesocket(client);
    }

    // if (closesocket(TCP_server_socket) == SOCKET_ERROR) {
    //     fprintf(stderr, "Closing sever socket falid. %d\n", WSAGetLastError());
    //     return 1;
    // }
    // printf("Closing success\n");

    // if (WSACleanup() == SOCKET_ERROR) {
    //     fprintf(stderr, "Cleanup falid. %d\n", WSAGetLastError());
    //     return 1;
    // }
    // printf("Cleanup success\n");

    // printf("\nPRESS ANYTHING\n");
    // fgetc(stdin);

    return 0;
}