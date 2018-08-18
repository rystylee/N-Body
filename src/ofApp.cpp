#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetBackgroundColor(0);
	ofEnableDepthTest();

	mNBody.setup(512 * 80);

	mVFX.setup(ofGetWidth(), ofGetHeight());
	mVFX.setVFXMode(ofxVFXMode::BLOOM);
	mVFX.setBloomAttenuation(4.0);
}

//--------------------------------------------------------------
void ofApp::update()
{
	float t = ofGetElapsedTimef();
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	mNBody.update(t);
	mVFX.update(t);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	mVFX.begin();
	mNBody.draw();
	mVFX.end();

	mVFX.draw();
	mNBody.drawGui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if(key == ' ')
		mNBody.setDt(0.004);
	switch (key)
	{
	case ' ':
		mNBody.setDt(0.004);
		break;
	case 'g':
		mIsGui = !mIsGui;
		mNBody.setIsGui(mIsGui);
	default:
		break;
	}
}

//--------------------------------------------------------------