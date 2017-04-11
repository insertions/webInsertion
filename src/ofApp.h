#pragma once

#include "ofMain.h"
#include "ofxAvFoundationHLSPlayer.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxInputField.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


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
    
        void setupGui();
        void setupVideo();
    
        void show_debug_changed(bool &);
        void video_url_changed(string &);
    
        void load_video();
        void load_video_pressed();
    
        bool load_url_from_file();
        void load_m3u8(string);
        void load_m3u8_from_video_url();
        void update_video_url(string);
        string exec(const char* cmd);
    
        ofxAvFoundationHLSPlayer videoPlayer;
        ofxSyphonServer mainOutputSyphonServer;
        //ofTexture tex;
    
        ofxPanel gui;
        ofxToggle show_debug_tgl;
        ofxButton load_video_btn;
    
        bool hide_video;
        bool debug_info;
    
        string video_url;
    
		
};
