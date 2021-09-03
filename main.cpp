#include "Constants.h"
#include "GraphWindow.h"
#include "GraphManager.h"
#include "MsgReceiver.h"
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>


int main(int argc, char **argv) {
    //create queue instance
    std::queue<SnapshotDataMsg> messageQueue;

    //create mutex instance
    std::mutex queueMutex;

    //update frequency for plotter gui, in hertz
    double freq = 4.0;

    //create MsgReceiver instance
    MsgReceiver receiver(messageQueue, queueMutex, 2 * freq);
    //open socket connection to server, receiver reads from socket 2 * freq hertz
    receiver.connectToServer("127.0.0.1", 8080);
    
    std::vector<std::vector<SENSOR>> plot_sensors = receiver.getRequestedSensors();

    Glib::set_application_name("MCFS LivePlot 2.0");
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("MCFS LivePlot 2.0");

    //create GraphWindow instance
    GraphWindow window(plot_sensors[0], plot_sensors[1]);
    
    //create GraphManager instance and assign GraphWindow object to it
    GraphManager manager(queueMutex, messageQueue, window, freq);

    //open thread for listening for new messages over socket
    std::thread receiveThread(&MsgReceiver::listenLoop, &receiver);
    receiveThread.detach();

    //open thread for listening for new sensor data to plot
    std::thread managerThread(&GraphManager::listen, &manager);
    managerThread.detach();

    //run window
    return app->run(window);
}