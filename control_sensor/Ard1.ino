int contador;

void setup(){
  
  contador=1;
  Serial.begin(9600);
  Serial.println("*****************");
  Serial.println("ENSAYO RESOLUCION");
  Serial.println("*****************");  
}

void loop(){

  muestreo();
  contador++;
}

void muestreo(){

    int A=analogRead(A0);
    float B= float(A)*5/1023;
    Serial.print(" Muestra: ");
    Serial.print(contador);
    Serial.print(" Medida Digital: ");
    Serial.print(A);
    Serial.print(" Medida Analogica: ");
    Serial.println(B, 5);

delay(1000);
  }
