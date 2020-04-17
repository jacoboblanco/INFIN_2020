char entrada[8]; //array de entrada
char salida [8]; //array de salida

char comanda;

int posicion=0; //posición de lectura array entrada
int PM=0; //PM=0 Paro, PM=1 Marcha
int v; //parametro v array entrada
float mostreo; //freq. de mostreo del sensor

void setup() {
  Serial.begin(9600);


void loop() {    
}


void lectura_array() {
 
  if(Serial.available()) { //Nos dice si hay datos dentro del buffer
    memset(entrada, 0,sizeof(entrada)); //memset borra el contenido del array  "cadena" desde la posición 0 hasta el final sizeof
 
    while(Serial.available()>0) //Mientras haya datos en el buffer ejecuta la función
    {
      delay(5); //Poner un pequeño delay para mejorar la recepción de datos
      entrada[posicion]=Serial.read();//Lee un carácter del string "cadena" de la "posicion", luego lee el siguiente carácter con "posicion++"
      posicion++;
    }
  }
}

void array_salida() {
 	salida[0] = 'A';
  	salida[1] = comanda;
}

void operacion() {
 	
  switch (entrada[1]) {
    case 'M':
      comanda = 'M';
      parada_marxa();
      break;
    
    case 'S':
      comanda = 'S';
      sort_digital();
      break;
    
    case 'E':
      comanda = 'E';
      ent_digital();
      break;
    
    case 'C':
      comanda = 'C';
      ultima_m();
      break;
  }
}

void parada_marxa() {
  
  pm = entrada[2];
  
  switch (PM) {
    
    case 0:
     if (v=0) {
      errorparametros();
      }
    else if (v=1) {
      PM = 1;
      mostreo = 
      
      
      
    break;
    
    case 1:
    
    break;
  }
}
