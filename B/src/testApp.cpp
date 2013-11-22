#include "testApp.h"


ofxUICanvas *gui; 

float* datavector;
string* tagvector;
ofColor colorvector[12];

int Nposicion = 0;
int Nbarra = 0;

ofxUILabel* titulo = NULL;
ofxUIToggleMatrix* matrix = NULL;
ofxUIRotarySlider* barra1 = NULL;
ofxUIRotarySlider* barra2 = NULL;
ofxUIRotarySlider* barra3 = NULL;
ofxUIRotarySlider* barra4 = NULL;
ofxUISlider* slider1 = NULL;
ofxUISlider* slider2 = NULL;
ofxUISlider* slider3 = NULL;
ofxUISlider* slider4 = NULL;


void testApp::setup()
{
	ofSetVerticalSync(true);
	TCP.setup(11999);
	TCP.setMessageDelimiter("\n");
	
	tagvector = new string[4];
	datavector = new float[4];
	for (int i = 0; i < 4; i++)
		datavector[i] = 0;

	colorvector[0] = ofColor(0x1e, 0x90, 0xff);
	colorvector[1] = ofColor(0xad, 0xff, 0x2f);
	colorvector[2] = ofColor(0xff, 0x14, 0x93);
	colorvector[3] = ofColor(0xff, 0xd7, 0x00);
	colorvector[4] = ofColor(0x5f, 0x9e, 0xa0);
	colorvector[5] = ofColor(0xdc, 0x14, 0x3c);
	colorvector[6] = ofColor(0xbd, 0xb7, 0x6b);
	colorvector[7] = ofColor(0xff, 0x69, 0xb4);
	colorvector[8] = ofColor(0x5f, 0x9e, 0xa0);
	colorvector[9] = ofColor(0x9a, 0xcd, 0x32);
	colorvector[10] = ofColor(0xfa, 0x80, 0x72);
	colorvector[11] = ofColor(0x80, 0x80, 0x00);
	
	ofEnableSmoothing();

	float dimX = 60;
	float dimY = 600;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = ofGetWidth();//620-xInit;
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    gui->setColorBack(ofColor(60,150));
	
    slider1 = gui->addSlider("slider1", 0, 0.9, 0.25, dimX, dimY);
	slider1->setColorFill(colorvector[0]);
	slider1->setColorBack(ofColor(60, 50));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	slider2 = gui->addSlider("slider2", 0, 0.9, 0.50, dimX, dimY);
	slider2->setColorFill(colorvector[1]);
	slider2->setColorBack(ofColor(60, 50));
	slider3 = gui->addSlider("slider3", 0, 0.9, 0.75, dimX, dimY);
	slider3->setColorFill(colorvector[2]);
	slider3->setColorBack(ofColor(60, 50));
	slider4 = gui->addSlider("slider4", 0, 0.9, 1.00, dimX, dimY);
	slider4->setColorFill(colorvector[3]);
	slider4->setColorBack(ofColor(60, 50));

	barra1 = new ofxUIRotarySlider(120, 0, 1, 0., "Barra1");
    gui->addWidgetDown(barra1);
	barra1->setColorBack(ofColor(60, 50));
	barra1->setColorFill(colorvector[0]);
	barra2 = new ofxUIRotarySlider(120, 0, 1, 0., "Barra2");
    gui->addWidgetDown(barra2);
	barra2->setColorBack(ofColor(60, 50));
	barra2->setColorFill(colorvector[1]);
	barra3 = new ofxUIRotarySlider(120, 0, 1, 0., "Barra3");
    gui->addWidgetDown(barra3);
	barra3->setColorBack(ofColor(60, 50));
	barra3->setColorFill(colorvector[2]);
	barra4 = new ofxUIRotarySlider(120, 0, 1, 0., "Barra4");
    gui->addWidgetDown(barra4);
	barra4->setColorBack(ofColor(60, 50));
	barra4->setColorFill(colorvector[3]);

	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    ofxUISpacer* separador = gui->addSpacer(length-xInit, 1);
	
	titulo = new ofxUILabel("TIPO DE DATOS", OFX_UI_FONT_MEDIUM);
    //gui->addWidgetDown(new ofxUILabel("DATOS", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(titulo);
	
	titulo->getRect()->setY(titulo->getRect()->getY() - 480);
	separador->getRect()->setY(separador->getRect()->getY() - 480);

	barra1->getRect()->setY(barra1->getRect()->getY() - 90);
	barra2->getRect()->setY(barra1->getRect()->getY());
	barra3->getRect()->setY(barra1->getRect()->getY());
	barra4->getRect()->setY(barra1->getRect()->getY());
	
	barra1->getRect()->setX(barra1->getRect()->getX() + 20);
	barra2->getRect()->setX(barra1->getRect()->getX() + 150);
	barra3->getRect()->setX(barra2->getRect()->getX() + 150);
	barra4->getRect()->setX(barra3->getRect()->getX() + 150);
	
	slider1->getRect()->setX(slider1->getRect()->getX() + 50);
	slider2->getRect()->setX(slider2->getRect()->getX() + 134);
	slider3->getRect()->setX(slider3->getRect()->getX() + 218);
	slider4->getRect()->setX(slider4->getRect()->getX() + 302);

    ofEnableSmoothing();
    ofSetFrameRate(30);
}


