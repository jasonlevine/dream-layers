#pragma once

#include "ofMain.h"
#include "ofxFire.h"
#include "ofxThreshold.h"
#include "ofxBlur.h"
#include "ofxGaussianBlur.h"
#include "ofxRipples.h"
#include "ofxFluid.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void loadLayers(string dirName);
    void loadShaders();
    void getMaxAbsDiff();
    void updateFluid();
    

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    vector<ofImage> layers;
    
    ofImage diff;
    float mix;
    
    float width, height;
    
    ofShader mixShader, accumShader;
    ofFbo accumFbo;
    ofPlanePrimitive plane;
    
//    ofImage noiseTex;
    ofxFire fire;
    ofxThreshold thresh;
    ofxBlur blur;
    ofxGaussianBlur gBlur;
    ofxRipples ripples;
    ofxFluid fluid;
    
    float brightness;
    bool drawNoise;
    bool mouseDown;
    ofVec2f oldM;
    
    
};
