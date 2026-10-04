
#include <ESP32Servo.h>

// =====================================
// GPIO 설정
// =====================================

#define POT_PIN    34    // 가변저항 가운데 단자
#define SERVO_PIN  25    // S3003 Signal

// =====================================
// 서보 설정
// =====================================

Servo myServo;

// 서보 목표 각도
int servoAngle = 90;

// PWM 펄스 범위 (마이크로초)
const int SERVO_MIN_US = 550;   //1000;
const int SERVO_MAX_US = 2450;  //2000;

// 업데이트 간격 (ms)
const int UPDATE_INTERVAL = 15;

unsigned long lastUpdate = 0;

void setup() {

  Serial.begin(115200);

  // ADC 설정
  analogReadResolution(12);

  // 0~3.3V 입력 측정
  analogSetPinAttenuation(POT_PIN, ADC_11db);

  // 서보 PWM 초기화
  myServo.setPeriodHertz(50);

  // 서보 연결
  myServo.attach(
    SERVO_PIN,
    SERVO_MIN_US,
    SERVO_MAX_US
  );

  // 초기 위치: 중앙
  myServo.write(90);

  Serial.println("================================");
  Serial.println("ESP32 + Futaba S3003 Servo");
  Serial.println("Potentiometer Control");
  Serial.println("================================");
}

void loop() {

  if (millis() - lastUpdate >= UPDATE_INTERVAL) {

    lastUpdate = millis();

    // =================================
    // 1. 가변저항 읽기
    // =================================

    int potValue = analogRead(POT_PIN);

    // =================================
    // 2. ADC 값을 각도로 변환
    // =================================

    servoAngle = map(
      potValue,
      0, 4095,
      0, 180
    );

    servoAngle = constrain(
      servoAngle,
      0, 180
    );

    // =================================
    // 3. 서보모터 이동
    // =================================

    myServo.write(servoAngle);

    // =================================
    // 4. 시리얼 모니터 출력
    // =================================

    Serial.print("POT: ");
    Serial.print(potValue);

    Serial.print("  |  Servo Angle: ");
    Serial.print(servoAngle);

    Serial.println(" deg");
  }
}
