//
//  ofxRemoteUI.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 09/01/13.
//
//

#ifndef _ofxRemoteUI__
#define _ofxRemoteUI__

// you will need to add this to your "Header Search Path" for ofxOsc to compile
// ../../../addons/ofxOsc/libs ../../../addons/ofxOsc/libs/oscpack ../../../addons/ofxOsc/libs/oscpack/src ../../../addons/ofxOsc/libs/oscpack/src/ip ../../../addons/ofxOsc/libs/oscpack/src/ip/posix ../../../addons/ofxOsc/libs/oscpack/src/ip/win32 ../../../addons/ofxOsc/libs/oscpack/src/osc ../../../addons/ofxOsc/src
#include "ofxOsc.h"
#include <map>
#include <set>
#include <vector>
using namespace std;

#define OFXREMOTEUI_PORT					10001
#define LATENCY_TEST_RATE					0.3333
#define CONNECTION_TIMEOUT					4.0f
#define OFX_REMOTEUI_SETTINGS_FILENAME		"ofxRemoteUISettings.xml"
#define OFX_REMOTEUI_XML_TAG				"OFX_REMOTE_UI_PARAMS"
#define DEFAULT_PARAM_GROUP					"defaultGroup"
#define OFX_REMOTEUI_PRESET_DIR				"ofxRemoteUIPresets"
#define OFX_REMOTEUI_NO_PRESETS				"NO_PRESETS"

/*

 // COMM /////////////////////////

 // init / setup connection /////
 CLIENT:	HELO								//client says HI
 SERVER:	HELO								//server says HI
 CLIENT:	REQU								//client requests all params from server
 SERVER:	SEND FLT PARAM_NAME val (float)		//server starts sending all vars one by one
 SERVER:	SEND INT PARAM_NAME2 val (int)
 SERVER:	SEND BOL PARAM_NAME3 val (bool)
 SERVER:	SEND STR PARAM_NAME4 val (string)
 SERVER:	REQU OK								//server closes REQU
 ...
 CLIENT:	PREL								//Preset List - client requests list of presets
 SERVER:	PREL PRESET_NAME_LIST(n)			//server sends all preset names
 ...

 // normal operation //////////
 CLIENT:	SEND TYP VAR_NAME val (varType)		//client sends a var change to server
 ...
 CLIENT:	TEST								//every second, client sends a msg to server to measure delay
 SERVER:	TEST								//server replies
 ...
 CLIENT:	SETP PRESET_NAME					//Set Preset - client wants to change all params according to preset "X"
 SERVER:	SETP OK								//server says ok
 CLIENT:	REQU								//client wants values for that preset
 SERVER:	SEND *****							//server sends all params
 SERVER:	REQU OK
 ...
 CLIENT:    SAVP PRESET_NAME					//Save Preset - client wants to save current params as a preset named PRESET_NAME
												//overwrites if already exists
 SERVER:	SAVP OK								//server replies OK
 CLIENT:	PREL								//Client requests full list of presets
 SERVER:	PREL PRESET_NAME_LIST(n)			//server sends all preset names
 ...
 CLIENT:	DELP PRESET_NAME					//client wants to delete preset named PRESET_NAME
 SERVER:	DELP OK								//server says ok
 CLIENT:	PREL								//Client requests full list of presets
 SERVER:	PREL PRESET_NAME_LIST(n)			//server sends all preset names

 // closing connection ////////
 CLIENT:	CIAO								//client disconnects
 SERVER:	CIAO								//server disconnects

 

 // SERVER API ////////////////////////////////////////

	server->setup(refreshRate);
	server->shareParam("paramName", &paramName, ... );
	...
	server->update(dt);

 // CLIENT API ////////////////////////////////////////
 
	client.setup(ipAddress, refreshRate);
	client.trackParam("paramName", &paramName);
	...
	client.update();


	float getMinThresholdForParam("paramMame"); //only applies to int and float
	float getMaxThresholdForParam("paramMame"); //only applies to int and float

	//get a report of params that changed on the server side since last check
	vector<string> updatedParamsList = client.getChangedParamsList();
 
	//push a param change to the server, will send the current value of the param to server
	client.sendTrackedParamUpdate("paramName");

 */

enum RemoteUICallBackArg{
	PARAMS_UPDATED, PRESETS_UPDATED, SERVER_DISCONNECTED
};

enum RemoteUIParamType{
	REMOTEUI_PARAM_FLOAT = 100,
	REMOTEUI_PARAM_INT,
	REMOTEUI_PARAM_BOOL,
	REMOTEUI_PARAM_STRING,
	REMOTEUI_PARAM_ENUM,
};

enum ActionType{
	HELO_ACTION, REQUEST_ACTION, SEND_PARAM_ACTION, CIAO_ACTION, TEST_ACTION, PRESET_LIST_ACTION,
	SET_PRESET_ACTION, SAVE_PRESET_ACTION, DELETE_PRESET_ACTION
};

