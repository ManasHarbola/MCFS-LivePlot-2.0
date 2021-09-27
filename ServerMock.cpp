#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "MessageStructs.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#define PORT 63475

char *startHeader = "STRT\0";
char *quitHeader = "QUIT\0";
char *avsenHeader = "AVSE\0";
char *propsenHeader = "PROP\0";

const int HEADER_LENGTH = 5;
//number of data points to plot before server sends "QUIT\0" string to plotter
int numTimes = 240;

/*
Example of a hypothetical server interfacing with MCFS LivePlot 2.0

Functions:
-Creates socket for communication with plotter
-Establishes connection with plotter with startup routine
-Sends sensor data over socket for user-specified amt of time (see numTimes)
-Sends "QUIT\0" to plotter to kill plotter process over socket
-Waits to accept connection from new plotter process (this is when you can change plotter settings in MCFS)
-Once plotter connects, performs startup routine again with new plotter instance and sends sensor data again
*/

int acceptNewClient(int server_fd, sockaddr_in address, int addrlen) {
    if (server_fd < 0) {
        std::cout << "server_fd invalid" << std::endl;
        return -1;
    }

    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        std::cout << "accept failed" << std::endl;
    }

    return new_socket;
}

//sensors to plot are hard coded for proof-of-concept
void startSequence(int client_fd) {
    //send STRT
    send(client_fd, (void*) startHeader, HEADER_LENGTH, 0);
    
    //prepare to send sensors
    std::vector<SENSOR> avsen_sensors{ACCEL_X_1, ACCEL_Y_1, ACCEL_X_2_DT};
    std::vector<SENSOR> propsen_sensors{PT01, TC01_DT};

    size_t avsen_sz = avsen_sensors.size();
    size_t propsen_sz = propsen_sensors.size();

    //send AVSEN size first
    send(client_fd, (void*) (&avsen_sz), sizeof(avsen_sz), 0);
    //send AVSEN sensors
    for (int i = 0; i < avsen_sz; i++) {
        send(client_fd, (void *)(&avsen_sensors[i]), sizeof(SENSOR), 0);
    }

    //send PROPSEN size first
    send(client_fd, (void*) (&propsen_sz), sizeof(propsen_sz), 0);
    //send PROPSEN sensors
    for (int i = 0; i < propsen_sz; i++) {
        send(client_fd, (void *)(&propsen_sensors[i]), sizeof(SENSOR), 0);
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating server file descriptor
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
    
    //Create to first client
    new_socket = acceptNewClient(server_fd, address, addrlen);

    if (new_socket < 0) {
        std::cout << "Invalid client fd" << std::endl;
        return -1;
    }

    //send over start sequence stuff
    startSequence(new_socket);
    
    std::cout << "Sequence sent" << std::endl;

    
    //duration is in microseconds here 
    int duration = 250000;

    PropsenDataMsg propsenMsg;
    AvsenDataMsg avsenMsg;

    int sz = sizeof(propsenMsg);

    avsenMsg.accel[0] = 1.996;
    avsenMsg.accel[1] = 0.666;
    avsenMsg.accel_ddt[3] = 0.888;
    propsenMsg.pt[0] = 2.002;
    propsenMsg.pt[1] = 0.75;
    propsenMsg.tc_ddt[0] = 0.0;
    avsenMsg.time_msec = 0;
    propsenMsg.time_msec = 0;
    
    /*
    auto milli_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    uint64_t milli_time = milli_since_epoch;
    std::cout << "chrono milliseconds: " << milli_since_epoch << std::endl;
    std::cout << "uint64_t milliseconds: " << milli_time << std::endl;
    */

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        send(new_socket, (void *) propsenHeader, HEADER_LENGTH, 0);
        send(new_socket, static_cast<void *>(&propsenMsg), sizeof(propsenMsg), 0);
        propsenMsg.pt[0] = ((double) rand() / (RAND_MAX)) * 8;
        propsenMsg.tc_ddt[0] = ((double) rand() / (RAND_MAX)) * 10;
        
        //propsenMsg.time_msec += (duration / 1000000.0);
        propsenMsg.time_msec += (uint64_t)(duration / 1000);
        //std::cout << propsenMsg.time_msec << std::endl;

        send(new_socket, (void *) avsenHeader, HEADER_LENGTH, 0);
        send(new_socket, static_cast<void *>(&avsenMsg), sizeof(avsenMsg), 0);
        avsenMsg.accel[0] = ((double) rand() / (RAND_MAX)) * 2;
        avsenMsg.accel[1] = ((double) rand() / (RAND_MAX)) * 2;
        avsenMsg.accel_ddt[3] = ((double) rand() / (RAND_MAX)) * 4;
        
        //avsenMsg.time_msec += (duration / 1000000.0);
        avsenMsg.time_msec += (uint64_t)(duration / 1000);

        std::this_thread::sleep_for(std::chrono::microseconds(duration));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta = end - start;

        std::cout << "time taken: " << delta.count() << "ms" << std::endl; 

        numTimes--;

        //test to restart plotter
        if (numTimes == 0) {
            numTimes = 100;
            avsenMsg.accel[0] = 1.996;
            avsenMsg.accel[1] = 0.666;
            avsenMsg.accel_ddt[3] = 0.888;
            propsenMsg.pt[0] = 2.002;
            propsenMsg.pt[1] = 0.75;
            propsenMsg.tc_ddt[0] = 0.0;

            avsenMsg.time_msec = 0;
            propsenMsg.time_msec = 0;

            //send quit string
            std::cout << "SENDING QUIT STRING" << std::endl;
            send(new_socket, (void *) quitHeader, HEADER_LENGTH, 0);

            //this call is blocking until new client connects
            new_socket = acceptNewClient(server_fd, address, addrlen);
            startSequence(new_socket);
        }
    }

    return 0;
}