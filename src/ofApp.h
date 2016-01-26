#pragma once

#include "ofMain.h"
#include "LSystem.h"
#include "ofxDatGui.h"
#include <regex>
#include <map>

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
    
    
    vector<ofxDatGuiComponent*> components;
    ofxDatGui* gui;
    ofXml XML;

    void onNewTextEvent(ofxDatGuiTextInputEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    string replaceWithMoves();

    ofTrueTypeFont font, header;
		
    LSys system;
    map <string, string> variableDance;

};
