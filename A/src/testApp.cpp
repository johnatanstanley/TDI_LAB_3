#include "testApp.h"
//#include "ofSerial.h"
#include <windows.h>
#include "SerialClass.h"	// Library described above

#define RECONNECT_TIME 400

Serial* SP;

int connectTime = 0;
int deltaTime = 0;
bool weConnected;
bool clickIn;


const int maxBarra = 4;

int Ngrafica = 1;
int Ntipo = 0;
int Nposicion = 0;
bool Zoom = false;

float* datavector;
ofColor colorvector[12];

ofxUILabel* titulo = NULL;
ofxUISlider* barra1 = NULL;
ofxUISlider* barra2 = NULL;
ofxUISlider* barra3 = NULL;
ofxUISlider* barra4 = NULL;


string testApp::nomArchivo(int grafica, int tipo)
{
	//1 = Defunciones 2 = Nacimientos 3 = Casamientos 4 = Multas
	string nomArch = std::to_string((int)grafica);
	std::string c = std::to_string((int)tipo);
	nomArch += '_' + c + ".csv";
	return nomArch;
}

float testApp::getValor(int grafica, int tipo, int index)
{
	float res = 0;
	ifstream archivo;
    string ruta = "../Datos/" + nomArchivo(grafica, tipo);
    archivo.open(ruta);
    string prueba = "";
    
	//descarto el titulo
	getline(archivo, prueba, ';');
	
	//descarto los datos del principio para mostrar los siguientes
	for (int i = 0; i < index; i++)
	{
		getline(archivo, prueba, ';');
		getline(archivo, prueba);
	}

	getline(archivo, prueba, ';');
	getline(archivo, prueba);
	std::string::size_type sz;
	res = std::stof (prueba, &sz);

	archivo.close();
	return res;
}

string testApp::getTag(int grafica, int tipo, int index)
{
	string res = "";
	
	ifstream archivo;
	string ruta = "../Datos/" + nomArchivo(grafica, tipo);
    archivo.open(ruta);
    string prueba = "";
    
	//obtengo el titulo
	getline(archivo, prueba, ';');
	titulo->setLabel(prueba);

	for (int i = 0; i < index; i++)
	{
		getline(archivo, prueba, ';');
		getline(archivo, prueba);
	}

	getline(archivo, prueba, ';');
	res = prueba;

	if (index == 0)
		res = res.substr(1);

	archivo.close();
	return res;
}

void testApp::setup()
{
	datavector = new float[4];
	clickIn = true;
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

	ofSetVerticalSync(true);
	weConnected = tcpClient.setup("127.0.0.1", 11999);
	tcpClient.setMessageDelimiter("\n");
	tcpClient.setVerbose(true);

	actualizarGraf = true;
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

	ofEnableSmoothing();

	float dimX = 80;
	float dimY = 450;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 800;//620-xInit;
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    gui->setColorBack(ofColor(60,150));
	
	titulo = new ofxUILabel("TIPO DE DATOS", OFX_UI_FONT_MEDIUM);
//    gui->addWidgetDown(new ofxUILabel("DATOS", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(titulo);

	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer(length-xInit, 1);
	
	vector<string> vnames; vnames.push_back("Defunciones"); vnames.push_back("Nacimientos"); vnames.push_back("Casamientos");vnames.push_back("Multas");
    ofxUIRadio *radio = gui->addRadio("VR", vnames, OFX_UI_ORIENTATION_HORIZONTAL);
    radio->activateToggle("SOCKS");

	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer(length-xInit, 1);

    barra1 = gui->addSlider("Barra1", 0, 1, 0.25, dimX, dimY);
	barra1->setColorFill(colorvector[0]);
	barra1->setColorBack(ofColor(60, 50));
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	barra2 = gui->addSlider("Barra2", 0, 1, 0.50, dimX, dimY);
	barra2->setColorFill(colorvector[1]);
	barra2->setColorBack(ofColor(60, 50));
	barra3 = gui->addSlider("Barra3", 0, 1, 0.75, dimX, dimY);
	barra3->setColorFill(colorvector[2]);
	barra3->setColorBack(ofColor(60, 50));
	barra4 = gui->addSlider("Barra4", 0, 1, 1.00, dimX, dimY);
	barra4->setColorFill(colorvector[3]);
	barra4->setColorBack(ofColor(60, 50));

	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer(length-xInit, 1);
	
    ofEnableSmoothing();
    ofSetFrameRate(30);
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);

	SP = new Serial("\\\\.\\COM18");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected\n");
}


