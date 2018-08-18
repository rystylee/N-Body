#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	ofGLFWWindowSettings s;
	s.setGLVersion(4, 3);
	s.width = 1920;
	s.height = 1080;
	ofCreateWindow(s);

	ofRunApp(new ofApp());
}
