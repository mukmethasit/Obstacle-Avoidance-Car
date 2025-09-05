#include <Servo.h>
#define PWM_ML 2
#define in1_ML 3
#define in2_ML 4
#define in1_MR 5
#define in2_MR 6
#define PWM_MR 7

// ตัวแปรสำหรับเก็บองศาการหมุน Servo
int right = 0;
int front = 90;
int left = 180;

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;

long duration;
int distanceFront;
int distanceLeft;
int distanceRight;
int SpeedR = 200;
int SpeedL = 200;

Servo myServo;

int measureDistance();  // ฟังก์ชันวัดระยะทาง

void setup() {
  pinMode(PWM_ML, OUTPUT);
  pinMode(in1_ML, OUTPUT);
  pinMode(in2_ML, OUTPUT);
  pinMode(in1_MR, OUTPUT);
  pinMode(in2_MR, OUTPUT);
  pinMode(PWM_MR, OUTPUT);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(90);
  delay(3000);
}

void loop() {
  // วัดระยะทางด้านหน้า
  distanceFront = measureDistance();  // วัดระยะทางด้านหน้า
  Serial.print("Distance Front: ");
  Serial.print(distanceFront);
  Serial.println(" cm");

  if (distanceFront > 20) {
    moveForward();
  } else {
    stopMoving();
    checkLeft();
    checkRight();
    // ตัดสินใจเลือกทิศทางที่จะเคลื่อนที่
    if (distanceLeft > 20 || distanceRight > 20) {
      if (distanceLeft > distanceRight) {
        myServo.write(90);
        turnLeft();
      } else {
        myServo.write(90);
        turnRight();
      }
    } else {
      // ถ้าไปไม่ได้ทุกทาง ให้ถอยหลัง
      Serial.println("No way! Going Backward");
      moveBackward();
      delay(1000);
      stopMoving();
    }
  }
  delay(100);
}


void checkLeft() {
  myServo.write(180);
  delay(1000);  // รอให้เซอร์โวมอเตอร์หมุนไปยังตำแหน่ง
  distanceLeft = measureDistance();
  Serial.print("Distance Left: ");
  Serial.print(distanceLeft);
  Serial.println(" cm");
}


void checkRight() {
  myServo.write(0);
  delay(1000);  // รอให้เซอร์โวมอเตอร์หมุนไปยังตำแหน่ง
  distanceRight = measureDistance();
  Serial.print("Distance Right: ");
  Serial.print(distanceRight);
  Serial.println(" cm");
}


int measureDistance() {
  // เคลียร์พิน trigPin โดยตั้งค่าให้เป็น LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // อ่านค่า echoPin และคำนวณระยะเวลา
  duration = pulseIn(echoPin, HIGH);  // อ่านค่าระยะเวลา echoPin เป็น HIGH

  // คำนวณระยะทาง (duration / 2) / 29.1
  int distance = duration * 0.034 / 2;

  return distance;
}


void moveForward() {
  digitalWrite(in1_ML, LOW);
  digitalWrite(in2_ML, HIGH);
  analogWrite(PWM_ML, SpeedL);

  digitalWrite(in1_MR, LOW);
  digitalWrite(in2_MR, HIGH);
  analogWrite(PWM_MR, SpeedR);
  Serial.println("Move Forward");
}



void stopMoving() {
  digitalWrite(in1_ML, LOW);
  digitalWrite(in2_ML, LOW);
  analogWrite(PWM_ML, 0);

  digitalWrite(in1_MR, LOW);
  digitalWrite(in2_MR, LOW);
  analogWrite(PWM_MR, 0);
}


void turnLeft() {
  digitalWrite(in1_ML, HIGH);
  digitalWrite(in2_ML, HIGH);
  analogWrite(PWM_ML, 0);
  Serial.println("Turn Left");
  digitalWrite(in1_MR, LOW);
  digitalWrite(in2_MR, HIGH);
  analogWrite(PWM_MR, 170);
  // รอ 250 มิลลิวินาทีก่อนหยุดหมุน
  delay(250);
  // เช็คระยะทางด้านหน้าใหม่
  myServo.write(90);
  delay(250);
  distanceFront = measureDistance();
  if (distanceFront > 20) {
    moveForward();
  } else {
    stopMoving();
  }
}




void turnRight() {
  digitalWrite(in1_ML, LOW);
  digitalWrite(in2_ML, HIGH);
  analogWrite(PWM_ML, 170);
  Serial.println("Turn Right");
  digitalWrite(in1_MR, HIGH);
  digitalWrite(in2_MR, HIGH);
  analogWrite(PWM_MR, 0);
  delay(250);
  // เช็คระยะทางด้านหน้าใหม่
  myServo.write(90);
  delay(250);
  distanceFront = measureDistance();
  if (distanceFront > 20) {
    moveForward();
  } else {
    stopMoving();
  }
}

void moveBackward() {
  digitalWrite(in1_ML, HIGH);  // หมุนมอเตอร์ ML ถอยหลัง
  digitalWrite(in2_ML, LOW);
  analogWrite(PWM_ML, SpeedL);

  digitalWrite(in1_MR, HIGH);  // หมุนมอเตอร์ MR ถอยหลัง
  digitalWrite(in2_MR, LOW);
  analogWrite(PWM_MR, SpeedR);

  Serial.println("Move Backward");
  delay(250);

  myServo.write(90);
}
