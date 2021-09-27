#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <iostream>
#include "Constants.h"
#include "MessageStructs.h"
#include "MsgReceiver.h"

void ReadReadMessage(int sock, void* buffer, unsigned int x){
	int bytesRead = 0;
	int result;
	//cout << "[SIZE2READ]: " << x;
	while (bytesRead < x) {
		result = read(sock, buffer + bytesRead, x - bytesRead);
		if (result < 1) {
			//oops something went wrong
			return;
		}
		bytesRead += result;
		//cout << " " << buffer;
	}
	//cout << '\n';
}

MsgReceiver::MsgReceiver(std::queue<SnapshotDataMsg>& qRef,
std::mutex& mutexRef, double sampleFreq) : qRef_(qRef), mutexRef_(mutexRef) {
    sleepDuration_ = (int) (ONE_SECOND_USECONDS / sampleFreq);
}

//returns true if connection succeeded
bool MsgReceiver::connectToServer(std::string ip, int PORT) {
    sockaddr_in serv_addr;

    if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        //return -1;
        return false;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0) 
    {
        //return -1;
        return false;
    }

    // Loop until connected
    while (connect(sock_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) ;
    std::cout << "Connected!" << std::endl;
    return true;
}

std::vector<std::vector<SENSOR>> MsgReceiver::getRequestedSensors() {
    //avsen is sensors[0], propsen is sensors[1]
    std::vector<std::vector<SENSOR>> sensors;
    sensors.push_back(std::vector<SENSOR>());
    sensors.push_back(std::vector<SENSOR>());

    if (sock_ <= 0) {
        return sensors;
    }

    std::cout << "[PLOT] Sensor Setup: " << std::endl;
    //check to see if STRT was received first
    ReadReadMessage( sock_, &headerBuffer_, HEADER_SIZE);
    std::cout << headerBuffer_ << std::endl;

    /*
    ReadReadMessage( sock_, &headerBuffer_, HEADER_SIZE);
    std::cout << headerBuffer_ << std::endl;
    */

    if (std::string(headerBuffer_) == "STRT") {
        std::cout << "[PLOT]   Received STRT" << std::endl;
        uint64_t arr_sz;
        SENSOR currSensor;

        //get size of AVSEN array
        ReadReadMessage(sock_, &arr_sz, sizeof(arr_sz));
        std::cout << "[PLOT]   AVSEN: " << arr_sz << std::endl;
        for (size_t i = 0; i < arr_sz; i++) {
            ReadReadMessage(sock_, &currSensor, sizeof(SENSOR));
            
            sensors[0].push_back(currSensor);
        }

        //get size of PROPSEN array
        ReadReadMessage(sock_, &arr_sz, sizeof(arr_sz));
        
        std::cout << "[PLOT]   PROPSEN: " << arr_sz << std::endl;
        for (size_t i = 0; i < arr_sz; i++) {
            ReadReadMessage(sock_, static_cast<void*>(&currSensor), sizeof(SENSOR));
            
            sensors[1].push_back(currSensor);
        }
    }

    return sensors;
}

