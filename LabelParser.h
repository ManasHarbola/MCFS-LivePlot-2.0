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
    {ACCEL_X_1, {AVSEN, ACCEL_X_1, "ACCEL X (1)", "ACCEL X (1) reading"}}
};

std::unordered_map<SENSOR, PlotLabel> getPropsenLabels();

#endif  //LABEL_PARSER_H