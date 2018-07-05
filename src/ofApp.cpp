#include "ofApp.h"
const int listSize = 100;
const int margin = 10;

void ofApp::drawGraph(int x, int y) {
	int graphWidth = ofGetWidth() - margin * 2;
	int graphHeight = 100;
	int offsetX = x + margin;
	int offsetY = y + margin;

	ofSetColor(64, 64, 64);
	ofDrawRectangle(offsetX, offsetY, graphWidth, graphHeight);

	list<int>::iterator rhr = _heatRate.begin(); // イテレータ
	list<int>::iterator rbr = _breathRate.begin(); // イテレータ
	list<int>::iterator rbm = _bodyMove.begin(); // イテレータ

	int pos = 0;

	ofPoint start1;
	ofPoint start2;
	ofPoint start3;

	start1.x = offsetX + pos * graphWidth / _heatRate.size();
	start1.y = offsetY - (*rhr - 32678) * graphHeight / 65535;
	start2.x = offsetX + pos * graphWidth / _breathRate.size();
	start2.y = offsetY - (*rbr - 32678) * graphHeight / 65535;
	start3.x = offsetX + pos * graphWidth / _bodyMove.size();
	start3.y = offsetY - (*rbm - 32678) * graphHeight / 65535;

	++rhr;  // イテレータを１つ進める
	++rbr;
	++rbm;

	ofPoint end1;
	ofPoint end2;
	ofPoint end3;


	end1.x = offsetX + (pos + 1) * graphWidth / _heatRate.size();
	end1.y = offsetY - (*rhr - 32678) * graphHeight / 65535;
	end2.x = offsetX + (pos + 1) * graphWidth / _breathRate.size();
	end2.y = offsetY - (*rbr - 32678) * graphHeight / 65535;
	end3.x = offsetX + (pos + 1) * graphWidth / _bodyMove.size();
	end3.y = offsetY - (*rbm - 32678) * graphHeight / 65535;

	ofSetColor(255, 0, 0);
	ofDrawLine(start1.x, start1.y, end1.x, end1.y);
	ofSetColor(0, 255, 0);
	ofDrawLine(start2.x, start2.y, end2.x, end2.y);
	ofSetColor(0, 0, 255);
	ofDrawLine(start3.x, start3.y, end3.x, end3.y);

	++rhr;
	++rbr;
	++rbm;

	while (rhr != _heatRate.end())  // listの末尾まで
	{
		start1 = end1;
		start2 = end2;
		start3 = end3;

		end1.x = offsetX + (pos + 1) * graphWidth / _heatRate.size();
		end1.y = offsetY - (*rhr - 32678) * graphHeight / 65535;
		end2.x = offsetX + (pos + 1) * graphWidth / _breathRate.size();
		end2.y = offsetY - (*rbr - 32678) * graphHeight / 65535;
		end3.x = offsetX + (pos + 1) * graphWidth / _bodyMove.size();
		end3.y = offsetY - (*rbm - 32678) * graphHeight / 65535;

		pos++;

		ofSetColor(255, 0, 0);
		ofDrawLine(start1.x, start1.y, end1.x, end1.y);
		ofSetColor(0, 255, 0);
		ofDrawLine(start2.x, start2.y, end2.x, end2.y);
		ofSetColor(0, 0, 255);
		ofDrawLine(start3.x, start3.y, end3.x, end3.y);

		++rhr;
		++rbr;
		++rbm;
	}
}

