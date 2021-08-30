#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <unordered_map>
#include "Structs.h"

//1 second = 1000000 microseconds
const int ONE_SECOND_USECONDS = 1000000;

enum MessageType {
	UNKNOWN,
	AVSEN,
	PROPSEN
};

enum SENSOR {
    //Avsen sensors
    ACCEL_X_1, ACCEL_Y_1, ACCEL_Z_1,
    ACCEL_X_2, ACCEL_Y_2, ACCEL_Z_2,

    GYRO_X_1, GYRO_Y_1, GYRO_Z_1,
    GYRO_X_2, GYRO_Y_2, GYRO_Z_2,

    MAG_X, MAG_Y, MAG_Z,

    ALTITUDE, PRESSURE,

    GPS_LAT, GPS_LONG, GPS_ALT,

    ADXL_X, ADXL_Y, ADXL_Z,

    //Propsen sensors 
    PT01, PT02, PT03, PT04, 
    PT05, PT06, PT07, PT08, 
    PT09, PT10, PT11, PT12, 
    PT13, PT14, PT15, PT16, 
    
    TC01, TC02, TC03, TC04, 
    TC05, TC06, TC07, TC08, 
    TC09, TC10, TC11, TC12, 
    TC13, TC14, TC15, TC16, 

    LC01
};

const std::unordered_map<std::string, MessageType> headerToMessageType {
    {"AVSE\0", AVSEN},
    {"PROP\0", PROPSEN}
};
const std::unordered_map<MessageType, size_t> messageTypeToMessageSize {
    {AVSEN, sizeof(AvsenDataMsg)},
    {PROPSEN, sizeof(PropsenDataMsg)}
};

const std::unordered_map<int, SENSOR> bitflagToAVSEN = {
    {0 ,  ADXL_Z},
    {1 ,  ADXL_Y},
    {2 ,  ADXL_X},
    {3 ,  GPS_ALT},
    {4 ,  GPS_LONG},
    {5 ,  GPS_LAT},
    {6 ,  PRESSURE},
    {7 ,  ALTITUDE},
    {8 ,  MAG_Z },
    {9 ,  MAG_Y },
    {10 , MAG_X },
    {11 , GYRO_Z_2},
    {12 , GYRO_Y_2},
    {13 , GYRO_X_2},
    {14 , ACCEL_Z_2},
    {15 , ACCEL_Y_2},
    {16 , ACCEL_X_2},
    {17 , GYRO_Z_1},
    {18 , GYRO_Y_1},
    {19 , GYRO_X_1},
    {20 , ACCEL_Z_1},
    {21 , ACCEL_Y_1},
    {22 , ACCEL_X_1}
};

const std::unordered_map<int, SENSOR> bitflagToPROPSEN = {
    {23 , LC01},

    {24 , TC16},
    {25 , TC15},
    {26 , TC14},
    {27 , TC13},
    {28 , TC12},
    {29 , TC11},
    {30 , TC10},
    {31 , TC09},
    {32 , TC08},
    {33 , TC07},
    {34 , TC06},
    {35 , TC05},
    {36 , TC04},
    {37 , TC03},
    {38 , TC02},
    {39 , TC01},

    {40 , PT16},
    {41 , PT15},
    {42 , PT14},
    {43 , PT13},
    {44 , PT12},
    {45 , PT11},
    {46 , PT10},
    {47 , PT09},
    {48 , PT08},
    {49 , PT07},
    {50 , PT06},
    {51 , PT05},
    {52 , PT04},
    {53 , PT03},
    {54 , PT02},
    {55 , PT01},
};

struct PlotLabel {
    MessageType messageType;
    SENSOR sensor;
	std::string plot_title;
	std::string y_axis_title;
};

/*
struct sensor_type {
    union sensor {
        AVSEN_SENSOR avsen_sensor_type;
        PROPSEN_SENSOR propsen_sensor_type;
    };

    enum sensor_flag {
        AVSEN,
        PROPSEN,
        NONE
    } flag;
}
*/

#endif