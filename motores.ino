#include <ax12.h>

//cantidad de motores
const int cantMotores = 4;


// Manejadores de los motores
AX12 motor[cantMotores];

// Recogida (en cm) por vuelta
int perimetro = 14;

// Velocidad de movimiento
int vel = 440;

// Tiempo por vuelta de cada motor
int tiempoPorVuelta[cantMotores];

// Posicion actual de cada motor
int pos[cantMotores];

// Lectura desde el serial para cada motor
int lectura[cantMotores];

// Tiempo que se va a estar moviendo cada motor
int aMover[cantMotores];

// Direccion de movimiento de cada motor, 1 = horario, -1 = anti-horario. Si la dir = 0 se resetea el motor
int dir[cantMotores];

//Recibe los cm y el id del motor y retorna el delay necesario para ese motor
int delayAMover(int cm, int idMotor){
	// Hay que hacer la cuenta de esta forma porque sino da mal
        int valor  = (cm-pos[idMotor])*(tiempoPorVuelta[idMotor]/perimetro);
	pos[idMotor] = cm;
	return valor;
}

// Se calculan los tiempos de movimiento para cada motor, se setea la direccion adecuada
void calcularTiemposMovimiento(){
	for (int i=0; i < cantMotores; i++){
		// Si es -1 entonces se setea el dir en 0
		if(lectura[i] == -1){
			dir[i] = 0;
			aMover[i] = 0;
		}
		// Si es diferente de -1 se calcula el tiempo, se lo deja positivo y se setea la direccion
		else{
			int aux = delayAMover(lectura[i],i);
			if (aux > 0){
				dir[i] = 1;
				aMover[i] = aux;
			}
			else{
				dir[i] = -1;
				aMover[i] = aux * -1;
			}

		}
	}
}

// Se resetean los motores cuya dir sea 0
void resetearMotores(){
        int reseteo = 0;
	for (int i=0; i < cantMotores; i++){
		// Si es 0 entonces se resetea el motor
		if(dir[i] == 0){
			// Se pasa a modo angulo, se mueve a 0 y se vuelve al modo rotacion
			motor[i].setEndlessTurnMode(false);
			pos[i] = 0;
			motor[i].setPosVel(pos[i], 500);
			dir[i] = 1;
            reseteo = 1;
		}
	}
        if (reseteo == 1){
            delay(1000);
            for(int i = 0; i < cantMotores; i++){
              motor[i].setVel(0); 
              motor[i].setEndlessTurnMode(true);
            }
        }
}

// Se obtiene el menor delay diferente de 0, retorna 0 solo si todos son 0
int obtenerMenor(){
	// Es bastante codigo para que sea mas eficiente y no iterar de mas ni tener que hacer comparaciones de mas
	int menor;
	if (aMover[0] != 0){
		menor = aMover[0];
		for (int i=1; i < cantMotores; i++){
			if ((aMover[i] != 0) && (aMover[i] < menor)){
				menor = aMover[i];
			}
		}
		return menor;
	}
	else if (aMover[1] != 0){
		menor = aMover[1];
		for (int i=2; i < cantMotores; i++){
			if ((aMover[i] != 0) && (aMover[i] < menor)){
				menor = aMover[i];
			}
		}
		return menor;		
	}
	else if (aMover[2] != 0){
		menor = aMover[2];
		if ((aMover[3] != 0) &&(aMover[3] < menor)){
			menor = aMover[3];
		}
		return menor;
	}
	else{
		return aMover[3];
	}
}

