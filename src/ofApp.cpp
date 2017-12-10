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
 - featureExtractor example
 - ofMesh documentation: http://openframeworks.cc/documentation/3d/ofMesh/
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
    
    zvalue[rows][cols] = { }; //multi-dimensional array to store z values of each vertex
    
    //Create the terrain
    
    //Terrain mesh conists of rows and columns, which corresponds with y (height) and x (width)
    //The mesh is connected by sets of vertices across and down in a triangular shape
    for(int y=0; y < rows-1; y++) {
        for(int x=0; x < cols; x++) {
            
            mesh.addColor(ofColor(hue, sat, br)); //assign the colours as defined in setup to each vertex
            
            mesh.addVertex(ofPoint(x*scale, y*scale, zvalue[x][y])); //draw set of vertices across
            mesh.addVertex(ofPoint(x*scale, (y+1)*scale, zvalue[x][y+1]));  //draw set of vertices down (y+1)
            
            //Add colour gradients by changing the saturation hue and brightness of each pixel
            sat += 0.6;
            hue += 0.6;
            if (sat > 255) sat = 0;
            if (hue > 255) hue = 0;
        }
    }
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP); //This mode allows us to create the mesh of triangle-shaped vertices
    
    
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
    
    //Make the vertices move along with the audio

    auto &verts = mesh.getVertices(); //Get all vertices of the mesh
    
    // For each vertex
    for(int i=0; i < verts.size(); i++ ) {
        // Loop through the bufferSize (512) to get the FFT magnitudes of the audio
        for(int j=0; j < bufferSize; j++) {
            
            int scale = 10; //scale up movement 10 times more to make it easier to see the movements
            
            //Assign audio magnitude to the vertices in the Z axis
            //so that each vertex moves up and down, along with the magnitudes of the audio
            verts[i].z = myFFT.magnitudes[i] * scale;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    easyCam.begin(); //start easyCam
    
    glEnable(GL_DEPTH_TEST);
    
    // Transform the mesh
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2-500, ofGetHeight()/2-100); //move the mesh to center of the window
    ofRotateX(ofRadToDeg(PI/1.7)); //lay it flat down
    
    //move it again so that it sits in the middle of the screen
    ofTranslate(-width/2, -height/2, 300);

    ofScale(1.5, 1.5); //scale it up by factor of 1.5 to make it bigger
    
    mesh.drawVertices(); //draws the vertices only (no edges)
                         //which gives us only the points
    
    // Finish transformations
    ofPopMatrix();
    
    easyCam.end(); //end easyCam
}

//--------------------------------------------------------------
void ofApp::audioOut (float *output, int bufferSize, int nChannels) {

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
  
        // calculate floating point absolute value of the sample
        // so that the values are always positive
        
        // Assign audioOoutputAudiout to both channels of the speaker
        output[i*nChannels    ] = sampleOut; //left channel
        output[i*nChannels + 1] = sampleOut; //right channel
        
    }

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
