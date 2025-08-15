#ifndef SCANNER_H
#define SCANNER_H

void identify_service(int sock, int port);
int scan(const std::string& ip, int start_port, int end_port);

#endif