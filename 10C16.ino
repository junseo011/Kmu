#include <Servo.h>
#include <math.h>

#define PIN_TRIG  12    // 초음파 센서 TRIGGER
#define PIN_ECHO  13    // 초음파 센서 ECHO
#define PIN_SERVO 10    // 서보모터 제어 핀
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)

Servo myServo;

bool carDetected = false;      // 차량 감지 여부
float distance = 0.0;          // 초음파 거리
unsigned long lastCheckTime = 0;
const unsigned long CHECK_INTERVAL = 200; // 거리 측정 주기 (ms)

// sigmoid 함수 (부드러운 S-커브)
float sigmoid(float t) {
  return 1.0 / (1.0 + exp(-12 * (t - 0.5)));
}

float cosine(float t) {
  if (t < 0.0) t = 0.0;
  if (t > 1.0) t = 1.0;
  return 0.5 - 0.5 * cos(PI * t);
}


void liftGateSmooth() {
  for (int i = 0; i <= 100; i++) {
    float t = i / 100.0;
    float s = cosine(t);
    int angle = (int)(s * 90);
    myServo.write(angle);
    delay(15);
  }
}

void lowerGateSmooth() {
  for (int i = 100; i >= 0; i--) {
    float t = i / 100.0;
    float s = cosine(t);
    int angle = (int)(s * 90);
    myServo.write(angle);
    delay(15);
  }
}

void setup() {
  Serial.begin(57600);
  myServo.attach(PIN_SERVO);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  delay(500);

  myServo.write(0);
}

void loop() {
  float dist_raw;

  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  // 차량 접근 감지 (20cm 이하)
  if (dist_raw < 100 && !carDetected) {
    liftGateSmooth();
    carDetected = true;
  }
  // 차량 통과 후 복귀 (30cm 이상)
  else if (dist_raw > 250 && carDetected) {
    lowerGateSmooth();
    carDetected = false;
  }
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
