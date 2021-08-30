#include "LabelParser.h"
#include <cstdlib>
#include <fstream>
#include <vector>

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
    const Json::Value& ind_tc_mappings = ptMapping["tcs"];

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

    return label_mappings;
}