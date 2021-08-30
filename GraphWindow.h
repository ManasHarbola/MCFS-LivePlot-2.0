#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

/*
#include <gtkmm-plplot.h>
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
 
#include <random>
*/

#include "gtkmm-plplot.h"
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <gtkmm/window.h>
#include <vector>
#include <utility>
#include <valarray>
#include <unistd.h> 
#include <thread>
#include "Constants.h"

class GraphWindow : public Gtk::Window {
    public:
        GraphWindow(std::vector<SENSOR>& selectedAvsen, std::vector<SENSOR>& selectedPropsen, int w = 1920, int h = 1080);
        virtual ~GraphWindow() {}
        std::vector<Gtk::PLplot::PlotData2D *>& getPlotDatas() {return plotDatas_;}
        std::unordered_map<MessageType, std::vector<std::pair<SENSOR, Gtk::PLplot::PlotData2D *>>>& getSelectedPlots() {return selectedPlots_;}

    private:
        std::vector<std::valarray<double> *> xPtrs_;
        std::vector<std::valarray<double> *> yPtrs_; 
        std::vector<Gtk::PLplot::PlotData2D *> plotDatas_;
        std::vector<Gtk::PLplot::Plot2D *> plot2Ds_;
        
        //maps sensor family (avsen or propsen) to vector of selected plots to optimize plotting
        std::unordered_map<MessageType, std::vector<std::pair<SENSOR, Gtk::PLplot::PlotData2D *>>> selectedPlots_;

        Gtk::PLplot::Canvas canvas_;
};

#endif