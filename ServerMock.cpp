#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "MessageStructs.h"
#include <iostream>
#include <thread>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    //SnapshotDataMsg snapshot;
    //snapshot.flag = MessageType::AVSEN_ONLY;
    //snapshot.avsenMsg.accel[0] = 5.0;
    //snapshot.avsenMsg.accel[1] = 4.5;
    //snapshot.avsenMsg.accel[2] = 4.0;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "socket failed" << std::endl;
        return -1;
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        std::cout << "setsockopt" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        std::cout << "bind failed" << std::endl;
        return -1;
    }
    if (listen(server_fd, 3) < 0)
    {
        std::cout << "listen failed" << std::endl;
        return -1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        std::cout << "accept failed" << std::endl;
    }

    //valread = read( new_socket , buffer, 1024);

    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n");

    int numTimes = 100;
    int duration = 250000;

    PropsenDataMsg propsenMsg;
    AvsenDataMsg avsenMsg;

    int sz = sizeof(propsenMsg);

    avsenMsg.accel[0] = 1.996;
    avsenMsg.accel[1] = 0.666;
    propsenMsg.pt[0] = 2.002;
    propsenMsg.pt[1] = 0.75;
    avsenMsg.time = 0.0;
    propsenMsg.time = 0.0;
    
    char *avsenHeader = "AVSE\0";
    char *propsenHeader = "PROP\0";

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        send(new_socket, (void *) propsenHeader, 5, 0);
        send(new_socket, static_cast<void *>(&propsenMsg), sizeof(propsenMsg), 0);
        propsenMsg.pt[0] = ((double) rand() / (RAND_MAX)) * 8;
        propsenMsg.tc[0] = ((double) rand() / (RAND_MAX)) * 10;
        propsenMsg.time += (duration / 1000000.0);

        send(new_socket, (void *) avsenHeader, 5, 0);
        send(new_socket, static_cast<void *>(&avsenMsg), sizeof(avsenMsg), 0);
        avsenMsg.accel[0] = ((double) rand() / (RAND_MAX)) * 2;
        avsenMsg.accel[1] = ((double) rand() / (RAND_MAX)) * 2;
        avsenMsg.time += (duration / 1000000.0);

        std::this_thread::sleep_for(std::chrono::microseconds(duration));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta = end - start;

        std::cout << "time taken: " << delta.count() << "ms" << std::endl; 
    }

    return 0;
}