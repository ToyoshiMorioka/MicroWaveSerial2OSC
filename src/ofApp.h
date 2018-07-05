#pragma once

#include "ofMain.h"
#include "ofSerial.h"
#include "ofxSMWSM.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

const string usbserial = "COM";

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxSMWSM _sensorModule;
		list<int> _realHeatRate;
		list<int> _realBreathRate;
		list<int> _heatRate;
		list<int> _breathRate;
		list<int> _bodyMove;

		void drawGraph(int x, int y);
		void drawReal(int x, int y);
		
		ofxOscSender sender;
		ofxXmlSettings xml;
};
