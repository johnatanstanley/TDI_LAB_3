#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxNetwork.h"

// constantes de posicion donde estaría el primer motor
const int baseX = 100;
const int baseY = 400;
const int cant = 12;
const int visibles = 5;
const int radioBase = 90;
const int cantidadTextos = 34;

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		
		ofxTCPServer TCP;
		ofTrueTypeFont  mono;
		ofTrueTypeFont  monosm;
};
