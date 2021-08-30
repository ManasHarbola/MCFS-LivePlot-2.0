#include "Constants.h"
#include "GraphWindow.h"
#include "GraphManager.h"
#include "MsgReceiver.h"
#include "Base64Tools.h"
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

/*
int main(int argc, char **argv) {
    Glib::set_application_name("gtkmm-plplot-test11");
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test11");
    std::vector<SENSOR> v1, v2;
    GraphWindow window(v1, v2);

    return app->run(window);
}
*/

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Did not receive plotcode...stopping" << std::endl;
        return -1;
    }

    //create queue instance
    std::queue<SnapshotDataMsg> messageQueue;

    //create mutex instance
    std::mutex queueMutex;

    //select which sensors to plot
    std::string plotcode(argv[1]);

    //sensor_pair.first -> AVSEN, sensor_pair.second -> PROPSEN
    auto sensor_pair = plotCode_to_sensors(plotcode);

    /*
    std::vector<SENSOR> avsen_sensors{ACCEL_X_1, ACCEL_Y_1};
    std::vector<SENSOR> propsen_sensors{PT01, TC01};
    */

    Glib::set_application_name("MCFS LivePlot 2.0");
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("MCFS LivePlot 2.0");

    //create GraphWindow instance
    //GraphWindow window(avsen_sensors, propsen_sensors);
    GraphWindow window(sensor_pair[0], sensor_pair[1]);
    
    //update frequency for plotter gui
    double freq = 4.0;

    //create GraphManager instance and assign GraphWindow object to it
    GraphManager manager(queueMutex, messageQueue, window, freq);

    //create MsgReceiver instance
    MsgReceiver receiver(messageQueue, queueMutex, 2 * freq);

    std::thread receiveThread(&MsgReceiver::listen, &receiver);
    receiveThread.detach();

    std::thread managerThread(&GraphManager::listen, &manager);
    managerThread.detach();

    //create vector of enum SENSORS for avsen group using avsen int IDs
    //create vector of enum SENSORS for propsen group using propsen int IDs

    return app->run(window);
}