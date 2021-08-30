#ifndef MSGRECEIVER_H
#define MSGRECEIVER_H

#include "MessageStructs.h"
#include <queue>
#include <mutex>

class MsgReceiver {
    public:
        MsgReceiver(std::queue<SnapshotDataMsg>& qRef, std::mutex& mutexRef, double sampleFreq=1.0);
        virtual ~MsgReceiver() {}
        int listen();

        static const size_t HEADER_SIZE{5};

    private:
        char headerBuffer_[HEADER_SIZE];
        bool recvHeaderNext_{true};
        SnapshotDataMsg snapBuffer_;

        std::queue<SnapshotDataMsg>& qRef_;
        std::mutex& mutexRef_;
        unsigned int sleepDuration_;

        
};

#endif