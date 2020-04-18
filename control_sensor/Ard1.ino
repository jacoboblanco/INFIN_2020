
char entrada[8]; //mensaje entrada
char salida[8];  //mensaje salida

// variables operacion() y lectura_array()
int posicion=0;
char comanda;

// variables comandaE() y comandaS()
int decenas;
int unidades;
int pin;
int on_off;

void setup(){
  Serial.begin(9600);
}

void loop(){
  lectura_array();
  delay(500);
  operacion();
  delay(2000);
}

void operacion() {
  
  Serial.println ("Seleccion de operacion");
  
  switch (comanda){
		case 'M':
			Serial.println ("S'executa la comanda M");
		break;
    
    	case 'S':
			Serial.println ("S'executa la comanda S");
        	comandaS();
    		Serial.println ("Finalitzat");
		break;
    
    	case 'E':
			Serial.println ("S'executa la comanda E");
    		comandaE();
    		Serial.println ("Finalitzat");
		break;
    
    	case 'C':
			Serial.println ("S'executa la comanda C");
		break;

		default:
			Serial.println ("Error");
		break;
  }
}


void lectura_array() {
 
  if(Serial.available()) //Nos dice si hay datos dentro del buffer
  {
    while (Serial.available()>0) //Comprobamos si en el buffer hay datos
    {
    entrada[posicion]=Serial.read();  //Lee cada carácter uno por uno y se almacena en una variable
    posicion++;
    }
  Serial.println(entrada);  //Imprimimos en la consola el carácter recibido
  comanda=entrada[1];
  Serial.println(comanda);
  }
}


void comandaE() {
  decenas = entrada[2];
  unidades = entrada[3];
  Serial.println (decenas);
  delay (2000);
  pin = decenas*10 + unidades;
  Serial.println (pin);
  
  on_off = digitalRead(pin);
  Serial.print ("estat: ");
  Serial.println (on_off);
  }


void comandaS() {
  decenas = entrada[2];
  unidades = entrada[3];
  on_off = entrada[4];
  pin = decenas*10 + unidades;
  Serial.print (pin);
  Serial.print ("   -   ");
  Serial.println (on_off);
  
  delay(100);
  
  if (on_off == 1) {
    digitalWrite (pin, 1);
  	Serial.print ("Pin");
    Serial.print (pin);
    Serial.println ("ON");
  }
  
  else if (on_off == 0) {
    digitalWrite (pin, 0);
  	Serial.print ("Pin");
    Serial.print (pin);
    Serial.println ("OFF");
  }
  
  else {
  	Serial.println ("ERROR");
  }
}
