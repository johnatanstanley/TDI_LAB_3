#include "ofMain.h"
#include "ofxUI.h"
#include "ofxNetwork.h"
#include "SerialClass.h"	// Para comunicacion con arduino x serial

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void reiniciar();
	string nomArchivo(int grafica, int tipo);
	ofxUICanvas *gui; 
	string getTag(int grafica, int tipo, int index);
	float getValor(int grafica, int tipo, int index);
	void guiEvent(ofxUIEventArgs &e);

    //bool drawPadding; 
	bool actualizarGraf;
	float red, green, blue; 	
	
	//Barra arrayBarras[10];
	
	float xPos;
	float yPos;
	float xPosB1;
	float yPosB1;
	//string titulo;
	ofColor color;
		
	ofxTCPClient tcpClient;
	string msgTx;

	ofTrueTypeFont  mono;
	ofTrueTypeFont  monosm;
};
