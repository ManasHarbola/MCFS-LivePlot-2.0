#ifndef MSGRECEIVER_H
#define MSGRECEIVER_H

#include "MessageStructs.h"
#include <queue>
#include <mutex>
#include <vector>
#include <string>

class MsgReceiver {
    public:
        MsgReceiver(std::queue<SnapshotDataMsg>& qRef, std::mutex& mutexRef, double sampleFreq=1.0);
        virtual ~MsgReceiver() {}
        bool connectToServer(std::string ip, int PORT);
        int listen();
        int listenLoop();
        std::vector<std::vector<SENSOR>> getRequestedSensors();
        static const size_t HEADER_SIZE{5};

    private:
        int sock_{0};
        char headerBuffer_[HEADER_SIZE];
        bool recvHeaderNext_{true};
        SnapshotDataMsg snapBuffer_;

        std::queue<SnapshotDataMsg>& qRef_;
        std::mutex& mutexRef_;
        unsigned int sleepDuration_;

        
};

#endif