void testApp::update()
{
	if(weConnected)
	{
		if(!tcpClient.send(msgTx) && !tcpClient.isConnected())
			weConnected = false;
		msgTx = "";
	}
	else
	{
		deltaTime = ofGetElapsedTimeMillis() - connectTime;
		if( deltaTime > 5000 )
		{
			weConnected = tcpClient.setup("127.0.0.1", 11999);
			connectTime = ofGetElapsedTimeMillis();
		}
	}

	if(actualizarGraf == true)
	{
		printf("entraaaa - ");
		float vmin = getValor(Ngrafica, Ntipo, 0), vmax = 0;
		for (int i = 0; (Zoom && i < 12) || (!Zoom && ((Ngrafica != 4 && i < 9) || (Ngrafica == 4 && i < 4))); i++)
		{
			float vval = getValor(Ngrafica, Ntipo, i);
			vmin = min(vmin, vval);
			vmax = max(vmax, vval);
		}

		float v1 = getValor(Ngrafica, Ntipo, Nposicion);
		float v2 = getValor(Ngrafica, Ntipo, Nposicion + 1);
		float v3 = getValor(Ngrafica, Ntipo, Nposicion + 2);
		float v4 = getValor(Ngrafica, Ntipo, Nposicion + 3);
		
		datavector[0] = (v1 - vmin / 1.1) / (vmax - vmin / 1.1);
		datavector[1] = (v2 - vmin / 1.1) / (vmax - vmin / 1.1);
		datavector[2] = (v3 - vmin / 1.1) / (vmax - vmin / 1.1);
		datavector[3] = (v4 - vmin / 1.1) / (vmax - vmin / 1.1);

		//Comunicación con arduino	
		std::string c1 = std::to_string((int)(datavector[0]*100));
		std::string c2 = std::to_string((int)(datavector[1]*100));
		std::string c3 = std::to_string((int)(datavector[2]*100));
		std::string c4 = std::to_string((int)(datavector[3]*100));
		std::string str = c1+","+c2+","+c3+","+c4;
		char* ca = new char[str.size()+1];
		std::copy(str.begin(), str.end(), ca);
		ca[str.size()] = '\0';
		unsigned char m_Test[256];
		strcpy( (char*) m_Test, ca );
	
		if(SP->IsConnected()){
			printf("We're connected\n");
			printf(ca);
			printf("\n");
			if (SP->WriteData(ca, str.size()))
				printf("enviado\n");
		}
		
		//Fin comunicación con arduino

		barra1->setValue(0);
		barra2->setValue(0);
		barra3->setValue(0);
		barra4->setValue(0);
		
		barra1->setColorFill(colorvector[Nposicion]);
		barra2->setColorFill(colorvector[Nposicion + 1]);
		barra3->setColorFill(colorvector[Nposicion + 2]);
		barra4->setColorFill(colorvector[Nposicion + 3]);

		barra1->getLabelWidget()->setLabel(getTag(Ngrafica, Ntipo, Nposicion));
		barra2->getLabelWidget()->setLabel(getTag(Ngrafica, Ntipo, Nposicion + 1));
		barra3->getLabelWidget()->setLabel(getTag(Ngrafica, Ntipo, Nposicion + 2));
		barra4->getLabelWidget()->setLabel(getTag(Ngrafica, Ntipo, Nposicion + 3));
		actualizarGraf = false;

		if(weConnected)
		{
			ostringstream sval;
			sval << v1 << "-" << v2 << "-" << v3 << "-" << v4 << "-" << datavector[0] << "-" << datavector[1] << "-" << datavector[2] << "-" << datavector[3] << "-" << Nposicion << '\n';
			msgTx = titulo->getLabel() + "-" + barra1->getLabelWidget()->getLabel() + "-" + 
				barra2->getLabelWidget()->getLabel() + "-" + barra3->getLabelWidget()->getLabel() + "-" + 
				barra4->getLabelWidget()->getLabel() + "-" + sval.str();
		}
	}
	else
	{
		barra1->setValue(barra1->getValue() + 0.05);
		if(barra1->getValue() > datavector[0])
			barra1->setValue(datavector[0]);
		barra2->setValue(barra2->getValue() + 0.05);
		if(barra2->getValue() > datavector[1])
			barra2->setValue(datavector[1]);
		barra3->setValue(barra3->getValue() + 0.05);
		if(barra3->getValue() > datavector[2])
			barra3->setValue(datavector[2]);
		barra4->setValue(barra4->getValue() + 0.05);
		if(barra4->getValue() > datavector[3])
			barra4->setValue(datavector[3]);
	}
	char data [256] = "";
	SP->ReadData(data,256);
	printf(data);
}