void testApp::update()
{
	if( TCP.isClientConnected(0) )
	{
		string str = TCP.receive(0);
		if ( str.length() > 0)
		{
			cout << str<< endl;

			string xtitulo, xtag1, xtag2, xtag3, xtag4, xsval1, xsval2, xsval3, xsval4, xsvalb1, xsvalb2, xsvalb3, xsvalb4, xspos;
			float xval1, xval2, xval3, xval4, xvalb1, xvalb2, xvalb3, xvalb4;
			int xpos;

			xtitulo = str.substr(0, str.find("-"));
			str = str.substr(str.find("-") + 1);
			xtag1 = str.substr(0, str.find("-"));
			str = str.substr(str.find("-") + 1);
			xtag2 = str.substr(0, str.find("-"));
			str = str.substr(str.find("-") + 1);
			xtag3 = str.substr(0, str.find("-"));
			str = str.substr(str.find("-") + 1);
			xtag4 = str.substr(0, str.find("-"));
			str = str.substr(str.find("-") + 1);

			xsval1 = str.substr(0, str.find("-"));
			xval1 = atof(xsval1.c_str());
			str = str.substr(str.find("-") + 1);
			xsval2 = str.substr(0, str.find("-"));
			xval2 = atof(xsval2.c_str());
			str = str.substr(str.find("-") + 1);
			xsval3 = str.substr(0, str.find("-"));
			xval3 = atof(xsval3.c_str());
			str = str.substr(str.find("-") + 1);
			xsval4 = str.substr(0, str.find("-"));
			xval4 = atof(xsval4.c_str());
			str = str.substr(str.find("-") + 1);

			xsvalb1 = str.substr(0, str.find("-"));
			xvalb1 = atof(xsvalb1.c_str());
			str = str.substr(str.find("-") + 1);
			xsvalb2 = str.substr(0, str.find("-"));
			xvalb2 = atof(xsvalb2.c_str());
			str = str.substr(str.find("-") + 1);
			xsvalb3 = str.substr(0, str.find("-"));
			xvalb3 = atof(xsvalb3.c_str());
			str = str.substr(str.find("-") + 1);
			xsvalb4 = str.substr(0, str.find("-"));
			xvalb4 = atof(xsvalb4.c_str());
			str = str.substr(str.find("-") + 1);

			xspos = str.substr(0, str.find("-"));
			xpos = atof(xspos.c_str());
			
			barra1->setValue(0);
			barra2->setValue(0);
			barra3->setValue(0);
			barra4->setValue(0);
			
			slider1->setValue(0);
			slider2->setValue(0);
			slider3->setValue(0);
			slider4->setValue(0);
			
			datavector[0] = xvalb1;
			datavector[1] = xvalb2;
			datavector[2] = xvalb3;
			datavector[3] = xvalb4;

			tagvector[0] = xtag1 + '\n' + '\n' + xsval1;
			tagvector[1] = xtag2 + '\n' + '\n' + xsval2;
			tagvector[2] = xtag3 + '\n' + '\n' + xsval3;
			tagvector[3] = xtag4 + '\n' + '\n' + xsval4;

			Nposicion = xpos;
			titulo->setLabel(xtitulo);
		}
	}
}


