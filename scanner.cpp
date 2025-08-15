#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "scanner.h"


void identify_service(int sock, int port) {


    // Send protocol-specific requests to identify service version

    const char* http_request = "HEAD / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    const char* ftp_request = "SYST\r\n";


    char buffer[1024] = {0};
    std::string response;

    if (port == 80) {
        send(sock, http_request, strlen(http_request), 0);
        read(sock, buffer, sizeof(buffer) - 1);
        response = buffer;
        auto pos = response.find("Server: ");
        if (pos != std::string::npos) {

            std::cout << "HTTP service detected. ";
            std::stringstream server_line(response.substr(pos));
            std::string item;
            getline(server_line, item); // Get the full line of the server response
            std::cout << "Server info: " << item << std::endl;
        }
    } else if (port == 21) {

        read(sock, buffer, sizeof(buffer) - 1);
        send(sock, ftp_request, strlen(ftp_request), 0);
        read(sock, buffer, sizeof(buffer) - 1);
        response = buffer;
        std::cout << "FTP service detected. Response: " << response << std::endl;
    }
}


int scan(const std::string& ip, int start_port, int end_port) {

   
   
    int sock;

    struct sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(ip.c_str());


    for (int port = start_port; port <= end_port; ++port) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "Could not create socket" << std::endl;
            continue;
        }

        target.sin_port = htons(port);

        if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0) {

            std::cout << "Port " << port << " is open." << std::endl;
            identify_service(sock, port);
        } else {

            std::cout << "Port " << port << " is closed or filtered." << std::endl;
        }
        close(sock);
    }
    return 0;
}