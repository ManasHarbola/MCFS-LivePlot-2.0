#include "LabelParser.h"
#include <cstdlib>
#include <fstream>
#include <vector>
//#include <iostream>

std::unordered_map<SENSOR, PlotLabel> getPropsenLabels() {
    std::unordered_map<SENSOR, PlotLabel> label_mappings;
    char *MCFS_INSTALL_ROOT = getenv("MCFS_INSTALL_ROOT");

    //return empty mapping if no env var was found
    if (MCFS_INSTALL_ROOT == nullptr) {
        return label_mappings;
    }

    std::string pt_path = std::string(MCFS_INSTALL_ROOT) + "/bin/env/PT_mappings.json";
    std::string tc_path = std::string(MCFS_INSTALL_ROOT) + "/bin/env/TC_mappings.json";

    Json::CharReaderBuilder pt_reader;
    Json::CharReaderBuilder tc_reader;
    Json::Value ptMapping;
    Json::Value tcMapping;

    pt_reader["collectComments"] = false;
    tc_reader["collectComments"] = false;
    std::ifstream pts(pt_path.c_str());
    std::ifstream tcs(tc_path.c_str());

    JSONCPP_STRING pt_errs;
    JSONCPP_STRING tc_errs;

    bool successfulPtRead = parseFromStream(pt_reader, pts, &ptMapping, &pt_errs);
    bool successfulTcRead = parseFromStream(tc_reader, tcs, &tcMapping, &tc_errs);

    const Json::Value& ind_pt_mappings = ptMapping["pts"];
    const Json::Value& ind_tc_mappings = tcMapping["tcs"];

    //add PT
    std::vector<SENSOR> pt_enums = {PT01, PT02, PT03, PT04, PT05,
                                    PT06, PT07, PT08, PT09, PT10,
                                    PT11, PT12, PT13, PT14, PT15,
                                    PT16};
    for (int i = 0; i < pt_enums.size(); i++) {
        label_mappings[pt_enums[i]] = {PROPSEN, pt_enums[i], ind_pt_mappings[i]["name"].asString(),
                            ind_pt_mappings[i]["name"].asString() + " reading"};
    }

    //add TC
    std::vector<SENSOR> tc_enums = {TC01, TC02, TC03, TC04, TC05,
                                    TC06, TC07, TC08, TC09, TC10,
                                    TC11, TC12, TC13, TC14, TC15,
                                    TC16};
    for (int i = 0; i < tc_enums.size(); i++) {
        label_mappings[tc_enums[i]] = {PROPSEN, tc_enums[i], ind_tc_mappings[i]["name"].asString(),
                            ind_tc_mappings[i]["name"].asString() + " reading"};
    }       

    //add Load Cell
    label_mappings[LC01] = {PROPSEN, LC01, "Load Cell", "Load Cell reading"};
    
    //generate derivative mappings based off pt, tc, and lc mappings
    //add PT derivatives
    std::vector<SENSOR> pt_ddt_enums = {PT01_DT, PT02_DT, PT03_DT, PT04_DT, PT05_DT,
                                        PT06_DT, PT07_DT, PT08_DT, PT09_DT, PT10_DT,
                                        PT11_DT, PT12_DT, PT13_DT, PT14_DT, PT15_DT,
                                        PT16_DT};
    //use pt_enums.size() to ensure that we're mapping only available PTs in json
    for (int i = 0; i < pt_enums.size(); i++) {
        label_mappings[pt_ddt_enums[i]] = {PROPSEN, pt_ddt_enums[i], ind_pt_mappings[i]["name"].asString() + " d/dt",
                                    ind_pt_mappings[i]["name"].asString() + " d/dt reading"};
    }

    //add TC derivatives
    std::vector<SENSOR> tc_ddt_enums = {TC01_DT, TC02_DT, TC03_DT, TC04_DT, TC05_DT,
                                        TC06_DT, TC07_DT, TC08_DT, TC09_DT, TC10_DT,
                                        TC11_DT, TC12_DT, TC13_DT, TC14_DT, TC15_DT,
                                        TC16_DT};
    //use tc_enums.size() to ensure that we're mapping only available TCs in json
    for (int i = 0; i < tc_enums.size(); i++) {
        label_mappings[tc_ddt_enums[i]] = {PROPSEN, tc_ddt_enums[i], ind_tc_mappings[i]["name"].asString() + " d/dt",
                                    ind_tc_mappings[i]["name"].asString() + " d/dt reading"};
    }       
    
    //add Load Cell derivative
    label_mappings[LC01_DT] = {PROPSEN, LC01_DT, "Load Cell d/dt", "Load Cell d/dt reading"};

    return label_mappings;
}