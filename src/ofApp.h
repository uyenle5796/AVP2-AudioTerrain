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
    ofEasyCam easyCam; //camera that allows looking around
    
    ofMesh mesh; //mesh object for creating the terrain
    
    int rows, cols;
    float width = 1000; //width of the mesh
    float height = 800; //height of the mesh
    int scale = 20; //resolution

    float flying;
    
    float sat = 0;
    float hue = 100;
    float br = 255;
    
    float zvalue[0][0]; //multi-dimensional array to store z values of each vertex
    
    
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

    
};
