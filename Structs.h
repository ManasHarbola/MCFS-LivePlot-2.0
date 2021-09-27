#ifndef STRUCTS_H
#define STRUCTS_H

struct AvsenDataMsg{
	//Indices 0,1,2 -> accelx1, accely1, accelz1
	//Indices 3,4,5 -> accelx2, accely2, accelz2 
	double accel[6] = {0};
	//Indices 0,1,2 -> gyrox1, gyroy1, gyroz1
	//Indices 3,4,5 -> gyrox2, gyroy2, gyroz2 
	double gyro[6] = {0};
	//Indices 0,1,2 -> magx, magy, magz
	double mag[3] = {0};
	//Indices 0,1,2,3,4,5,6,7 -> gpslat, gpslong, gpsalt, gpshdop, gpsvdop, gpspdop, gpsnumSats, gpstime
	double gps[8] = {0};
	//Altitude
	double alt = 0;
	//Pressure
	double press = 0;
	//board temp
	double temp = 0;
	//Indices 0,1,2 -> adxl_x, adxl_y, adxl_z
	double adxl[3] = {0};
	double accel_ddt[6] = {0};
	//Indices 0,1,2 -> gyrox1, gyroy1, gyroz1
	//Indices 3,4,5 -> gyrox2, gyroy2, gyroz2 
	double gyro_ddt[6] = {0};
	//Indices 0,1,2 -> magx, magy, magz
	double mag_ddt[3] = {0};
	//Indices 0,1,2,3,4,5,6,7 -> gpslat, gpslong, gpsalt, gpshdop, gpsvdop, gpspdop, gpsnumSats, gpstime
	double gps_ddt[8] = {0};
	//Altitude
	double alt_ddt = 0;
	//Pressure
	double press_ddt = 0;
	//board temp
	double temp_ddt = 0;
	//Indices 0,1,2 -> adxl_x, adxl_y, adxl_z
	double adxl_ddt[3] = {0};
	bool accel_persistance[6] = {false};
	bool gyro_persistance[6] = {false};
	bool mag_persistance[3] = {false};
	bool gps_persistance[8] = {false};
	bool alt_persistance = false;
	bool press_persistance = false;
	bool temp_persistance = false;
	bool adxl_persistance[3] = {false};
	bool test_running = false;// something for DIGISIM/SSHSIM
	
	uint64_t time_msec;
};

struct PropsenDataMsg{
	double tc[16] = {0};  //Indices 0-14 -> tc01-tc15
	double tc_ddt[16] = {0};
	double lc01 = 0;
	double lc01_ddt = 0;
	double boardTemp = 0;
	double boardTemp_ddt = 0;
	double pt[16] = {0}; //Indices 0-15 -> pt01-pt16
	double pt_ddt[16] = {0};
	bool tc_persistance[16] = {false};
	bool lc_persistance = false;
	bool boardTemp_persistance = false;
	bool pt_persistance[16] = {false};
	bool test_running = false;
	bool pt_calstate = false;
	bool tc_calstate = false;

	uint64_t time_msec;
};

#endif