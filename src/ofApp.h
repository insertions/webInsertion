#pragma once

#include "ofMain.h"
#include "ofxAvFoundationHLSPlayer.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#define DEFAULT_VIDEO "http://devimages.apple.com.edgekey.net/samplecode/avfoundationMedia/AVFoundationQueuePlayer_HLS2/master.m3u8"
#define QUALITY_RANGE 5
#define YOUTUBE_DL_PARAMETER_FOR_LIVE_VIDEO "-g"
#define YOUTUBE_DL_PARAMETER_FOR_NON_LIVE_VIDEO "--get-url"
#define INITIAL_LIVE_VIDEO_QUALITY 95
#define INITIAL_NON_LIVE_VIDEO_QUALITY 18

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
    
        void checkSettings();
    
        void setupGui();
        void setupVideo();
    
        void show_debug_changed(bool &);
        void video_url_changed(string &);
    
        bool load_video();
        void load_video_pressed();
    
        bool read_url_from_file();
        void update_video_url(string);
    
        bool is_live_video(string);
    
        string translate_video_url(string, int, string);
        string translate_live_video_url(string);
        string translate_non_live_video_url(string);
        bool load_video_from_url(string);
        string exec_in_command_line(const char* cmd);
    
        ofxAvFoundationHLSPlayer videoPlayer;
        ofxSyphonServer mainOutputSyphonServer;
        //ofxSyphonServer ccvSyphonServer;
        //ofTexture tex;
    
        ofxPanel gui;
        ofxToggle show_debug_tgl;
        ofxButton load_video_btn;
    
        bool hide_video;
        bool debug_info;
        bool is_loading_video;
    
        int video_quality_offset;
        int video_quality;
    
        string video_url;
    
        time_t lastSaveVideoFile;
    
		
};
