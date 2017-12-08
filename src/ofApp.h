#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "maximilian.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
        void audioOut(float * input, int bufferSize, int nChannels); //output audio to the speakers

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
    
    
    // 3D TERRAIN
    int rows, cols;
    int scale = 15; //resolution
    float width = 1300;
    float height = 1500;
    
    float flying;
    
    float sat = 0;
    float hue = 100;
    float br = 255;
    
    float zvalue[0][0]; //multi-dimensional array to store z values of each vertex
    
    ofEasyCam easyCam;
    
    ofMesh mesh;
    
    
    // MAXIMILIAN STUFF
    
    // Timer
    // creates rhythms by triggering samples at specific times (it's like a metronome)
    maxiClock timer;
    
    //FFT
    maxiFFT myFFT;
    
    // Samples
    maxiSample baseSample; //base audio sample
    maxiSample pianoSample; //piano audio sample
    
    // Variables
    float kickGain; //a value that acts as a 'kick'
    int counter; //keeps track of the timer
    int myCounter; //another counter for drawing output
    double sampleOut; //stores the output audio
    int sampleRate = 44100;
    float fftSize = 1024;
    int bufferSize = 512;
    float drawOutput [512]; //to draw bufferSize
    
    float sum = 0;
    float average = 0;

    
};