// Se mueven los motores a las posiciones adecuadas, se mueven a la vez
void moverMotores(){
      // Serial.println("muevo motores");
	// Se obtiene el menor delay distinto de 0
	int menor = obtenerMenor();
	// Se setean las velocidades de los que tengan que moverse
	for (int i=0; i < cantMotores; i++){
  Serial.print("ID ");
  Serial.print(i);
   Serial.print(" - Dir ");
  Serial.print(dir[i]);
    Serial.print(" aMover ");
  Serial.println(aMover[i]);
		if(aMover[i] != 0){
			motor[i].endlessTurn(vel * dir[i]);
		}		
	}
	// Mientras falte mover alguno
	while(menor != 0){
		delay(menor);	
		// Se actualizan los delay y se frenan los que no tengan que moverse mas
		for (int i=0; i < cantMotores; i++){
			// Si aMover ya era 0 entonces ya se tenia velocidad 0
			if(aMover[i] != 0){
				// Se actualiza el delay y si queda en 0 se frena el motor
				aMover[i] = aMover[i] - menor;
				if(aMover[i] == 0){
					motor[i].endlessTurn(0);
				}
			}
		}	
		menor = obtenerMenor();
	}
	// Se frenan todos los motores (deberian estar todos frenados ya)
	for (int i=0; i < cantMotores; i++){
		motor[i].endlessTurn(0);
	}
}

// Setup
void setup() { 
		
	// Inicializacion de los motores
	motor[0] = AX12(1);
	motor[1] = AX12(2);
	motor[2] = AX12(4);
	motor[3] = AX12(8);

	// Inicializacion velocidades, Serial a 115,2 Kb/s, AX12 baudrate 1 Mb/s
	//Serial.begin (115200);
	Serial.begin (9600);
	AX12::init (1000000);
	
	// Se setean los tiempos vuelta - Medida empirica
	tiempoPorVuelta[0] = 2530;
	tiempoPorVuelta[1] = 2530;
	tiempoPorVuelta[2] = 2530;
	tiempoPorVuelta[3] = 2530;
	
	for (int i=0; i < cantMotores; i++){
	
		// Se setean los id a los motores. El retorno queda en NONE por defecto
		// no se setan por aca
		
		// Se setean las posiciones iniciales

		pos[i] = 0;
		
		// Se setean los motores en modo angulo para posicionarlos de forma inicial
		motor[i].setEndlessTurnMode(false);
		
		// Se setean las direcciones de movimiento en 1
		dir[i] = 1;
		
		// Se setan las velocidades y posiciones iniciales
		motor[i].setPosVel(pos[i], 300);
	}
	
	// Se espera a que se acomoden los motores
	delay(1000);
	
	for (int i=0; i < cantMotores; i++){
	
		// Se detienen los motores

		motor[i].setVel(0);
		
		// Se setean los motores en modo de rotacion continua
		motor[i].setEndlessTurnMode(true);		
	}
	
	Serial.println("Fin del setup");
}

// Loop principal
void loop() {
    Serial.println("--- LOOP ---");
	// Si hay datos para leer
	if (Serial.available() > 1){
	//	Serial.println("Hay datos en el serial");
		// Se asume que los datos en el serial vienen de la forma pos1,pos2,pos3,pos4, cada pos es la altura que debe estar, si pos = -1 se resetea el motor
		
          //      for (int i=0; i < cantMotores; i++){
	//	        Serial.println(lectura[i]);                     
	//	}
// Serial.println("luego");    
		for (int i=0; i < cantMotores; i++){
			int inbyte =   Serial.parseInt();		      
                        if (inbyte != 0)
			      lectura[i] = inbyte;
                        Serial.println(inbyte);
		}

		// Se calculan (y setean) los tiempos de movimiento y las direcciones de movimiento
		calcularTiemposMovimiento();
		
		// Se resetean los motores que se deban resetear (es decir, los que recibieron -1)
		resetearMotores();
		
		// Se mueven los motores a las posiciones adecuadas
		moverMotores();
	}
     
   // Serial.println("Fin loop principal.");
	
	// Se detienen todos los motores
	for (int i=0; i < cantMotores; i++){
		motor[i].endlessTurn(0);
	}
	
	// Delay de espera para no loopear como loco cuando no hay datos en el serial
    delay(500);
}
