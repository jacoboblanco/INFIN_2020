#define size 100
#define ledPin 13

int tiempo_muestreo;
int posicion=0;
int valor_MarchaParo=0;
int muestras[size];
char entrada_consola[size]; 
int t=0;
int i=0;
int ultimo_valor=0; 

void setup(){ 

    noInterrupts();           /* disable all interrupts                                                        */
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A = 31250;            /* compare match register 16MHz/256/2Hz (SALTA LA INTERRUPCIÓ CADA 500ms)        */
    TCCR1B |= (1 << WGM12);   /* CTC mode                                                                      */
    TCCR1B |= (1 << CS12);    /* 256 prescaler                                                                 */
    TIMSK1 |= (1 << OCIE1A);  /* enable timer compare interrupt                                                */
    interrupts();             /* enable all interrupts                                                         */
    
    pinMode(ledPin,OUTPUT);
    
    Serial.begin(9600);       
}


ISR(TIMER1_COMPA_vect){
    
    int lectura = analogRead(A0);
                 
   if (posicion>size) posicion=0;      
   
   if (valor_MarchaParo==1) i=i+1;  

   if ((i==tiempo_muestreo) && (valor_MarchaParo==1)) {                        
        
        muestras[posicion] = lectura;

        digitalWrite(ledPin, digitalRead(ledPin)^1);
        Serial.print("\nMuestra añadida: ");
        Serial.println(muestras[posicion]);   
        Serial.print("\n");
        muestras[posicion]=ultimo_valor;
        i=0;  
   }
 posicion++;    
}


void loop(){

    if(lectura_array()==1) {
  
    casos(entrada_consola[1]);
    
  }
    delay(2000);
}

int lectura_array() { 
  int r=0;
  
  if(Serial.available()>0) { 
    r=1;
    for (t=0; t<=8; t++)  {
    	entrada_consola[t]=Serial.read();  
    }
  t=0;
  }
  return r; 
}

void casos(char c){

    switch (c)
    {
    case 'M':
        comandaM();
        break;
    
    case 'E':
        comandaE();
        break;
    
    case 'S':
        comandaS();
        break;
    
    case 'C':
        comandaC();
        break;
    }

}


void comandaM(){
 
     if (entrada_consola[5]!='Z') {           // ERROR_protocol: la 'Z' no està en la posició correcta.
            	Serial.print("AM1Z\n");        // Missatge ERROR 1 per la operació 'M'.
        }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' || (entrada_consola[3])=='A'){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
                Serial.println (" ");      // es descarten bytes rebuts i no es respon res.
        }
        
            valor_MarchaParo=entrada_consola[2]-48;
            tiempo_muestreo=((entrada_consola[3]-48)*10+(entrada_consola[4]-48))*2;
            Serial.print("AM0Z\n");
}

void comandaE() {  

    int pin=0;
    bool ValorEntrada=0;	
  
  pin = (entrada_consola[2]-48)*10+(entrada_consola[3]-48);           
  pinMode(pin, INPUT);                   
  ValorEntrada=digitalRead(pin); 
  
         if (entrada_consola[4]!='Z') {           // ERROR_protocol: la 'Z' no està en la posició correcta.
            	Serial.print("AE1");        // Missatge ERROR 1 per la operació 'E'.
                Serial.print(ValorEntrada);
                Serial.print("Z\n");
        }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' || (entrada_consola[3])=='A'){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
                Serial.println (" ");      // es descarten bytes rebuts i no es respon res.
        }
        
        else {          
		Serial.print("AE0");         // Missatge de OK. 
		Serial.print(ValorEntrada);
		Serial.print("Z\n");
        }
}


void comandaS(){
    
    int pin=0;
    bool ValorSortida=0;
  
   if (entrada_consola[5]!='Z') {           // ERROR_protocol: la 'Z' no està en la posició correcta.
       	Serial.print("AS1Z\n");        // Missatge ERROR 1 per la operació 'S'.;
   }
  
   else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' || (entrada_consola[3])=='A' || (entrada_consola[4])=='A'){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
        Serial.println (" ");      // es descarten bytes rebuts i no es respon res.
   }

   else {
     	ValorSortida = (entrada_consola[4]-48);
  		pin = (entrada_consola[2]-48)*10+(entrada_consola[3]-48);  
        digitalWrite(pin, ValorSortida);         
  		Serial.print("AS0Z\n");         // Missatge de OK.
   }

}


void comandaC(){

    char salida_de_comanda_C[size];

        if (entrada_consola[2]!='Z') {           // ERROR_protocol: la 'Z' no està en la posició correcta.
       	    Serial.print("AC10000Z\n");        // Missatge ERROR 1 per la operació 'S'.;
         }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' ){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
            Serial.println (" ");      // es descarten bytes rebuts i no es respon res.
          }      
        
        sprintf(salida_de_comanda_C, "AC0%04dZ", ultimo_valor);
		Serial.print(salida_de_comanda_C);

}
