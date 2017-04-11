#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //tex.allocate(videoPlayer.getWidth(), videoPlayer.getHeight(), GL_RGBA);
    //cout << "setup finished! width: " << videoPlayer.getWidth() << " height: " << videoPlayer.getHeight() << endl;
    
    hide_video = false;
    debug_info = true;
    
    mainOutputSyphonServer.setName("Insertion Output");
    
    setupVideo();
    setupGui();
    
    ofSetFrameRate(60);
}


//--------------------------------------------------------------
//setting up the video
void ofApp::setupVideo() {
    load_video();

}

//--------------------------------------------------------------
//code retrieved from:
//http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
string ofApp::exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}


//--------------------------------------------------------------
void ofApp::setupGui() {
    gui.setup();
    gui.add(show_debug_tgl.setup("show debug info", true));
    gui.add(load_video_btn.setup("reload url from file"));
    
    int ui_width = gui.getWidth();
    gui.setSize(ui_width, gui.getHeight());
    show_debug_tgl.setSize(ui_width, show_debug_tgl.getHeight());
    load_video_btn.setSize(ui_width, load_video_btn.getHeight());
    
    show_debug_tgl.addListener(this, &ofApp::show_debug_changed);
    load_video_btn.addListener(this, &ofApp::load_video_pressed);
    
    gui.setPosition( ofGetWidth() - ui_width - 5, 5);
}

//--------------------------------------------------------------
bool ofApp::load_url_from_file() {
    ofBuffer buffer = ofBufferFromFile("video.txt");
    
    bool result = false;
    
    if(buffer.size()) {
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            
            string line = *it;
            
            if(line.empty() == false) {
                update_video_url(line);
                cout <<  "video link: " << video_url << endl;
                result = true;
            }
        }
        
    }
    
    return result;

}

//--------------------------------------------------------------
void ofApp::load_video() {
    bool loaded_from_file = load_url_from_file();
    
    //trying to load url from file
    if (loaded_from_file)
        load_m3u8_from_video_url();
    
    //if any error occurred
    else {
        string v="https://devimages.apple.com.edgekey.net/samplecode/avfoundationMedia/AVFoundationQueuePlayer_HLS2/master.m3u8";
        load_m3u8(v);
    }
}


//--------------------------------------------------------------
void ofApp::load_m3u8_from_video_url() {
    
    int quality = 95;
    string var_video_m3u8 = "";
    
    //if quality 95 is too high...
    while (var_video_m3u8.empty()) {
        stringstream cmd;
        //formats the command as a string
        cmd << "/usr/local/bin/youtube-dl -f " << quality <<" -g "  << video_url;
        //tries to execute the command
        var_video_m3u8 = exec(cmd.str().c_str());
        var_video_m3u8.pop_back();
        
        //if the video is not empty
        if (!var_video_m3u8.empty())
            //tries to find the last occurrence of https (the valid one)
            var_video_m3u8 = var_video_m3u8.substr(var_video_m3u8.rfind("https://"), var_video_m3u8.size());
        
        quality--;
    }
    
    //load the m3u8 video
    load_m3u8(var_video_m3u8);
}

//--------------------------------------------------------------
void ofApp::load_m3u8(string var_video_m3u8) {
    videoPlayer.stop();
    videoPlayer = *(new ofxAvFoundationHLSPlayer());
    videoPlayer.load(var_video_m3u8);
    cout << "video loaded! " << endl;
}

//--------------------------------------------------------------
void ofApp::update_video_url(string url) {
    video_url = url;
    cout << "update video url: " << video_url << endl;
}

//--------------------------------------------------------------
void ofApp::show_debug_changed(bool & value) {
    debug_info = value;
    cout << "show_debug_changed to: " << value << endl;
}

//--------------------------------------------------------------
void ofApp::video_url_changed(string & value) {
    update_video_url (value);
    cout << "video_url_changed to: " << value << endl;
}


//--------------------------------------------------------------
void ofApp::load_video_pressed() {
    //load_url_from_file();
    load_video();
    cout << "load_video_pressed!"<< endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    videoPlayer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    stringstream info;
    info << "URL: " << video_url<< endl << endl;
    string videoinfo = videoPlayer.getInfo();
    videoinfo.pop_back();
    info << videoinfo << endl;
    info << "framerate: " <<ofGetFrameRate() << endl;
    videoinfo = info.str();

    /*
    //getting the texture
    tex = videoPlayer.getTexture();
    //drawing the texture into the screen
    tex.draw(0, 0, ofGetWidth(), ofGetHeight());
    //sending the texture via syphon
    mainOutputSyphonServer.publishTexture(&tex);
     */
    
    int xpos = (int) (ofGetWidth()/2)-175;
    int ypos = (int) (ofGetHeight()/2)-25;
    

    //if the user wants to see the result, draws the video
    if (!hide_video)
        //draws the video
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    if (debug_info) {
        //draws test info
        ofDrawBitmapStringHighlight(info.str(), 20, 20, ofColor(ofColor::black, 90), ofColor::yellow);
        //draws test info
        ofDrawBitmapStringHighlight("insertions into mediatic circuits. test 1", xpos, ypos, ofColor(ofColor::black, 90), ofColor::yellow);
    }
    
    mainOutputSyphonServer.publishScreen();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h'){
        hide_video = !hide_video;
    }
    else if(key == 'd'){
        debug_info = !debug_info;
    }
    
    else if(key == 's'){
        gui.saveToFile("settings.xml");
    }
    else if(key == 'l'){
        gui.loadFromFile("settings.xml");
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
