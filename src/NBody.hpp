#pragma once

#include "ofMain.h"
//#include "ofBufferObject.h"
#include "ofxImGui.h"
#include "ofxAutoReloadedShader.h"

#include <random>

struct  Particle
{
	ofVec4f pos; // w is mass
	ofVec4f vel; // w isn't used
};
	
class NBody
{
public:
	NBody();
	void setup(const int n);
	void update(const float t);
	void draw();
	void drawGui();

	// Setter
	inline void setIsGui(const bool isGui) { mIsGui = isGui; }
	inline void setG(const float g) { mG = g; }
	inline void setDt(const float dt) { mDt = dt; }
	inline void setEPS2(const float eps2) { mEPS2 = eps2; }
	inline void setPower(const float power) { mPower = power; }

private:
	void initParticles(const int initMode);

	ofEasyCam mCam;
	float mTime;

	static const int WORK_GROUP_SIZE = 512;
	int mNumParticles;
	ofShader mComputeShader, mRenderShader;
	vector<Particle> mParticles;
	ofBufferObject mParticlesBuffer;
	ofVbo mVbo;

	ofxImGui::Gui mGui;
	bool mIsGui = false;
	float mG = 0.001;
	float mDt = 0.0;
	float mEPS2 = 0.005;
	float mPower = 1.5;
};
