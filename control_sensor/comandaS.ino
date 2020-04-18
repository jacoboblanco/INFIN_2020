char entrada[8] = {'A', 'S', 1, 3, 1, 'Z'};
int decenas;
int unidades;
int pin_out;
int on_off;

void setup() {
  Serial.begin(9600);
}

void loop() {
  decenas = entrada[2];
  unidades = entrada[3];
  on_off = entrada[4];
  pin_out = decenas*10 + unidades;
  Serial.println (pin_out);
  Serial.println (on_off);
  
  delay(100);
  
  if (on_off == 1) {
    digitalWrite (pin_out, 1);
  	Serial.print ("Pin");
    Serial.print (pin_out);
    Serial.println ("ON");
  }
  
  else if (on_off == 0) {
    digitalWrite (pin_out, 0);
  	Serial.print ("Pin");
    Serial.print (pin_out);
    Serial.println ("OFF");
  }
  
  else {
  	Serial.println ("ERROR");
  }
