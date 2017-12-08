#include "ofApp.h"

/*
 
 Advanced Audio-Visual Processing Coursework
 Project 2: Visualising audio with 3D Terrain
 
 by Uyen Le
 tle004@gold.ac.uk
 
 ----------------
 A simple project that explores how to visualise audio with 3D Terrain.
 
 ----------------
 Credits:
 - Samples from https://www.freesound.org
 - 3D Perlin Noise Terrain tutorial: https://www.youtube.com/watch?v=IKB1hWWedMk
 */


//--------------------------------------------------------------
void ofApp::setup(){
    
    // Canvas settings
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetupScreen();
    ofBackground(0, 0, 0); //black background
    ofSetFrameRate(60);
    
    //3D TERRAIN SETUP
    rows = width/scale;
    cols = height/scale;
    
    zvalue[rows][cols] = { };
    
    //Create the mesh
    for(int y=0; y < rows-1; y++) {
        for(int x=0; x < cols; x++) {
            
            mesh.addColor(ofColor(hue, sat, br));
            
            mesh.addVertex(ofPoint(x*scale, y*scale, zvalue[x][y])); //draw set of vertices across
            mesh.addVertex(ofPoint(x*scale, (y+1)*scale, zvalue[x][y+1]));  //draw set of vertices down (y+1)
            
            //Colours
            sat += 0.05;
            hue += 0.025;
            if (sat > 255) sat = 0;
            if (hue > 255) hue = 0;
        }
    }
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    
    // MAXIMILIAN SETUP
    //Setup FFT
    myFFT.setup(fftSize, 512, 256);
    
    // Setup the timer
    timer.setTempo(160); //tempo is how fast we want the timer to be (the pace)
    //here we set to 160 bpm (beats per minute)
    timer.setTicksPerBeat(6); //6 ticks per beat
    
    // Load samples from files
    baseSample.load(ofToDataPath("/Users/uyenle/Desktop/AudioVisual/AVPCoursework_tle004/AVP2-AudioTerrain/bin/data/shnur_base.wav"));
    pianoSample.load(ofToDataPath("/Users/uyenle/Desktop/AudioVisual/AVPCoursework_tle004/AVP2-AudioTerrain/bin/data/lemoncreme_piano.wav"));
    
    //Setup the audio output
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0,this, sampleRate, bufferSize, 4);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Get vertices of the mesh
    auto &verts = mesh.getVertices();
    
    //Add noise to the z coordinate vertices
    for(int i=0; i < verts.size(); i++ ) {
        for(int j=0; j < bufferSize; j++) {
            
            int scale = 10;
            
            //Assign magnitude of the audio to the vertices in the Z axis
            // so that the terrain moves along with the magnitudes of the audio
            verts[i].z = myFFT.magnitudes[i] * scale; //multiply with scale of 10 to make it easier to see the movements
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    easyCam.begin();
    
    glEnable(GL_DEPTH_TEST);
    
    ofSetColor(255,200,1, 255);

    // Transform the mesh
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2-1000, ofGetHeight()/2-100);
    ofRotateX(ofRadToDeg(PI/1.7));
    ofTranslate(-width/2, -height/2, 500);
    ofScale(1.5, 1.5);
    
    mesh.drawVertices();
    
    ofPopMatrix();
    
    easyCam.end();
}

//--------------------------------------------------------------
void ofApp::audioOut (float *output, int bufferSize, int nChannels) {
    
    sum = 0;
    average = 0;
    
    for (int i = 0; i < bufferSize; i++) {
        
        //Start the timer
        timer.ticker(); // ticker ticks at the current sample rate (44,100)
        
        /* Create the fun, upbeat audio */
        
        if (timer.tick) { // If timer is ticking
            
            counter++; //increase the counter by 1 each time the ticker ticks
            
            //  Here we manipulate the samples by setting the playback speed.
            //  We trigger them at certain times using modulus calculations.
            //  Modulus calculation allows us recursively looping through the counter values and triggering the samples whenever the calculation result matches the defined remainder.
            
            //Base
            if (counter % 6 == 2) { //divide the counter value every 6 ticks
                //trigger baseSample when remainder is 2
                baseSample.trigger();
            }
            if (counter % 8 == 6) { //same as above
                //divide the counter value every 8 ticks
                //trigger baseSample when remainder is 6
                baseSample.trigger();
            }
        }
        
        //Piano (not using timer)
        if (counter % 8 != 4) {
            pianoSample.trigger();
        }
        
        // Store both samples to sampleOut
        sampleOut = pianoSample.playOnce() + baseSample.playOnce(); //play samples just once
        
        if (myFFT.process(sampleOut)) {
            
            drawOutput[myCounter % bufferSize] = sampleOut;
            
            myCounter++;
        }
        
        sum += fabs(sampleOut);
        // calculate floating point absolute value of the sample
        // so that the values are always positive
        
        // Assign audioOoutputAudiout to both channels of the speaker
        output[i*nChannels    ] = sampleOut; //left channel
        output[i*nChannels + 1] = sampleOut; //right channel
        
    }
    
    //sum = sqrt(sum);
    
    average = sum / bufferSize;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
