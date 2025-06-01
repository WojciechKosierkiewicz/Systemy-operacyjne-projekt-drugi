#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <fstream>

std::vector<int> clients;
std::mutex clients_mutex;

std::mutex history_mutex;
const std::string HISTORY_FILE = "./chat_history.txt";

void send_history(int client_socket) {
    std::ifstream infile(HISTORY_FILE);
    std::string line;
    while (std::getline(infile, line)) {
        std::string msg = line + "\n";
        send(client_socket, msg.c_str(), msg.size(), 0);
    }
}

void broadcast_message(const std::string &message, int sender_fd) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    std::string msg = message ;
    for (int client_fd : clients) {
        if (client_fd != sender_fd) {
            send(client_fd, msg.c_str(), msg.size(), 0);
        }
    }
}

void handle_client(int client_socket) {
    send_history(client_socket);

    char buffer[1024];
    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        std::string msg = buffer;

        std::cout << "Message from client: " << msg << std::endl;

        {
            std::lock_guard<std::mutex> lock(history_mutex);
            std::ofstream outfile(HISTORY_FILE, std::ios::app);
            if (outfile) {
                outfile << msg << std::endl;
            }
        }

        broadcast_message(msg, client_socket);
    }
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    close(client_socket);
}

int main() {
    int port;
    std::cout << "Enter the port number for the chat server: ";
    std::cin >> port;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Chat server started on port: " << port << std::endl;

    while (true) {
        socklen_t addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(new_socket);
        }
        std::thread(handle_client, new_socket).detach();
    }
}