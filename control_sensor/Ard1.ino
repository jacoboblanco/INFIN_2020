char comanda;
char entrada[8]; //array de entrada
int posicion=0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  lectura_array();
  delay(20);
  operacion();
  delay(500);
  
}

void operacion() {

  switch (comanda){
		case 'M':
			Serial.println ("S'executa la comanda M");
		break;
    
    	case 'S':
			Serial.println ("S'executa la comanda S");
		break;
    
    	case 'E':
			Serial.println ("S'executa la comanda E");
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
 
    while (Serial.available()>0) //Comprobamos si en el buffer hay datos
    {
    entrada[posicion]=Serial.read();  //Lee cada carácter uno por uno y se almacena en una variable
    posicion++;
    }
  Serial.println(entrada);  //Imprimimos en la consola el carácter recibido
  comanda=entrada[1];
  Serial.println(comanda);
}
