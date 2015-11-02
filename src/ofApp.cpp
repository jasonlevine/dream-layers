#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    loadLayers("chinesechardream1");
    loadShaders();
    getMaxAbsDiff();
//    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
    mixShader.load("mixShader");
    accumShader.load("leakyAccum");
    
    updateFluid();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    


}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofTexture accumTex = accumFbo.getTextureReference();
//    
//    accumFbo.begin();
//    ofClear(0);
//    accumShader.begin();
//    accumShader.setUniformTexture("accumTex", accumTex, 0);
//    accumShader.setUniformTexture("newTex", fire.getTextureReference(), 1);
//    
//    glBegin(GL_QUADS);
//    glVertex2f(0, 0); glTexCoord2f(0, 0);
//    glVertex2f(width, 0); glTexCoord2f(width, 0);
//    glVertex2f(width, height); glTexCoord2f(width, height);
//    glVertex2f(0, height); glTexCoord2f(0, height);
//    
//    glEnd();


//    accumShader.end();
//    accumFbo.end();
    
//    accumFbo.begin();
//    gBlur.draw();
//    ofSetColor(0,0,0,5);
//    ofRect(0,0,width, height);
//    ofSetColor(255, 100);
//    if (mouseDown) ofCircle(mouseX, mouseY, 25);
////    fire.draw(0,0);
//    accumFbo.end();
//    
//    gBlur << accumFbo.getTextureReference();
//    gBlur.update();
    
    
    accumFbo.begin();
    ofCircle(200,200,50);
    fluid.draw(0,0);
    accumFbo.end();
    
    ofSetColor(255);
    if (drawNoise) fluid.draw();
    else {
        mixShader.begin();
        for (int i = 0; i < layers.size(); i++) {
            mixShader.setUniformTexture("tex" + ofToString(i), layers[i], i);
        }
        mixShader.setUniformTexture("noiseTex", accumFbo.getTextureReference(), layers.size());
        mixShader.setUniformTexture("diffTex", blur.getTextureReference(), layers.size()+1);
        mixShader.setUniform1f("time", ofGetElapsedTimef());
        mixShader.setUniform1f("layerMix", ofMap(cos(ofGetElapsedTimef()*0.25), -1, 1, 0.0, 1.0));
        ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
        plane.draw();
        ofPopMatrix();
        mixShader.end();
    }
}


//--------------------------------------------------------------
void ofApp::loadLayers(string dirName){
    ofDirectory dir;
    dir.listDir(dirName);
    
    for (int i = 0; i < dir.size(); i++) {
        ofImage img;
        img.loadImage(dirName + "/dream_" + ofToString(i) + ".jpg");
        layers.push_back(img);
    }
    
    width = layers[0].width;
    height = layers[0].height;
}

//--------------------------------------------------------------
void ofApp::loadShaders(){
    mixShader.load("mixShader");
    accumShader.load("leakyAccum");
    
    
    accumFbo.allocate(width, height, GL_RGBA32F);
    accumFbo.begin();
    ofClear(0);
    ofSetColor(0);
    ofRect(0,0,ofGetWidth(), ofGetHeight());
    accumFbo.end();
    
    fire.allocate(width, height);
    fire.setZoom(2.0);
    brightness = 2.2;
    fire.setBrightness(brightness);
    
    
    thresh.allocate(width, height);
    thresh.threshold = 0.2;
    
    blur.allocate(width, height, GL_RGBA32F);
    blur.setFade(1.0);
    
    gBlur.allocate(width, height, GL_RGBA32F);
    gBlur.setRadius(20);
    
    ripples.allocate(width, height, GL_RGBA32F);
    
    gBlur << diff.getTextureReference();
    gBlur.update();
    
    thresh << gBlur;
    thresh.update();
    
    blur << thresh;
    blur.update();
    
    blur << blur;
    blur.update();
    
    fluid.allocate(width, height);
    fluid.dissipation = 0.995;
    fluid.velocityDissipation = 0.99;
    fluid.setGravity(ofPoint(0,0,0));
    
    plane.set(ofGetWidth(), ofGetHeight());
    plane.mapTexCoordsFromTexture(layers[0].getTextureReference());
}

//--------------------------------------------------------------
void ofApp::getMaxAbsDiff(){
    diff = layers[0];
    
    vector<ofImage> absDiffs;
    
    for (int i = 1; i < layers.size(); i++){
        
        ofImage absDiff = layers[0];
        
        //getAbsdiff
        for (int x = 0; x < diff.width; x++){
            for (int y = 0; y < diff.height; y++){
                ofColor firstCol = layers[0].getColor(x,y);
                ofColor lastCol = layers[i].getColor(x,y);
                int absD = abs(firstCol.getLightness() - lastCol.getLightness());
                absDiff.setColor(x, y, ofColor(absD));
            }
        }
        
        absDiffs.push_back(absDiff);
    }
    
    diff = absDiffs[0];
    for (int i = 1; i < absDiffs.size(); i++){
        for (int x = 0; x < diff.width; x++){
            for (int y = 0; y < diff.height; y++){
                ofColor firstCol = diff.getColor(x,y);
                ofColor lastCol = absDiffs[i].getColor(x,y);
                int max = MAX(firstCol.getLightness(), lastCol.getLightness());
                diff.setColor(x, y, ofColor(max));
            }
        }
    }
    
    diff.update();
}

//--------------------------------------------------------------
void ofApp::updateFluid(){
    ofPoint m = ofPoint(mouseX,mouseY);
    ofPoint d = (m - oldM)*10.0;
    oldM = m;
    ofPoint c = ofPoint(width, height) - m;
    c.normalize();
    fluid.addTemporalForce(m, d, ofFloatColor(1.0),3.0f);
    fluid.update();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'b') {
        brightness+=0.1;
        fire.setBrightness(brightness);
        cout << brightness << endl;
    }

    if (key == 'v') {
        brightness-=0.1;
        fire.setBrightness(brightness);
        cout << brightness << endl;
    }
    
    if (key == 'n') {
        drawNoise = !drawNoise;
    }
    
    if (key == 'f') {
        ofBackground(0);
    }
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
    mouseDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mouseDown = false;
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
