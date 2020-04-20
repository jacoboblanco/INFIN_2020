#define size 100
#define ledPin 13

int tiempo_muestreo;
int posicion=0;
int valor_MarchaParo=0;
float muestras[size];
char comanda[size]; 
int t=0;

//FUNCION PARA LED PARPADEANTE



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

//INTERRUPCION 

ISR(TIMER1_COMPA_vect){

       int lectura = analogRead(A0);
       int i=0;             /*Contador de numero de interrupciones para 
                            que coincida con el tiempo que se le asigna al tiempo de muestreo*/
 
   if (posicion>size) posicion=0;      
   if (valor_MarchaParo==1) i=i+1;  

   if ((i==tiempo_muestreo) && (valor_MarchaParo==1)) {                          /* Es realitza la adquisició si i==tmostreig i seleccionem marxa d'adquisisció.*/
     
        muestras[posicion] = lectura; 
        digitalWrite(ledPin,HIGH);
        Serial.print("\nMuestra añadida: ");
        Serial.println(muestras[posicion]);   
        Serial.print("\n");
   }    
   posicion++;
   i=0;  
}

void loop(){
    
    char salida_de_comanda_C[size];

    entrada_de_comanda();

  switch (comanda[1]){
		case 'M':
			Serial.println ("S'executa la comanda M");
            valor_MarchaParo=comanda[2]-48;
            tiempo_muestreo=((comanda[3]-48)*10+(comanda[4]-48))*2;
            Serial.print("AM0Z\n");
            memset(comanda, 0, sizeof(comanda));
            entrada_de_comanda();
		break;
        
    	case 'S':
			Serial.println ("S'executa la comanda S");
        	comandaS();
            Serial.print("AS0Z\n");
    		Serial.println ("\nFinalitzat\n");
            memset(comanda, 0, sizeof(comanda));
            entrada_de_comanda();
		break;
    
    	case 'E':
			Serial.println ("S'executa la comanda E");
    		comandaE();
            Serial.print("AE0Z\n");
    		Serial.println ("\nFinalitzat\n");
            memset(comanda, 0, sizeof(comanda));
            entrada_de_comanda();
		break;

    	case 'C':
            Serial.println ("S'executa la comanda E");
    	    sprintf(salida_de_comanda_C, "AC0%dZ",muestras[posicion]);
		    Serial.print(salida_de_comanda_C);
            Serial.println ("\nS'executa la comanda C");
            memset(comanda, 0, sizeof(comanda));
            entrada_de_comanda();
        break;
  }
}



void entrada_de_comanda() {
 
        if(Serial.available()){
            
            while (Serial.available()>0){
               comanda[t]=Serial.read(); 
               t++;
            } 
        }
}

void comandaE() {

    int decenas;
    int unidades;
    int pin;
    int valorPin;

        decenas = comanda[2]-48;
        unidades = comanda[3]-48;
  
        delay (1000);
  
        pin = decenas*10 + unidades;
        Serial.println (pin);
        
        delay(1000);
        
        valorPin = digitalRead(pin);
        Serial.print ("\nestat: \n");
        Serial.println (valorPin);
  }


void comandaS() {
   
    int decenas;
    int unidades;
    int pin;
    int valorPin;

    decenas = comanda[2]-48;
    unidades = comanda[3]-48;
    valorPin = comanda[4]-48;
    pin = decenas*10 + unidades;
  
    delay(100);
  
        if (valorPin == 1) {          
            digitalWrite (pin, 1);
        }
         else if (valorPin == 0) {
             digitalWrite (pin, 0);
        } 
}

void error_protocolo(){
    
    int i=0;
 
 switch (comanda[1]) {
   case 'E':
     if (comanda[4]!='Z') {           //ERROR_protocol: la 'Z' no està en la posició correcta.
       Serial.println ("AE10Z\n");    //resposta ERROR_protocol operació 'E'.
     }
     else if (comanda[i]){            //ERROR_protocol: es rep una segona 'A' abans 'Z' 
       for (i=1; i<4; i++){
         if (comanda[i]=='A'){
    	    Serial.println (":)");      //es descarten bytes rebuts i no es respon res. 
         }  
         else {}
       }
     }
     else {    
       comandaE();
     }
   break;
    
   case 'S':
     if (comanda[5]!='Z') {           //ERROR_protocol: la 'Z' no està en la posició correcta.
       Serial.println ("AS1Z\n");     //resposta ERROR_protocol operació 'S'.
     }
     else if (comanda[i]){            //ERROR_protocol: es rep una segona 'A' abans 'Z' 
       for (i=1; i<5; i++){
         if (comanda[i]=='A'){
    	  Serial.println (":)");      //es descarten bytes rebuts i no es respon res. 
         }  
         else {}
       }
     }
     else {  
       comandaS();
     }
    break;
    
    default:
      Serial.println ("");
 }     
}
