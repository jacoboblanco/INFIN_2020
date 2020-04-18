char entrada[8] = {'A', 'S', 0, 8, 'Z'};
int decenas;
int unidades;
int pin;
int on_off;

void setup() {
  Serial.begin(9600);
}

void loop() {
  decenas = entrada[2];
  unidades = entrada[3];
  pin = decenas*10 + unidades;
  Serial.println (pin);
  
  on_off = digitalRead(pin);
  Serial.print ("estat: ");
  Serial.println (on_off);
  
  delay(1000);
  }