enum ArgType{
	FLT_ARG, INT_ARG, BOL_ARG, STR_ARG, NULL_ARG, ENUM_ARG
};

struct DecodedMessage{
	ActionType action;
	ArgType argument;
	string paramName;
	string paramGroup;
};

class RemoteUIParam{ //I am lazy and I know it

public:

	RemoteUIParam(){
		floatValAddr = NULL;
		intValAddr = NULL;
		boolValAddr = NULL;
		stringValAddr = NULL;
		floatVal = minFloat = maxFloat = 0;
		intVal = minInt = maxInt = 0;
		boolVal = false;
		stringVal = "empty";
		r = g = b = a = 0;
		group = DEFAULT_PARAM_GROUP;
	};


	bool isEqualTo(RemoteUIParam &p){

		bool equal = true;
		switch (type) {
			case REMOTEUI_PARAM_FLOAT:
				if(p.floatVal != floatVal) equal = false;
				if(p.minFloat != minFloat) equal = false;
				if(p.maxFloat != maxFloat) equal = false;
				break;
			case REMOTEUI_PARAM_ENUM:
			case REMOTEUI_PARAM_INT:
				if(p.intVal != intVal) equal = false;
				if(p.minInt != minInt) equal = false;
				if(p.maxInt != maxInt) equal = false;
				break;
			case REMOTEUI_PARAM_BOOL:
				if(p.boolVal != boolVal) equal = false;
				break;
			case REMOTEUI_PARAM_STRING:
				if(p.stringVal != stringVal) equal = false;
				break;
			default: printf("weird RemoteUIParam at isEqualTo()!\n"); break;
		}
		return equal;
	}


	void print(){
		switch (type) {
			case REMOTEUI_PARAM_FLOAT: printf("float: %.2f [%.2f, %.2f]\n", floatVal, minFloat, maxFloat); break;
			case REMOTEUI_PARAM_INT: printf("int: %d [%d, %d]\n", intVal, minInt, maxInt); break;
			case REMOTEUI_PARAM_ENUM: printf("enum: %d [%d, %d]\n", intVal, minInt, maxInt); break;
			case REMOTEUI_PARAM_BOOL: printf("bool: %d\n", boolVal); break;
			case REMOTEUI_PARAM_STRING: printf("string: %s\n", stringVal.c_str()); break;
			default: printf("weird RemoteUIParam at print()!\n"); break;
		}
	};

	RemoteUIParamType type;

	float * floatValAddr;	//used in server
	float floatVal;			//used in client
	float minFloat;
	float maxFloat;

	int * intValAddr;
	int intVal;
	int minInt;
	int maxInt;

	bool * boolValAddr;
	bool boolVal;

	string * stringValAddr;
	string stringVal;
	string group;
	vector<string> enumList; //for enum type

	unsigned char r,g,b,a; // color [0,255]
};


class ofxRemoteUI{

public:

	vector<string> getAllParamNamesList();
	vector<string> getChangedParamsList(); //in user add order
	RemoteUIParam getParamForName(string paramName);
	vector<string> getPresetsList();
	
	string getValuesAsString();
	void setValuesFromString( string values );	

	bool ready();
	float connectionLag();

	virtual void sendUntrackedParamUpdate(RemoteUIParam p, string paramName){};

protected:

	virtual void update(float dt) = 0;
	void sendParam(string paramName, RemoteUIParam p);
	DecodedMessage decode(ofxOscMessage m);

	vector<string> scanForUpdatedParamsAndSync();

	void sendUpdateForParamsInList(vector<string>paramsPendingUpdate);
	bool hasParamChanged(RemoteUIParam p);	

	void updateParamFromDecodedMessage(ofxOscMessage m, DecodedMessage dm);
	void syncParamToPointer(string paramName);
	void addParamToDB(RemoteUIParam p, string paramName);

	void sendREQU(bool confirm = false); //a request for a complete list of server params
	void sendHELLO();
	void sendCIAO();
	void sendTEST();
	void sendPREL(vector<string> presetNames);
	void sendSAVP(string presetName);
	void sendSETP(string presetName);
	void sendDELP(string presetName);

	bool							readyToSend;
	ofxOscSender					oscSender;
	ofxOscReceiver					oscReceiver;

	float							time;
	float							timeSinceLastReply;
	float							avgTimeSinceLastReply;
	bool							waitingForReply;

	float							updateInterval;
	int								port;

	map<string, RemoteUIParam>		params;
	map<int, string>				orderedKeys; // used to keep the order in which the params were added
	vector<string>					presetNames;

	set<string>						paramsChangedSinceLastCheck;

private:

	string stringForParamType(RemoteUIParamType t);

};

#endif
