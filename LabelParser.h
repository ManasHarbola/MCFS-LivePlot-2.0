#ifndef LABEL_PARSER_H
#define LABEL_PARSER_H

#include <unordered_map>
#include <vector>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "Constants.h"

//std::unordered_map<SENSOR, PlotLabel> getAvsenLabels(std::vector<SENSOR> avsenSensors);
const std::unordered_map<SENSOR, PlotLabel> avsenLabels = {
    {ADXL_Z,    {AVSEN, ADXL_Z, "ADXL Z", "ADXL Z reading"}},
    {ADXL_Y,    {AVSEN, ADXL_Y, "ADXL Y", "ADXL Y reading"}},
    {ADXL_X,    {AVSEN, ADXL_X, "ADXL X", "ADXL X reading"}},
    {GPS_ALT,   {AVSEN, GPS_ALT, "GPS ALTITUDE", "GPS Altitude reading"}},
    {GPS_LONG,  {AVSEN, GPS_LONG, "GPS LONGITUDE", "GPS Longitude reading"}},
    {GPS_LAT,   {AVSEN, GPS_LAT, "GPS LATITUDE", "GPS Latitude reading"}},
    {PRESSURE,  {AVSEN, PRESSURE, "PRESSURE", "PRESSURE reading"}},
    {ALTITUDE,  {AVSEN, ALTITUDE, "ALTITUDE", "ALTITUDE reading"}},
    {MAG_Z,     {AVSEN, MAG_Z, "MAG Z", "MAG Z reading"}},
    {MAG_Y,     {AVSEN, MAG_Y, "MAG Y", "MAG Y reading"}},
    {MAG_X,     {AVSEN, MAG_X, "MAG X", "MAG X reading"}},
    {GYRO_Z_2,  {AVSEN, GYRO_Z_2, "GYRO Z (2)", "GYRO Z (2) reading"}},
    {GYRO_Y_2,  {AVSEN, GYRO_Y_2, "GYRO Y (2)", "GYRO Y (2) reading"}},
    {GYRO_X_2,  {AVSEN, GYRO_X_2, "GYRO X (2)", "GYRO X (2) reading"}},
    {ACCEL_Z_2, {AVSEN, ACCEL_Z_2, "ACCEL Z (2)", "ACCEL Z (2) reading"}},
    {ACCEL_Y_2, {AVSEN, ACCEL_Y_2, "ACCEL Y (2)", "ACCEL Y (2) reading"}},
    {ACCEL_X_2, {AVSEN, ACCEL_X_2, "ACCEL X (2)", "ACCEL X (2) reading"}},
    {GYRO_Z_1,  {AVSEN, GYRO_Z_1, "GYRO Z (1)", "GYRO Z (1) reading"}},
    {GYRO_Y_1,  {AVSEN, GYRO_Y_1, "GYRO Y (1)", "GYRO Y (1) reading"}},
    {GYRO_X_1,  {AVSEN, GYRO_X_1, "GYRO X (1)", "GYRO X (1) reading"}},
    {ACCEL_Z_1, {AVSEN, ACCEL_Z_1, "ACCEL Z (1)", "ACCEL Z (1) reading"}},
    {ACCEL_Y_1, {AVSEN, ACCEL_Y_1, "ACCEL Y (1)", "ACCEL Y (1) reading"}},
    {ACCEL_X_1, {AVSEN, ACCEL_X_1, "ACCEL X (1)", "ACCEL X (1) reading"}},

    //derivatives entries
    {ADXL_Z_DT,    {AVSEN, ADXL_Z_DT, "ADXL Z d/dt", "ADXL Z d/dt reading"}},
    {ADXL_Y_DT,    {AVSEN, ADXL_Y_DT, "ADXL Y d/dt", "ADXL Y d/dt reading"}},
    {ADXL_X_DT,    {AVSEN, ADXL_X_DT, "ADXL X d/dt", "ADXL X d/dt reading"}},
    {GPS_ALT_DT,   {AVSEN, GPS_ALT_DT, "GPS ALTITUDE d/dt", "GPS Altitude d/dt reading"}},
    {GPS_LONG_DT,  {AVSEN, GPS_LONG_DT, "GPS LONGITUDE d/dt", "GPS Longitude d/dt reading"}},
    {GPS_LAT_DT,   {AVSEN, GPS_LAT_DT, "GPS LATITUDE d/dt", "GPS Latitude d/dt reading"}},
    {PRESSURE_DT,  {AVSEN, PRESSURE_DT, "PRESSURE d/dt", "PRESSURE d/dt reading"}},
    {ALTITUDE_DT,  {AVSEN, ALTITUDE_DT, "ALTITUDE d/dt", "ALTITUDE d/dt reading"}},
    {MAG_Z_DT,     {AVSEN, MAG_Z_DT, "MAG Z d/dt", "MAG Z d/dt reading"}},
    {MAG_Y_DT,     {AVSEN, MAG_Y_DT, "MAG Y d/dt", "MAG Y d/dt reading"}},
    {MAG_X_DT,     {AVSEN, MAG_X_DT, "MAG X d/dt", "MAG X d/dt reading"}},
    {GYRO_Z_2_DT,  {AVSEN, GYRO_Z_2_DT, "GYRO Z (2) d/dt", "GYRO Z (2) d/dt reading"}},
    {GYRO_Y_2_DT,  {AVSEN, GYRO_Y_2_DT, "GYRO Y (2) d/dt", "GYRO Y (2) d/dt reading"}},
    {GYRO_X_2_DT,  {AVSEN, GYRO_X_2_DT, "GYRO X (2) d/dt", "GYRO X (2) d/dt reading"}},
    {ACCEL_Z_2_DT, {AVSEN, ACCEL_Z_2_DT, "ACCEL Z (2) d/dt", "ACCEL Z (2) d/dt reading"}},
    {ACCEL_Y_2_DT, {AVSEN, ACCEL_Y_2_DT, "ACCEL Y (2) d/dt", "ACCEL Y (2) d/dt reading"}},
    {ACCEL_X_2_DT, {AVSEN, ACCEL_X_2_DT, "ACCEL X (2) d/dt", "ACCEL X (2) d/dt reading"}},
    {GYRO_Z_1_DT,  {AVSEN, GYRO_Z_1_DT, "GYRO Z (1) d/dt", "GYRO Z (1) d/dt reading"}},
    {GYRO_Y_1_DT,  {AVSEN, GYRO_Y_1_DT, "GYRO Y (1) d/dt", "GYRO Y (1) d/dt reading"}},
    {GYRO_X_1_DT,  {AVSEN, GYRO_X_1_DT, "GYRO X (1) d/dt", "GYRO X (1) d/dt reading"}},
    {ACCEL_Z_1_DT, {AVSEN, ACCEL_Z_1_DT, "ACCEL Z (1) d/dt", "ACCEL Z (1) d/dt reading"}},
    {ACCEL_Y_1_DT, {AVSEN, ACCEL_Y_1_DT, "ACCEL Y (1) d/dt", "ACCEL Y (1) d/dt reading"}},
    {ACCEL_X_1_DT, {AVSEN, ACCEL_X_1_DT, "ACCEL X (1) d/dt", "ACCEL X (1) d/dt reading"}},

};

std::unordered_map<SENSOR, PlotLabel> getPropsenLabels();

#endif  //LABEL_PARSER_H