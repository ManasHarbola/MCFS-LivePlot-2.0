#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "Constants.h"
#include "MessageStructs.h"
#include "GraphWindow.h"
#include <unordered_map>
#include <queue>
#include <mutex>

class GraphManager {
    public:
        GraphManager(std::mutex& mLock, std::queue<SnapshotDataMsg>& qRef,
                     GraphWindow& window, double samplingFreq=1.0);
        virtual ~GraphManager() {}
        int listen();
        
        const std::unordered_map<SENSOR, double *> snapshotLookupMap {
            //AVSEN enums
            {ACCEL_X_1, &snapshot_.avsenMsg.accel[0]},  {ACCEL_Y_1, &snapshot_.avsenMsg.accel[1]},  
            {ACCEL_Z_1, &snapshot_.avsenMsg.accel[2]},  {ACCEL_X_2, &snapshot_.avsenMsg.accel[3]},
            {ACCEL_Y_2, &snapshot_.avsenMsg.accel[4]},  {ACCEL_Z_2, &snapshot_.avsenMsg.accel[5]},

            {GYRO_X_1, &snapshot_.avsenMsg.gyro[0]},    {GYRO_Y_1, &snapshot_.avsenMsg.gyro[1]},
            {GYRO_Z_1, &snapshot_.avsenMsg.gyro[2]},    {GYRO_X_2, &snapshot_.avsenMsg.gyro[3]},  
            {GYRO_Y_2, &snapshot_.avsenMsg.gyro[4]},    {GYRO_Z_2, &snapshot_.avsenMsg.gyro[5]},

            {MAG_X, &snapshot_.avsenMsg.mag[0]},        {MAG_Y, &snapshot_.avsenMsg.mag[1]},
            {MAG_Z, &snapshot_.avsenMsg.mag[2]},
            
            {ALTITUDE, &snapshot_.avsenMsg.alt},        {PRESSURE, &snapshot_.avsenMsg.press},
            
            {GPS_LAT, &snapshot_.avsenMsg.gps[0]},      {GPS_LONG, &snapshot_.avsenMsg.gps[1]},
            {GPS_ALT, &snapshot_.avsenMsg.gps[2]},

            {ADXL_X, &snapshot_.avsenMsg.adxl[0]},      {ADXL_Y, &snapshot_.avsenMsg.adxl[1]},
            {ADXL_Z, &snapshot_.avsenMsg.adxl[2]},

            //AVSEN derivative enums
            {ACCEL_X_1_DT, &snapshot_.avsenMsg.accel_ddt[0]},  {ACCEL_Y_1_DT, &snapshot_.avsenMsg.accel_ddt[1]},  
            {ACCEL_Z_1_DT, &snapshot_.avsenMsg.accel_ddt[2]},  {ACCEL_X_2_DT, &snapshot_.avsenMsg.accel_ddt[3]},
            {ACCEL_Y_2_DT, &snapshot_.avsenMsg.accel_ddt[4]},  {ACCEL_Z_2_DT, &snapshot_.avsenMsg.accel_ddt[5]},

            {GYRO_X_1_DT, &snapshot_.avsenMsg.gyro_ddt[0]},    {GYRO_Y_1_DT, &snapshot_.avsenMsg.gyro_ddt[1]},
            {GYRO_Z_1_DT, &snapshot_.avsenMsg.gyro_ddt[2]},    {GYRO_X_2_DT, &snapshot_.avsenMsg.gyro_ddt[3]},  
            {GYRO_Y_2_DT, &snapshot_.avsenMsg.gyro_ddt[4]},    {GYRO_Z_2_DT, &snapshot_.avsenMsg.gyro_ddt[5]},

            {MAG_X_DT, &snapshot_.avsenMsg.mag_ddt[0]},        {MAG_Y_DT, &snapshot_.avsenMsg.mag_ddt[1]},
            {MAG_Z_DT, &snapshot_.avsenMsg.mag_ddt[2]},
            
            {ALTITUDE_DT, &snapshot_.avsenMsg.alt_ddt},        {PRESSURE_DT, &snapshot_.avsenMsg.press_ddt},
            
            {GPS_LAT_DT, &snapshot_.avsenMsg.gps_ddt[0]},      {GPS_LONG_DT, &snapshot_.avsenMsg.gps_ddt[1]},
            {GPS_ALT_DT, &snapshot_.avsenMsg.gps_ddt[2]},

            {ADXL_X_DT, &snapshot_.avsenMsg.adxl_ddt[0]},      {ADXL_Y_DT, &snapshot_.avsenMsg.adxl_ddt[1]},
            {ADXL_Z_DT, &snapshot_.avsenMsg.adxl_ddt[2]},


            //PROPSEN enums
            {PT01, &snapshot_.propsenMsg.pt[0]},        {PT02, &snapshot_.propsenMsg.pt[1]},
            {PT03, &snapshot_.propsenMsg.pt[2]},        {PT04, &snapshot_.propsenMsg.pt[3]},
            {PT05, &snapshot_.propsenMsg.pt[4]},        {PT06, &snapshot_.propsenMsg.pt[5]},
            {PT07, &snapshot_.propsenMsg.pt[6]},        {PT08, &snapshot_.propsenMsg.pt[7]},
            {PT09, &snapshot_.propsenMsg.pt[8]},        {PT10, &snapshot_.propsenMsg.pt[9]},
            {PT11, &snapshot_.propsenMsg.pt[10]},       {PT12, &snapshot_.propsenMsg.pt[11]},
            {PT13, &snapshot_.propsenMsg.pt[12]},       {PT14, &snapshot_.propsenMsg.pt[13]},
            {PT15, &snapshot_.propsenMsg.pt[14]},       {PT16, &snapshot_.propsenMsg.pt[15]},
            
            {TC01, &snapshot_.propsenMsg.tc[0]},        {TC02, &snapshot_.propsenMsg.tc[1]},
            {TC03, &snapshot_.propsenMsg.tc[2]},        {TC04, &snapshot_.propsenMsg.tc[3]},
            {TC05, &snapshot_.propsenMsg.tc[4]},        {TC06, &snapshot_.propsenMsg.tc[5]},
            {TC07, &snapshot_.propsenMsg.tc[6]},        {TC08, &snapshot_.propsenMsg.tc[7]},
            {TC09, &snapshot_.propsenMsg.tc[8]},        {TC10, &snapshot_.propsenMsg.tc[9]},
            {TC11, &snapshot_.propsenMsg.tc[10]},       {TC12, &snapshot_.propsenMsg.tc[11]},
            {TC13, &snapshot_.propsenMsg.tc[12]},       {TC14, &snapshot_.propsenMsg.tc[13]},
            {TC15, &snapshot_.propsenMsg.tc[14]},       {TC16, &snapshot_.propsenMsg.tc[15]},

            {LC01, &snapshot_.propsenMsg.lc01},

            //PROPSEN derivative enums
            {PT01_DT, &snapshot_.propsenMsg.pt_ddt[0]},        {PT02_DT, &snapshot_.propsenMsg.pt_ddt[1]},
            {PT03_DT, &snapshot_.propsenMsg.pt_ddt[2]},        {PT04_DT, &snapshot_.propsenMsg.pt_ddt[3]},
            {PT05_DT, &snapshot_.propsenMsg.pt_ddt[4]},        {PT06_DT, &snapshot_.propsenMsg.pt_ddt[5]},
            {PT07_DT, &snapshot_.propsenMsg.pt_ddt[6]},        {PT08_DT, &snapshot_.propsenMsg.pt_ddt[7]},
            {PT09_DT, &snapshot_.propsenMsg.pt_ddt[8]},        {PT10_DT, &snapshot_.propsenMsg.pt_ddt[9]},
            {PT11_DT, &snapshot_.propsenMsg.pt_ddt[10]},       {PT12_DT, &snapshot_.propsenMsg.pt_ddt[11]},
            {PT13_DT, &snapshot_.propsenMsg.pt_ddt[12]},       {PT14_DT, &snapshot_.propsenMsg.pt_ddt[13]},
            {PT15_DT, &snapshot_.propsenMsg.pt_ddt[14]},       {PT16_DT, &snapshot_.propsenMsg.pt_ddt[15]},
            
            {TC01_DT, &snapshot_.propsenMsg.tc_ddt[0]},        {TC02_DT, &snapshot_.propsenMsg.tc_ddt[1]},
            {TC03_DT, &snapshot_.propsenMsg.tc_ddt[2]},        {TC04_DT, &snapshot_.propsenMsg.tc_ddt[3]},
            {TC05_DT, &snapshot_.propsenMsg.tc_ddt[4]},        {TC06_DT, &snapshot_.propsenMsg.tc_ddt[5]},
            {TC07_DT, &snapshot_.propsenMsg.tc_ddt[6]},        {TC08_DT, &snapshot_.propsenMsg.tc_ddt[7]},
            {TC09_DT, &snapshot_.propsenMsg.tc_ddt[8]},        {TC10_DT, &snapshot_.propsenMsg.tc_ddt[9]},
            {TC11_DT, &snapshot_.propsenMsg.tc_ddt[10]},       {TC12_DT, &snapshot_.propsenMsg.tc_ddt[11]},
            {TC13_DT, &snapshot_.propsenMsg.tc_ddt[12]},       {TC14_DT, &snapshot_.propsenMsg.tc_ddt[13]},
            {TC15_DT, &snapshot_.propsenMsg.tc_ddt[14]},       {TC16_DT, &snapshot_.propsenMsg.tc_ddt[15]},

            {LC01_DT, &snapshot_.propsenMsg.lc01_ddt}

        };
        
    private:
        std::mutex& mLock_;
        std::queue<SnapshotDataMsg>& qRef_;
        GraphWindow& managedWindow_;

        SnapshotDataMsg snapshot_;
        std::queue<SnapshotDataMsg> render_queue;

        int sleepDurationUseconds_;
        double samplingFreq_;
        
        int maxWindowPoints_;
};



#endif