// DEFINIMOS EL TAMAÑO DE LOS ARRAYS Y EL PIN DEL LED

#define SIZE 100
#define ledPin 13

//DEFINIMOS LAS VARIABLES GLOBALES QUE SE USARAN TANTO EN LA INTERRUPCION Y EN LOOP

int tiempo_muestreo;
int tiempo_entrado_usuario;
int posicion=0;
int valor_MarchaParo=0;
int lectura;
char entrada_consola[SIZE]; 
int t=0;
int i=0;

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

//PROGRAMA QUE EJECUTARA LA INTERRUPCION

ISR(TIMER1_COMPA_vect){
    
        posicion++;       
    
        if (posicion>=SIZE) posicion=0;      
   
        if (valor_MarchaParo==1) i=i+1;  

        if ((i>=tiempo_muestreo) && (valor_MarchaParo==1)) {                        
        
                lectura = analogRead(A0);

                digitalWrite(ledPin, digitalRead(ledPin)^1);
                
                i=0;     
        } 
}


void loop(){

    if(lectura_array()>=1) {
  
        casos(entrada_consola[1]);
    }
}

//FUNCION PARA LEER LO QUE ENTRA POR LA TERMINAL

int lectura_array() { 

  int r=0;
  
        if(Serial.available()>0) { 
             
            r = Serial.readBytesUntil('\n',entrada_consola,SIZE); 
        }

  return r; 

}

//FUNCION DONDE DEPENDIENDO DE LA ENTRADA ELEGIRA QUE FUNCION EJECUTAR

void casos(char c){

    switch (c){

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

/*****************************FUNCIONES DE LAS DIFERENTES COMANDAS***********************/

void comandaM(){
 
  tiempo_entrado_usuario=((entrada_consola[3]-'0')*10+(entrada_consola[4]-'0'));

     if (entrada_consola[5]!='Z') {           // ERROR_protocol: la 'Z' no està en la posició correcta.
              Serial.print("AM1Z\n");       // Missatge ERROR 1 per la operació 'M'.
        }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' || (entrada_consola[3])=='A' || (entrada_consola[4])=='A'){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
              Serial.println (" ");         // es descarten bytes rebuts i no es respon res.
        }
       
        else if (((valor_MarchaParo == 0) && (entrada_consola[2] == '0')) || (((valor_MarchaParo == 1) && (entrada_consola[2] == '1')) && ((tiempo_entrado_usuario*2) == tiempo_muestreo )) || ((entrada_consola[2]-'0') < 0 ) || ((entrada_consola[2]-'0') > 1 )  || (tiempo_entrado_usuario < 0) || (tiempo_entrado_usuario > 20)) 
        {
              Serial.print("AM2Z\n");        /*ERROR_parametres: siempre que pida el mismo modo de funcionamiento que el que se esta en el 
                                             momento y siempre que el tiempo o el valor del modo que entren por la terminal 
                                             no este entre los valores  especificados.*/
        }
        
        else{

            valor_MarchaParo=entrada_consola[2]-'0';
            tiempo_muestreo=((entrada_consola[3]-'0')*10+(entrada_consola[4]-'0'))*2;  //SE MULTIPLICA POR 2 PARA QUE COINCIDA EN LA INTERRUPCION Y PODER ENTRAR EN SEGUNDOS DESDE LA CONSOLA

            Serial.print("AM0Z\n");
        }
}


void comandaE() {  

    int pin;
    bool ValorEntrada;
    ValorEntrada=digitalRead(pin);  
  
    pin = (entrada_consola[2]-'0')*10+(entrada_consola[3]-'0');           
    pinMode(pin, INPUT); 
                  
     
  
         if (entrada_consola[4]!='Z') {     // ERROR_protocol: la 'Z' no està en la posició correcta.
                Serial.print("AE1");        // Missatge ERROR 1 per la operació 'E'.
                Serial.print(ValorEntrada);
                Serial.print("Z\n");
        }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' || (entrada_consola[3])=='A'){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
                Serial.println (" ");      // es descarten bytes rebuts i no es respon res.
        }
        
        else if (pin<0 || pin>13) {         // ERROR_parametres:sempre que el número d'entrada digital nn no sigui correcte.
                Serial.print("AE2");        // Missatge ERROR 2 per la operació 'E'.
                Serial.print(ValorEntrada);
                Serial.print("Z\n");
        }

        else {       
                ValorEntrada=digitalRead(pin);

            Serial.print("AE0");         // Missatge de OK. 
            Serial.print(ValorEntrada);
            Serial.print("Z\n");
        }
}


void comandaS(){
    
    int pin;
    bool ValorSortida;

      ValorSortida = (entrada_consola[4]-'0');
      pin = (entrada_consola[2]-'0')*10+(entrada_consola[3]-'0');  

        if (entrada_consola[5]!='Z') {         // ERROR_protocol: la 'Z' no està en la posició correcta.
              Serial.print("AS1Z\n");        // Missatge ERROR 1 per la operació 'S'.;
        }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' || (entrada_consola[3])=='A' || (entrada_consola[4])=='A'){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
                Serial.println (" ");          // es descarten bytes rebuts i no es respon res.
        }

        else if (pin<0 || pin>13 || ValorSortida<0 || ValorSortida>1){ //ERROR_parametres:empre que el número de sortida digital nn no sigui correcte. Sempre que el valor v no sigui correcte.
              Serial.print("AS2Z\n");        // Missatge ERROR 2 per la operació 'S'.;
        }
   
        else {
                digitalWrite(pin, ValorSortida);         
              Serial.print("AS0Z\n");         // Missatge de OK.
            }
}


void comandaC(){

    char salida_de_comanda_C[10];

        if (entrada_consola[2]!='Z') {         // ERROR_protocol: la 'Z' no està en la posició correcta.
            Serial.print("AC19999Z\n");        // Missatge ERROR 1 per la operació 'S'.;
        }
  
        else if ((entrada_consola[1])=='A' || (entrada_consola[2])=='A' ){ // ERROR_protocol: es rep una segona 'A' abans 'Z' 
            Serial.println (" ");              // es descarten bytes rebuts i no es respon res.
        }   

        else if (valor_MarchaParo == 0) {      // ERROR_parametres: siempre que se pida y estemos en paro.
            Serial.print("AC2Z\n");
        }
           
        else{
            sprintf(salida_de_comanda_C, "AC0%04dZ\n", lectura);
            Serial.print(salida_de_comanda_C);
        }
}
