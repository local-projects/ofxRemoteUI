//
//  ofxRemoteUI.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 09/01/13.
//
//

#ifndef _ofxRemoteUIServer__
#define _ofxRemoteUIServer__

// you will need to add this to your "Header Search Path" for ofxOsc to compile
// ../../../addons/ofxOsc/libs ../../../addons/ofxOsc/libs/oscpack ../../../addons/ofxOsc/libs/oscpack/src ../../../addons/ofxOsc/libs/oscpack/src/ip ../../../addons/ofxOsc/libs/oscpack/src/ip/posix ../../../addons/ofxOsc/libs/oscpack/src/ip/win32 ../../../addons/ofxOsc/libs/oscpack/src/osc ../../../addons/ofxOsc/src
#include "ofxOsc.h"
#include "ofxRemoteUI.h"
#include "ofxXmlSettings.h"
#include <map>
#include <set>
#include <vector>

//easy param sharing macro, share from from anywhere!
#define OFX_REMOTEUI_SERVER_SHARE_PARAM(val,...)		( ofxRemoteUIServer::instance()->shareParam( #val, &val, ##__VA_ARGS__ ) )
#define OFX_REMOTEUI_SERVER_SHARE_ENUM_PARAM(val,enumMin,enumMax,menuList,...)	( ofxRemoteUIServer::instance()->shareParam( #val, (int*)&val,enumMin, enumMax,menuList, ##__VA_ARGS__ ) )
#define OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR(c)	( ofxRemoteUIServer::instance()->setParamColor( c ) )
#define OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP(g)	( ofxRemoteUIServer::instance()->setParamGroup( g ) )
#define OFX_REMOTEUI_SERVER_SETUP(port, ...)			( ofxRemoteUIServer::instance()->setup(port, ##__VA_ARGS__) )
#define OFX_REMOTEUI_SERVER_UPDATE(deltaTime)			( ofxRemoteUIServer::instance()->update(deltaTime) )
#define OFX_REMOTEUI_SERVER_CLOSE()						( ofxRemoteUIServer::instance()->close() )
#define	OFX_REMOTEUI_SERVER_SAVE_TO_XML()				( ofxRemoteUIServer::instance()->saveToXML(OFX_REMOTEUI_SETTINGS_FILENAME) )
#define	OFX_REMOTEUI_SERVER_LOAD_FROM_XML()				( ofxRemoteUIServer::instance()->loadFromXML(OFX_REMOTEUI_SETTINGS_FILENAME) )

class ofxRemoteUIServer: public ofxRemoteUI{ //this is injected into your app

public:

	static ofxRemoteUIServer* instance();

	void setup(int port = OFXREMOTEUI_PORT, float updateInterval = 0.1/*sec*/);
	void update(float dt);
	void close();
	void loadFromXML(string fileName);
	void saveToXML(string fileName);

	void shareParam(string paramName, float* param, float min, float max, ofColor bgColor = ofColor(0,0,0,0) );
	void shareParam(string paramName, bool* param, ofColor bgColor = ofColor(0,0,0,0), int nothing = 0 ); //"nothing" args are just to match other methods
	void shareParam(string paramName, int* param, int min, int max, ofColor bgColor = ofColor(0,0,0,0) );
	void shareParam(string paramName, string* param, ofColor bgColor = ofColor(0,0,0,0), int nothing = 0 ); //"nothing" args are just to match other methods
	void shareParam(string paramName, int* param, int min, int max, vector<string> names, ofColor c = ofColor(0,0,0,0));
	void setParamColor( ofColor c );
	void setParamGroup(string g);

private:

	void connect(string address, int port);

	ofxRemoteUIServer(); // use ofxRemoteUIServer::instance() instead!
	static ofxRemoteUIServer* singleton;
	void setColorForParam(RemoteUIParam &p, ofColor c);
	vector<string> getAvailablePresets();
	void deletePreset(string name);

	bool colorSet; //if user called setParamColor()
	ofColor paramColor;
	string upcomingGroup;


};
#endif
