#pragma once

#include "ofMain.h"
#include "NBody.hpp"
#include "ofxVFX.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	NBody mNBody;
	ofxVFX mVFX;

	bool mIsGui = false;
};
