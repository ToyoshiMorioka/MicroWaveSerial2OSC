//
//  ofxSMWSM.h
//  SHARP_microwaveSensor
//
//  Created by MORIOKAToyoshi on 2015/11/13.
//
//

#ifndef ofxSMWSM_hpp
#define ofxSMWSM_hpp

#include "ofMain.h"

class ofxSMWSM{
public:
    int graphHeartRate; /** グラフの心拍(単位不明) */
    int graphBreathRate; /** グラフの呼吸(単位不明 */
    int graphMoveRate; /** グラフの体動(単位不明) */
    int heartRate; /** 心拍数(per minute) */
    int breathRate; /** 呼吸数(per minute) */
    int heartRateAccuracy; /** 心拍数(per minute) */
    int breathRateAccuracy; /** 呼吸数(per minute) */
    
    ofxSMWSM();
    ~ofxSMWSM();
    
    void setup(string portName);
    void setup(int portNum);
    void update();
    bool isConnected;

private:
    ofSerial serial;
    
    int messageCount;
    list<unsigned char> buffer;
    
    void serializeData(int type, list<unsigned char> &data, int length);
    
    bool isHeader(vector<int> &header);
};

#endif /* ofxSMWSM_h */
