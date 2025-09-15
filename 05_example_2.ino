#define PIN_LED 7
unsigned int count, toggle;
  
void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
}

void loop() {
  digitalWrite(PIN_LED,0);
  delay(1000);
  for (int i=0; i<5; i++){
    digitalWrite(PIN_LED,1);
    delay(100);
    digitalWrite(PIN_LED,0);
    delay(100);
  }
  while(1){
    digitalWrite(PIN_LED,1);
  }
}
