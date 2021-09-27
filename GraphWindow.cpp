#include "GraphWindow.h"
#include "LabelParser.h"
#include <math.h>
#include <iostream>
GraphWindow::GraphWindow(std::vector<SENSOR>& selectedAvsen, std::vector<SENSOR>& selectedPropsen, int w, int h) :
canvas_()
{
    //set window dimensions
    set_default_size(w, h);
    //set window title
    set_title("MCFS LivePlot 2.0");

    int numPlots = selectedAvsen.size() + selectedPropsen.size();
    int numRows, numCols;

    //calculate grid dimensions
    if (numPlots == 1) {
        numRows = 1;
        numCols = 1;
    } else {
        numCols = (int) (log((double) numPlots) / log(2.0));
        numRows = (int) ceil(((double) numPlots) / numCols);
    }

    std::cout << "#rows: " << numRows << ", #cols: " << numCols << std::endl;

    //normalized factors for calculating the x and y positions of every graph
    double rowNormFactor = 1.0 / numPlots / numRows;
    double colNormFactor = 1.0 / numPlots / numCols;

    //double scalingFactor = 1.06;

    //normalized widths and heights of graphs
    double normalizedWidth = 1.0 / numCols;
    double normalizedHeight = 1.0 / numRows;
    //double normalizedHeight = 1.0 / numRows * scalingFactor;

    std::cout << "normW: " << normalizedWidth << ", normH: " << normalizedHeight << std::endl;

    double normXPos = 0.0, normYPos = 0.0;
    
    selectedPlots_[MessageType::AVSEN] = std::vector<std::pair<SENSOR, Gtk::PLplot::PlotData2D*>>();
    selectedPlots_[MessageType::PROPSEN] = std::vector<std::pair<SENSOR, Gtk::PLplot::PlotData2D*>>();

    std::unordered_map<SENSOR, PlotLabel> propsenLabels = getPropsenLabels();

    for (int i = 0; i < numPlots; i++) {
        xPtrs_.push_back(new std::valarray<double>());
        yPtrs_.push_back(new std::valarray<double>());

        auto plotDataPtr = new Gtk::PLplot::PlotData2D(*xPtrs_[i], *yPtrs_[i]);

        plotDatas_.push_back(plotDataPtr);

        //add mappings
        if (i < selectedAvsen.size()) {
            selectedPlots_[AVSEN].push_back({selectedAvsen[i], plotDataPtr});
        } else {
            selectedPlots_[PROPSEN].push_back({selectedPropsen[i - selectedAvsen.size()], plotDataPtr});
        }

        normXPos = normalizedWidth * (i % numCols);
        normYPos = normalizedHeight * (i / numCols);
        //normYPos = normalizedHeight/(scalingFactor * scalingFactor) * (i / numCols);

        //std::cout << "plot #" << i << ": xPos: " << normXPos << ", yPos: " << normYPos << std::endl;
        
        PlotLabel label;
        if (i < selectedAvsen.size()) {
            label = avsenLabels.at(selectedAvsen[i]);
        } else {
            label = propsenLabels.at(selectedPropsen[i - selectedAvsen.size()]);
        }

        plot2Ds_.push_back(new Gtk::PLplot::Plot2D(*plotDatas_[i], "Time (secs)", label.y_axis_title, 
                                                    label.plot_title, normalizedWidth, normalizedHeight,
                                                    //rowNormFactor * row, colNormFactor * col
                                                    normXPos, normYPos
                                                    )
                                                );
        
        plot2Ds_[i]->hide_legend();
        canvas_.add_plot(*plot2Ds_[i]);
    }
    
    canvas_.set_hexpand(true);
    canvas_.set_vexpand(true);

    add(canvas_);
    set_border_width(5);
    canvas_.show();
}