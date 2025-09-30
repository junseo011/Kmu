int led = 7;         // the PWM pin the LED is attached to
int br = 0;  // how bright the LED is
int p = 10000;  // how many points to fade the LED by
int fade = 1;
int wait = 0;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  set_period(p);
}

void set_period(int period){ // period: 100 to 10000 (unit: us)
  wait = period/200;

void set_duty(int duty){ // duty: 0 to 100 (unit: %)
    int count = 0;
    while(count!=wait){
    digitalWrite(led, HIGH);
    delayMicroseconds(1*duty);
    digitalWrite(led, LOW);
    delayMicroseconds(100-duty*1);
    count++;
    }
    return;
}


void loop() {
  set_duty(br);
  br = br + fade;
    if (br <= 0 || br >= 100) {
      fade = -fade;
    }
}