void testApp::draw()
{

}

void testApp::keyPressed(int key)
{
	/*if(weConnected)
	{
		msgTx = "x";
		msgTx.append(1, (char) key);
		msgTx.append(1, (char) '\n');
	}*/

	if (key == 'a' && Nposicion > 0)
	{
		Nposicion--;
		actualizarGraf = true;
	}
	else if (key == 'd' && ((Zoom && Nposicion < 12 - maxBarra) || (!Zoom && Ngrafica != 4 && Nposicion < 9 - maxBarra)))
	{
		Nposicion++;
		actualizarGraf = true;
	}
	else if (key == 'w' && Zoom)
	{
		Ntipo = 0;
		Nposicion = 0;
		Zoom = false;
		actualizarGraf = true;
	}
	else if (key == 's' && !Zoom)
	{
		Ntipo = Nposicion + 1;
		Nposicion = 0;
		Zoom = true;
		actualizarGraf = true;
	}
	else if (key == 'r')
	{
		//Comunicación con arduino	
		std::string str = "-1,-1,-1,-1";
		char* ca = new char[str.size()+1];
		std::copy(str.begin(), str.end(), ca);
		ca[str.size()] = '\0';
		unsigned char m_Test[256];
		strcpy( (char*) m_Test, ca );
	
		if(SP->IsConnected()){
			printf("We're connected\n");
			printf(ca);
			printf("\n");
			if (SP->WriteData(ca, 256))
				printf("enviado\n");
		}
		//Fin comunicación con arduino
		
	}
}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::guiEvent(ofxUIEventArgs &e)
{
	//printf("GUI/n");
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
    if(kind == OFX_UI_WIDGET_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
        cout << name << "\t value: " << toggle->getValue() << endl;             
    }

	if(name == "Defunciones")
	{
		Ngrafica = 1;
		Ntipo = 0;
		Nposicion = 0;
		Zoom = false;
		actualizarGraf = true;
	}
	else if(name == "Nacimientos")
	{
		Ngrafica = 2;
		Ntipo = 0;
		Nposicion = 0;
		Zoom = false;
		actualizarGraf = true;
	}
	else if(name == "Casamientos")
	{
		Ngrafica = 3;
		Ntipo = 0;
		Nposicion = 0;
		Zoom = false;
		actualizarGraf = true;
	}
	else if(name == "Multas")
	{
		Ngrafica = 4;
		Ntipo = 0;
		Nposicion = 0;
		Zoom = false;
		actualizarGraf = true;
	}
	else if (clickIn){
		clickIn = false;
	if(name == "Barra1")
	{
		Ntipo = Nposicion + 1;
		Nposicion = 0;
		Zoom = true;
		actualizarGraf = true;
	}
	else if(name == "Barra2")
	{
		Ntipo = Nposicion + 2;
		Nposicion = 0;
		Zoom = true;
		actualizarGraf = true;
	}
	else if(name == "Barra3")
	{
		Ntipo = Nposicion + 3;
		Nposicion = 0;
		Zoom = true;
		actualizarGraf = true;
	}
	else if(name == "Barra4")
	{
		Ntipo = Nposicion + 4;
		Nposicion = 0;
		Zoom = true;
		actualizarGraf = true;
	}
}
	else{
		clickIn = true;
	}
}
