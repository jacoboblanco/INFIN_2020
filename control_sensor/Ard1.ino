char entrada[8]; //array de entrada
char salida [8]; //array de salida

char comanda;

int posicion=0; //posición de lectura array entrada
int PM=0; //PM=0 Paro, PM=1 Marcha
int pm;
int v; //parametro v array entrada
float mostreo; //freq. de mostreo del sensor

void setup() {
  Serial.begin(9600);
}


void loop() {
  lectura_array();
}


void lectura_array() {
 
    if(Serial.available()>0) //Comprobamos si en el buffer hay datos
    {
    char entrada=Serial.read();  //Lee cada carácter uno por uno y se almacena en una variable
 	Serial.print(entrada);  //Imprimimos en la consola el carácter recibido
    }
}

void array_salida() {
   salida[0] = 'A';
    salida[1] = comanda;
}

/*void operacion() {
  
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
      //mostreo = 
      
      
      
    break;
    
    case 1:
    
    break;
  }
}
  
  void sort_digital() {
  }
  
  void ent_digital() {
  }
  
  void ultima_m() {
  }
  
  void parada_marxa() {
  }
  
  void errorparametros() {
  }
  
  */