int MsgReceiver::listenLoop() {
    //return if sock_ is not proper fd value
    if (sock_ < 0) {
        return -1;
    }

    bool firstMsgReceived = false;
    uint64_t epoch_msec;

    //inside listenLoop(), headerBuffer_ can be "AVSE\0", "PROP\0", or "QUIT\0"
    recvHeaderNext_ = true;
    while (true) {
        //auto start = std::chrono::high_resolution_clock::now();
        //perform socket read -> this is blocking info
        //receive header
        if (recvHeaderNext_) {
            ReadReadMessage(sock_, static_cast<void *>(&headerBuffer_), HEADER_SIZE);
            std::cout << "[PLOT] Received " << headerBuffer_ << std::endl;
        } else {    //receive message
            try {
                //valread = recv(sock_, static_cast<void *>(&snapBuffer_), messageTypeToMessageSize.at(snapBuffer_.msgType), 0);
                switch (snapBuffer_.msgType)
                {
                case AVSEN:
                    ReadReadMessage(sock_, static_cast<void *>(&snapBuffer_.avsenMsg), messageTypeToMessageSize.at(snapBuffer_.msgType));
                    if (!firstMsgReceived) {
                        epoch_msec = snapBuffer_.avsenMsg.time_msec;
                        //std::cout << "HEY THIS IS EPOCH: " << epoch_msec << std::endl;
                        firstMsgReceived = true;
                    }
                    break;
                case PROPSEN:
                    ReadReadMessage(sock_, static_cast<void *>(&snapBuffer_.propsenMsg), messageTypeToMessageSize.at(snapBuffer_.msgType));
                    if (!firstMsgReceived) {
                        epoch_msec = snapBuffer_.propsenMsg.time_msec;
                        //std::cout << "HEY THIS IS EPOCH: " << epoch_msec << std::endl;
                        firstMsgReceived = true;
                    }
                    break;
                default:
                    break;
                }
                //
            } catch (std::out_of_range& exception) {
                std::cout << "Improper message size, terminating receiver..." << std::endl;
                std::cout << exception.what() << std::endl;
                return -1;
            }
        }

        //handle shutdown of plotter
        //must ensure that server doesn't shut down
        if (recvHeaderNext_ && std::string(headerBuffer_) == "QUIT\0") {
            std::cout << "Received QUIT header, terminating plotter..." << std::endl;

            //signal to GraphManager to stop
            mutexRef_.lock();
            snapBuffer_.msgType = QUIT;
            qRef_.push(snapBuffer_);
            mutexRef_.unlock();

            // Close socket to GUI
            close(sock_);

            return 0;
        }

        if (recvHeaderNext_) {
            //std::cout << "Received header: " << headerBuffer_ << std::endl;
            //try-catch guarantees that we receive a valid header message
            try {
                snapBuffer_.msgType = headerToMessageType.at(std::string(headerBuffer_));
            } catch (const std::out_of_range& exception) {
                std::cout << "Improper header value: \"" << std::string(headerBuffer_) 
                          << "\", terminating receiver..." << std::endl;
                std::cout << exception.what() << std::endl;
                return -1;
            }
        } else {
            //package timestamp
            switch (snapBuffer_.msgType) {
            case AVSEN:
                snapBuffer_.avsenMsg.time_msec -= epoch_msec;
                //std::cout << "time_msec in avsenMsg: " << snapBuffer_.avsenMsg.time_msec << std::endl;
                break;
            case PROPSEN:
                snapBuffer_.propsenMsg.time_msec -= epoch_msec;
                //std::cout << "time_msec in propsenMsg: " << snapBuffer_.propsenMsg.time_msec << std::endl;
                break;
            default:
                break;
            }
            
            mutexRef_.lock();
            qRef_.push(snapBuffer_);
            mutexRef_.unlock();
        }

        //flip reception state
        recvHeaderNext_ = !recvHeaderNext_;
        /*
        We use sleepDuration_ / 2 because we're constantly switching
        between receiving the header or the message, which in total should equal
        the sampling frequency.
        */
        //usleep(sleepDuration_ / 2);
        std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration_ / 2));

        /*
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> delta = end - start;

        std::cout << "time taken by receiver: " << delta.count() << "ms\n";
        */
    }
}


int MsgReceiver::listen() {
    int PORT = 8080;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return -1;
    }
    
    std::cout << "Connected!" << std::endl;

    while (true) {
        //auto start = std::chrono::high_resolution_clock::now();

        //perform socket read -> this is blocking info
        //receive header
        if (recvHeaderNext_) {
            ReadReadMessage( sock, static_cast<void *>(&headerBuffer_), HEADER_SIZE);

        } else {    //receive message
            try {
                ReadReadMessage( sock, static_cast<void *>(&snapBuffer_), messageTypeToMessageSize.at(snapBuffer_.msgType));
            } catch (std::out_of_range& exception) {
                std::cout << "Improper message size, terminating receiver..." << std::endl;
                std::cout << exception.what() << std::endl;
                return -1;
            }
        }

        if (valread == -1) {
            std::cout << "An error occurred\n, terminating receiver..." << std::endl;
            return -1;
        } else if (valread == 0) {
            std::cout << "Server socket has shutdown, terminating receiver..." << std::endl;
            return 0;   //we consider this a graceful exit
        }

        if (recvHeaderNext_) {
            //std::cout << "Received header: " << headerBuffer_ << std::endl;
            //try-catch guarantees that we receive a valid header message
            try {
                snapBuffer_.msgType = headerToMessageType.at(std::string(headerBuffer_));
            } catch (const std::out_of_range& exception) {
                std::cout << "Improper header value: \"" << std::string(headerBuffer_) 
                          << "\", terminating receiver..." << std::endl;
                std::cout << exception.what() << std::endl;
                return -1;
            }
        } else {
            /*
            if (snapBuffer_.msgType == MessageType::AVSEN) {
                std::cout << snapBuffer_.avsenMsg.accel[0] << std::endl;
            } else {
                std::cout << snapBuffer_.propsenMsg.pt[0] << std::endl;
            }
            */
            mutexRef_.lock();
            qRef_.push(snapBuffer_);
            mutexRef_.unlock();
        }

        //flip reception state
        recvHeaderNext_ = !recvHeaderNext_;
        /*
        We use sleepDuration_ / 2 because we're constantly switching
        between receiving the header or the message, which in total should equal
        the sampling frequency.
        */
        //usleep(sleepDuration_ / 2);
        std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration_ / 2));

        /*
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> delta = end - start;

        std::cout << "time taken by receiver: " << delta.count() << "ms\n";
        */
    }

    return 0;
}

/*
int main() {
    std::queue<SnapshotDataMsg> queue;
    std::mutex mutex;
    double freq = 1.0;

    MsgReceiver receiver(queue, mutex, freq);

    return receiver.listen();
}
*/

