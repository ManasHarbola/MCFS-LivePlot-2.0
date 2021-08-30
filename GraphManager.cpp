#include "Constants.h"
#include <unistd.h>
#include "GraphManager.h"
#include <chrono>
#include <thread>

GraphManager::GraphManager(std::mutex& mLock, std::queue<SnapshotDataMsg>& qRef,
                           GraphWindow& window, double samplingFreq) :
    mLock_(mLock), qRef_(qRef), managedWindow_(window) {
    //mLock_ = mLock;
    //qRef_ = qRef;
    //managedWindow_ = window;
    samplingFreq_ = samplingFreq;
    sleepDurationUseconds_ = (int) (ONE_SECOND_USECONDS / samplingFreq_);
    maxWindowPoints_ = (int) (60.0 * samplingFreq_);
}


int GraphManager::listen() {
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        bool newSnapReceived = false;

        int snapCount = 0;
        mLock_.lock();
        if (!qRef_.empty()) {
            newSnapReceived = true;
            /*
            snapshot_ = qRef_.front();
            qRef_.pop();
            */
            snapCount = qRef_.size();
            while (!qRef_.empty()) {
                render_queue.push(qRef_.front());
                qRef_.pop();
            }
        }
        mLock_.unlock();

        auto end2 = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> delta2 = end2 - start;

        std::cout << "added " << snapCount << " items to render_queue in " << delta2.count() << "ms\n";

        if (newSnapReceived) {
            while (!render_queue.empty()) {
                snapshot_ = render_queue.front();
                render_queue.pop();
                for (auto pair : managedWindow_.getSelectedPlots()[snapshot_.msgType]) {
                    auto plotData2DPtr = pair.second;
                    double newT;

                    switch(snapshot_.msgType) {
                        case AVSEN:
                            newT = (double) snapshot_.avsenMsg.time;
                            break;
                        case PROPSEN:
                            newT = (double) snapshot_.propsenMsg.time;
                            break;
                        default:
                            break;
                    }

                    //add data point, and remove oldest point if necessary
                    plotData2DPtr->add_datapoint(newT, *(snapshotLookupMap.at(pair.first)));
                    if (plotData2DPtr->size() > maxWindowPoints_) {
                        plotData2DPtr->remove_datapoint(0);
                    }
                }
            }
        }

        //usleep(sleepDurationUseconds_);
        std::this_thread::sleep_for(std::chrono::microseconds(sleepDurationUseconds_));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta = end - start;

        std::cout << "time taken to render: " << delta.count() << "ms" << std::endl; 
    }
}