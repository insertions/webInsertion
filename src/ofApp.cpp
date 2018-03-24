#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    hide_video = false;
    debug_info = true;
    face_tracking = true;
    
    video_quality_offset = 0;
    
    mainOutputSyphonServer.setName("Insertion Output");
    //ccvSyphonServer.setName("CCV Output");
    
    bool sucesss = load_video();
    
//    if (!sucesss)
//    {
//        cout << "some error happened when loading the video file." << endl <<
//                " please, try to run the software again. exiting now..." << endl;
//        throw std::exception();
//    }
    
    setupGui();
    
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    face_overlay.load("demo.png");
    
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::checkSettings(){
    boost::filesystem::path filePath = ofToDataPath("video.txt");
    time_t t = boost::filesystem::last_write_time(filePath);
    if (t != lastSaveVideoFile) {
        lastSaveVideoFile = t;
        load_video();
    }
}


//--------------------------------------------------------------
//code retrieved from:
//http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
string ofApp::exec_in_command_line(const char* cmd) {
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

bool ofApp::is_live_video(string url) {
    if (url.find("manifest") != std::string::npos)      return true;
    if (url.find("videoplayback") != std::string::npos) return false;
    
    cout << "the URL provided is not a valid youtube video." << endl;
    throw std::exception();
}

//--------------------------------------------------------------
bool ofApp::load_video() {
    videoPlayer.stop();
    
    read_url_from_file();
    
    string translated_url = "";
    
    //try to load url as a live video
    translated_url = translate_live_video_url(video_url);
    
    //if no results were found for live videos...
    if (translated_url.empty())
    // ...try non-live videos
        translated_url = translate_non_live_video_url(video_url);
    
    //if no results were found again, use default video...
    if (translated_url.empty())
        translated_url = DEFAULT_VIDEO;
    
    return load_video_from_url(translated_url);
}

//--------------------------------------------------------------
bool ofApp::read_url_from_file() {
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
    
    //updating the last time saved
    boost::filesystem::path filePath = ofToDataPath("video.txt");
    lastSaveVideoFile = boost::filesystem::last_write_time(filePath);
    
    return result;
}

//--------------------------------------------------------------
string ofApp::translate_live_video_url(string url){
    return translate_video_url(url,
                               INITIAL_LIVE_VIDEO_QUALITY+video_quality_offset,
                               YOUTUBE_DL_PARAMETER_FOR_LIVE_VIDEO);
}

//--------------------------------------------------------------
string ofApp::translate_non_live_video_url(string url){
    return translate_video_url(url,
                               INITIAL_NON_LIVE_VIDEO_QUALITY+video_quality_offset,
                               YOUTUBE_DL_PARAMETER_FOR_NON_LIVE_VIDEO);
}

//--------------------------------------------------------------
string ofApp::translate_video_url(string url, int quality, string parameter){
    
    video_quality = quality;
    string translated_url = "";
    int quality_limit = quality - QUALITY_RANGE;
    
    //if quality 95 is too high...
    while (translated_url.empty() && quality >= quality_limit) {
        stringstream cmd;
        //formats the command as a string
        //cmd << "/usr/local/bin/youtube-dl -f " << quality <<" -g "  << url;
        cmd << "/usr/local/bin/youtube-dl -f " << quality << " " << parameter << " " << url;
        
        //tries to execute the command
        translated_url = exec_in_command_line(cmd.str().c_str());
        translated_url.pop_back();
        
        //if the video is not empty
        if (!translated_url.empty())
            //tries to find the last occurrence of https (the valid one)
            translated_url = translated_url.substr(translated_url.rfind("https://"), translated_url.size());
        
        quality--;
    }
    
    if (translated_url.empty())
        return "";
    else
        return translated_url;
}

//--------------------------------------------------------------
bool ofApp::load_video_from_url(string var_video) {
    //videoPlayer = *(new ofxAvFoundationHLSPlayer());
    bool success = videoPlayer.load(var_video);
    return success;
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
    ofSystem("open "+ofToDataPath("video.txt"));
    cout << "opening video.txt file..." << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (ofGetFrameNum() % 30 == 0)
        checkSettings();
    
    if (ofGetFrameNum() % 600 == 0)
        results_face.clear();
    
    videoPlayer.update();
    
    if (face_tracking)
        update_OSC_messages();
    
    
}

//--------------------------------------------------------------
void ofApp::update_OSC_messages() {
    int i = 0;
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
    //if(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        //cout << m.getAddress() << endl;
        
        if (m.getAddress() == "/face") {
            int id = m.getArgAsInt(0);
            
            float x  = m.getArgAsFloat(1);
            float y  = m.getArgAsFloat(2);
            float w  = m.getArgAsFloat(3);
            float h  = m.getArgAsFloat(4);
            
            if (results_face.size() > i)
                results_face.erase(results_face.begin()+i);
            
            results_face.insert(results_face.begin()+i, ofRectangle(x, y, w, h));

        }
        
        i++;
        /*
         // check for mouse moved message
         if(m.getAddress() == "/mouse/position"){
         // both the arguments are int32's
         mouseX = m.getArgAsInt32(0);
         mouseY = m.getArgAsInt32(1);
         }
         // check for mouse button message
         else if(m.getAddress() == "/mouse/button"){
         // the single argument is a string
         mouseButtonState = m.getArgAsString(0);
         }
         // check for an image being sent (note: the size of the image depends greatly on your network buffer sizes - if an image is too big the message won't come through )
         else if(m.getAddress() == "/image" ){
         ofBuffer buffer = m.getArgAsBlob(0);
         receivedImage.load(buffer);
         }
         else{
         // unrecognized message: display on the bottom of the screen
         string msg_string;
         msg_string = m.getAddress();
         msg_string += ": ";
         for(int i = 0; i < m.getNumArgs(); i++){
         // get the argument type
         msg_string += m.getArgTypeName(i);
         msg_string += ":";
         // display the argument - make sure we get the right type
         if(m.getArgType(i) == OFXOSC_TYPE_INT32){
         msg_string += ofToString(m.getArgAsInt32(i));
         }
         else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
         msg_string += ofToString(m.getArgAsFloat(i));
         }
         else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
         msg_string += m.getArgAsString(i);
         }
         else{
         msg_string += "unknown";
         }
         }
         // add to the list of strings to display
         msg_strings[current_msg_string] = msg_string;
         timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
         current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
         // clear the next line
         msg_strings[current_msg_string] = "";
         }
         */
        
    }
    
    
    
//    ofxOscMessage m;
//    while(receiver.hasWaitingMessages())
//        receiver.getNextMessage(m);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    stringstream info;
    info << "URL: " << video_url<< endl << endl;
    string videoinfo = videoPlayer.getInfo();
    videoinfo.pop_back();
    info << videoinfo << endl;
    info << "framerate: " <<ofGetFrameRate() << endl;
    info << "video quality: " << video_quality << endl;
    info << "resolution: " <<ofGetWidth() << "x" << ofGetHeight() << endl << endl;
    info << "jeraman.info, 2017" << endl;
    videoinfo = info.str();

    //getting the texture
    //tex = videoPlayer.getTexture();
    //drawing the texture into the screen
    //tex.draw(0, 0, ofGetWidth(), ofGetHeight());
    //sending the texture via syphon
    //ccvSyphonServer.publishTexture(&tex);
    
    
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
    
    
    if (face_tracking) {
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        
        for(int i = 0; i < results_face.size(); i++) {
            ofRectangle face(results_face[i].getX()*ofGetWidth(),
                             results_face[i].getY()*ofGetHeight(),
                             results_face[i].getWidth()*ofGetWidth(),
                             results_face[i].getHeight()*ofGetHeight());
            //ofDrawRectangle(face);
            //ofDrawBitmapStringHighlight(ofToString(i), face.getPosition());
            
            int x = (results_face[i].getX()*ofGetWidth());
            int y = (results_face[i].getY()*ofGetHeight());
            int w = (results_face[i].getWidth() * 3 * face_overlay.getWidth());
            int h = (results_face[i].getHeight() * 3 * face_overlay.getHeight());
            face_overlay.draw(x, y, w, h);
        }
        
        ofPopStyle();
    }
    
    
    
    mainOutputSyphonServer.publishScreen();
    //ccvSyphonServer.publishScreen();
    
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
    else if(key == '['){
        video_quality_offset--;
        load_video();
    }
    else if(key == ']'){
        video_quality_offset++;
        load_video();
    }
    else if(key == 'f')
         face_tracking = !face_tracking;
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
