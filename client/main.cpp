#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <chrono>
#include <iomanip>
#include <algorithm>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            std::cout << "Disconnected from server." << std::endl;
            break;
        }
        buffer[bytes] = '\0';
        std::cout << "\r" << buffer << std::endl << "> " << std::flush;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    std::string nick;

    std::cout << "Enter your nickname: ";
    std::getline(std::cin, nick);
    if (nick.empty()) {
        std::cout << "Nickname cannot be empty." << std::endl;
        return 1;
    }

    if (sock < 0) {
        return 1;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address or address not supported\n";
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connect failed.\n";
        return 1;
    }

    std::cout << "Connected to chat server at " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    std::thread recv_thread(receive_messages, sock);
    std::string message;
    std::string out;
    std::string timestamp;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, message);
        if (message == "/quit") break;
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm *ltm = std::localtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(ltm, "%H:%M:%S");
        timestamp = oss.str();
        out = timestamp + " " +nick + ": " + message;
        send(sock, out.c_str(), out.length(), 0);
    }

    close(sock);
    recv_thread.detach();

    return 0;
}
