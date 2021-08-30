#ifndef MESSAGESTRUCTS_H
#define MESSAGESTRUCTS_H
#include "Constants.h"
#include "Structs.h"

/*
struct SnapshotDataMsg {
    AvsenDataMsg avsenMsg;
    PropsenDataMsg propsenMsg;
	float time;

	//Possible flag states:
	//msgFlag == AVSEN_ONLY: only avsenMsg data has been sent over, propsenMsg is not valid
	//msgFlag == PROPSEN_ONLY: only propsenMsg data has been sent over, propsenMsg is not valid
	//msgFlag == ALL_MSG: both avsenMsg and propsenMsg have been sent over, both data are valid
	MessageType flag;
};
*/

struct SnapshotDataMsg {
	SnapshotDataMsg() {

	}
	union {
		AvsenDataMsg avsenMsg;
		PropsenDataMsg propsenMsg;
	};
	//float time{0.0};
	MessageType msgType{MessageType::UNKNOWN};
};

#endif