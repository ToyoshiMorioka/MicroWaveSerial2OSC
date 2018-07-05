//
//  ofxSMWSM.cpp
//  SHARP_microwaveSensor
//
//  Created by MORIOKAToyoshi on 2015/11/13.
//
//

#include "ofxSMWSM.h"

const int baudrate = 115200; /** 違うタイプもあるかも */
const short maxGraph = 0x7fff;
const short minGraph = 0x8000;

ofxSMWSM::ofxSMWSM(){
    graphHeartRate = 0;
    graphBreathRate = 0;
    graphMoveRate = 0;
    heartRate = 0;
    breathRate = 0;
    isConnected = false;
}

ofxSMWSM::~ofxSMWSM(){
    if (serial.available()) {
        serial.close();
    }
}

void ofxSMWSM::setup(string portName){
    isConnected = serial.setup(portName, baudrate);
    if (isConnected){
        serial.flush();
    }
}

void ofxSMWSM::setup(int portNum){
    isConnected = serial.setup(portNum, baudrate);
    if (isConnected){
        serial.flush();
    }
}

void ofxSMWSM::update(){
    if (isConnected) {
        unsigned char oneBuffer = 0;
        
        int nRead  = 0;
        messageCount = 0;
        
        while((nRead = serial.readBytes(&oneBuffer, 1) > 0)){
            buffer.push_back(oneBuffer);
            messageCount++;
        }
        
        // check preamble + type + length
       // std::for_each(buffer.cbegin(), buffer.cend(), [](int x){
            // std::cout << x << " ";
        //});
        
        if (buffer.size() > 10) {
            vector<int> header(10, 0);
            auto itr = buffer.begin();
            
            // copy header
            for (int i = 0; i < 10; i++) {
                header[i] = *itr;
                ++itr;
            }
            
            // cout << header[0]<< ":" << header[1] << ":" << header[2] << ":" << header[3] << ":" << header[4] << ":" << header[5] << ":" << header[6] << ":" << header[7] << endl;
            
            if (isHeader(header)) {
                // cout << "has header" << endl;
                
                // delete header from buffer
                
                int type = header[8];
                int length = header[9];
                
                // cout << "type:" << type << " length:" << length << endl;
                
                // parse message
                switch (type) {
                    case 1: // data
                        // cout << "data" << endl;
                        graphHeartRate = char(*itr) * 256;
                        ++itr;
                        graphHeartRate+= char(*itr);
                        ++itr;
                        graphBreathRate = char(*itr)* 256;
                        ++itr;
                        graphBreathRate += char(*itr);
                        ++itr;
                        graphMoveRate = char(*itr) * 256;
                        ++itr;
                        graphMoveRate+= char(*itr);
                        break;
                    case 2: //heartrate
                        // cout << "heartrate" << endl;
                        heartRate = *itr;
                        ++itr;
                        heartRateAccuracy = *itr;
                        cout << "heartRate:" << heartRate << " accuracy:" << heartRateAccuracy << endl;
                        break;
                    case 3: // breathrate
                        // cout << "breathrate" << endl;
                        breathRate = *itr;
                        ++itr;
                        breathRateAccuracy = *itr;
                        cout << "breathRate:" << breathRate << " accuracy:" << breathRateAccuracy << endl;
                        break;
                    default:
                        break;
                }
                
                for (int i = 0; i < 10 + length + 2; i++) {
                    buffer.pop_front();
                }
                
            }else{
                buffer.pop_front();
            }
        }
//        bool finish = false;
//        while (buffer.size() > 2 && finish == false) {
//            int type = buffer.front();
//            buffer.pop_front();
//            int length = buffer.front();
//            buffer.pop_front();
//
//           // cout << "message type:"<< type << " length:" << length << endl;
//
//            if (buffer.size() >= length) {
//                serializeData(type, buffer, length);
//            }else{
//                buffer.push_front(length);
//                buffer.push_front(type);
//                finish = true;
//            }
//        }
    }
}

bool ofxSMWSM::isHeader(vector<int> &header){
    // bool result = false;
    
    if (header[0] != 128) {
        return false;
    }
    
    if (header[1] != 0){
        return false;
    }
    
    if (header[2] != 128) {
        return false;
    }
    
    if (header[3] != 0) {
        return false;
    }
    
    if (header[4] != 128) {
        return false;
    }
    
    if (header[5] != 0) {
        return false;
    }
    
    if (header[6] != 128) {
        return false;
    }
    
    if (header[7] != 0) {
        return false;
    }
    
    return true;
}

void ofxSMWSM::serializeData(int type, list<unsigned char> &data, int length){
    switch (type) {
        case 1:
            // raw data
            if (length == 6) {
                // cout << "match data size."<< length << endl;
                graphHeartRate = char(data.front()) * 256;
                data.pop_front();
                graphHeartRate+= char(data.front());
                data.pop_front();
                graphBreathRate = char(data.front())* 256;
                data.pop_front();
                graphBreathRate += char(data.front());
                data.pop_front();
                graphMoveRate = char(data.front()) * 256;
                data.pop_front();
                graphMoveRate+= char(data.front());
                data.pop_front();
                
            }else{
                // cout << "type1 but unknown length:"<< length << endl;
            }
            
            break;
        case 2:
            cout << "heartrate detected."<< length << endl;
            if (length == 1){
                heartRate = data.front();
                data.pop_front();
                cout << "heartRate:" << heartRate << endl;
                
            }else if (length == 2){
                heartRate = data.front();
                data.pop_front();
                heartRateAccuracy = data.front();
                data.pop_front();
                cout << "heartRate:" << heartRate << " accuracy:" << heartRateAccuracy << endl;
                
            }else{
                cout << "type2 but unknown length:"<< length << endl;
            }
            break;
        case 3:
            cout << "breathrate detected."<< length << endl;
            if (length == 1) {
                breathRate = data.front();
                data.pop_front();
                cout << "breathRate:" << breathRate << endl;
            }else if (length == 2) {
                breathRate = data.front();
                data.pop_front();
                breathRateAccuracy = data.front();
                data.pop_front();
                cout << "breathRate:" << breathRate << " accuracy:" << breathRateAccuracy << endl;
            }else{
                cout << "type3 but unknown length:"<< length << endl;
            }
            
            break;
        case 4:
            for (int i = 0; i < length; i++) {
                data.pop_front();
            }
            break;
        default:
            // cout << "unknown type unknown length:"<< length << endl;
            break;
    }
}