void ofApp::drawReal(int x, int y) {
	int graphWidth = ofGetWidth() - margin * 2;
	int graphHeight = 100;
	int offsetX = x + margin;
	int offsetY = y + margin;

	ofSetColor(64, 64, 64);
	ofDrawRectangle(offsetX, offsetY, graphWidth, graphHeight);

	list<int>::iterator rhr = _realHeatRate.begin(); // イテレータ
	list<int>::iterator rbr = _realBreathRate.begin(); // イテレータ

	int pos = 0;

	ofPoint start1;
	ofPoint start2;

	start1.x = offsetX + pos * graphWidth / _realHeatRate.size();
	start1.y = offsetY + (200 - *rhr) * graphHeight / 200;
	start2.x = offsetX + pos * graphWidth / _realBreathRate.size();
	start2.y = offsetY + (50 - *rbr) * graphHeight / 50;

	++rhr;  // イテレータを１つ進める
	++rbr;

	ofPoint end1;
	ofPoint end2;

	end1.x = offsetX + (pos + 1) * graphWidth / _realHeatRate.size();
	end1.y = offsetY + (200 - *rhr) * graphHeight / 200;
	end2.x = offsetX + (pos + 1) * graphWidth / _realBreathRate.size();
	end2.y = offsetY + (50 - *rbr) * graphHeight / 50;

	ofSetColor(255, 0, 0);
	ofDrawLine(start1.x, start1.y, end1.x, end1.y);
	ofSetColor(0, 255, 0);
	ofDrawLine(start2.x, start2.y, end2.x, end2.y);

	++rhr;
	++rbr;

	while (rhr != _realHeatRate.end())  // listの末尾まで
	{
		start1 = end1;
		start2 = end2;

		end1.x = offsetX + (pos + 1) * graphWidth / _realHeatRate.size();
		end1.y = offsetY + (200 - *rhr) * graphHeight / 200;
		end2.x = offsetX + (pos + 1) * graphWidth / _realBreathRate.size();
		end2.y = offsetY + (50 - *rbr) * graphHeight / 50;

		pos++;

		ofSetColor(255, 0, 0);
		ofDrawLine(start1.x, start1.y, end1.x, end1.y);
		ofSetColor(0, 255, 0);
		ofDrawLine(start2.x, start2.y, end2.x, end2.y);

		++rhr;
		++rbr;
	}
}
//--------------------------------------------------------------
//string serialPort = "COM3";
//string host = "localhost";
//int portNum = 7000;

void ofApp::setup(){
	for (int i = 0; i < listSize; i++) {
		_realHeatRate.push_back(0);
		_realBreathRate.push_back(0);
		_heatRate.push_back(0);
		_breathRate.push_back(0);
		_bodyMove.push_back(0);
	}
	// xml
	xml.loadFile("mySettings.xml");
	

	// シリアル接続
	_sensorModule.setup(xml.getValue("serialportnum", "COM1"));
	sender.setup(xml.getValue("oscip", "localhost") , xml.getValue("oscport", 6000));

}

//--------------------------------------------------------------
void ofApp::update(){
	_sensorModule.update();

	_heatRate.push_back(_sensorModule.graphHeartRate);
	_breathRate.push_back(_sensorModule.graphBreathRate);
	_bodyMove.push_back(_sensorModule.graphMoveRate);
	_realHeatRate.push_back(_sensorModule.heartRate);
	_realBreathRate.push_back(_sensorModule.breathRate);

	_heatRate.pop_front();
	_breathRate.pop_front();
	_bodyMove.pop_front();
	_realHeatRate.pop_front();
	_realBreathRate.pop_front();

	

	// osc
	ofxOscMessage h;
	h.setAddress("/heartrate");
	h.addIntArg(_sensorModule.heartRate);
	h.addIntArg(_sensorModule.heartRateAccuracy);
	sender.sendMessage(h);

	ofxOscMessage b;
	b.setAddress("/breathrate");
	b.addIntArg(_sensorModule.breathRate);
	b.addIntArg(_sensorModule.breathRateAccuracy);
	sender.sendMessage(b);

	ofxOscMessage hd;
	hd.setAddress("/heartratedata");
	hd.addIntArg(_sensorModule.graphHeartRate);
	sender.sendMessage(hd);

	ofxOscMessage bd;
	bd.setAddress("/breatratedata");
	bd.addIntArg(_sensorModule.graphBreathRate);
	sender.sendMessage(bd);

	ofxOscMessage bmd;
	bmd.setAddress("/movedata");
	bmd.addIntArg(_sensorModule.graphMoveRate);
	sender.sendMessage(bmd);


}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);
	ofSetColor(255);
	ofDrawBitmapString("heartRate:" + ofToString(_sensorModule.heartRate) + " heartRateAccuracy:" + ofToString(_sensorModule.heartRateAccuracy) + "\n" +
		"breathRate:" + ofToString(_sensorModule.breathRate) + " breathRateAccuracy:" + ofToString(_sensorModule.breathRateAccuracy) + "\n" +
		"graph(heartRate):" + ofToString(_sensorModule.graphHeartRate) + "\n" +
		"graph(breathRate):" + ofToString(_sensorModule.graphBreathRate) + "\n" +
		"graph(moveRate):" + ofToString(_sensorModule.graphMoveRate) + "\n", 10, 10);
	drawGraph(0, 100);
	drawReal(0, 220);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