void testApp::draw()
{
	barra1->setValue(barra1->getValue() + 0.01);
	if(barra1->getValue() >= 1)
		barra1->setValue(0);
	barra2->setValue(barra2->getValue() + 0.01);
	if(barra2->getValue() >= 1)
		barra2->setValue(0);
	barra3->setValue(barra3->getValue() + 0.01);
	if(barra3->getValue() >= 1)
		barra3->setValue(0);
	barra4->setValue(barra4->getValue() + 0.01);
	if(barra4->getValue() >= 1)
		barra4->setValue(0);

	slider1->setValue(slider1->getValue() + 0.02);
	if(slider1->getValue() > datavector[0])
		slider1->setValue(datavector[0]);
	slider2->setValue(slider2->getValue() + 0.02);
	if(slider2->getValue() > datavector[1])
		slider2->setValue(datavector[1]);
	slider3->setValue(slider3->getValue() + 0.02);
	if(slider3->getValue() > datavector[2])
		slider3->setValue(datavector[2]);
	slider4->setValue(slider4->getValue() + 0.02);
	if(slider4->getValue() > datavector[3])
		slider4->setValue(datavector[3]);

	barra1->getLabelWidget()->setLabel(tagvector[0]);
	barra2->getLabelWidget()->setLabel(tagvector[1]);
	barra3->getLabelWidget()->setLabel(tagvector[2]);
	barra4->getLabelWidget()->setLabel(tagvector[3]);

	slider1->getLabelWidget()->setLabel("");
	slider2->getLabelWidget()->setLabel("");
	slider3->getLabelWidget()->setLabel("");
	slider4->getLabelWidget()->setLabel("");
	
	barra1->setColorFill(colorvector[Nposicion]);
	barra2->setColorFill(colorvector[Nposicion + 1]);
	barra3->setColorFill(colorvector[Nposicion + 2]);
	barra4->setColorFill(colorvector[Nposicion + 3]);
	
	slider1->setColorFill(colorvector[Nposicion]);
	slider2->setColorFill(colorvector[Nposicion + 1]);
	slider3->setColorFill(colorvector[Nposicion + 2]);
	slider4->setColorFill(colorvector[Nposicion + 3]);
}


void testApp::keyPressed(int key)
{
	if(key == '1')
		Nbarra = 1;
	if(key == '2')
		Nbarra = 2;
	if(key == '3')
		Nbarra = 3;
	if(key == '4')
		Nbarra = 4;
	
	ofxUIRectangle* rect1 = NULL;
	ofxUIRectangle* rect2 = NULL;

	if(Nbarra == 1)
	{
		rect1 = barra1->getRect();
		rect2 = slider1->getRect();
	}
	if(Nbarra == 2)
	{
		rect1 = barra2->getRect();
		rect2 = slider2->getRect();
	}
	if(Nbarra == 3)
	{
		rect1 = barra3->getRect();
		rect2 = slider3->getRect();
	}
	if(Nbarra == 4)
	{
		rect1 = barra4->getRect();
		rect2 = slider4->getRect();
	}

	if(key == OF_KEY_LEFT)
	{
		rect1->setX(rect1->getX() - 5);
		rect2->setX(rect2->getX() - 5);
	}
	if(key == OF_KEY_RIGHT)
	{
		rect1->setX(rect1->getX() + 5);
		rect2->setX(rect2->getX() + 5);
	}
	if(key == OF_KEY_UP)
	{
		rect1->setY(rect1->getY() - 5);
		rect2->setY(rect2->getY() - 5);
	}
	if(key == OF_KEY_DOWN)
	{
		rect1->setY(rect1->getY() + 5);
		rect2->setY(rect2->getY() + 5);
	}
}
