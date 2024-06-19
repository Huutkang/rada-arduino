#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"


// Các chân kết nối cho cảm biến siêu âm
#define trigPin 6
#define echoPin 7

// Các chân kết nối bt1, bt2, bt3
#define bt1 2
#define bt2 3
#define bt3 4

// Chân điều khiển động cơ servo
#define servoPin 5

// Biến để lưu trữ thời gian và khoảng cách đo được
long duration;
int distance ;

// Biến để điều khiển góc quay của servo
int angle = 90;
int direction = 1; // Hướng quay của servo (-1, 0, 1)
float dl = 15; // Thời gian chờ giữa các góc quay của servo

// Biến để kiểm soát trạng thái của LED và mảng chứa các chân LED
bool led_on = true;
int array_led[] = {8,9,10,11,12,13};
int length_arr = 6;  // độ dài mảng bên trên. vậy đi cho nhanh
int index_led = 0;
int index_led_old = 0;

// các biến để hẹn giờ
unsigned long current_time;
unsigned long time1=0;
unsigned long time2=0;
unsigned long time3=0;
unsigned long time4=0;
unsigned long time5=0;

// Khởi tạo đối tượng cho màn hình LCD và servo motor
Servo myservo;
rgb_lcd lcd;

// Hàm đo khoảng cách sử dụng cảm biến siêu âm
int calculateDistance()
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  return distance;
}

void rada(){
  // Cập nhật góc quay của servo, đo khoảng cách và hiển thị thông tin
  calculateAngle();
  myservo.write(angle);
  calculateDistance();
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);
  // display();
  if (distance<40){
    led_on = true;
  }
  else{
    led_on = false;
  }
}

// Hàm tính toán góc quay của servo
void calculateAngle(){
  angle = angle + direction;
  if (angle > 165){
    angle = 165;
    direction = -1;
  }
  if (angle < 15){
    angle = 15;
    direction = 1;
  }
}

// Hàm hiển thị thông tin lên màn hình LCD
void display(){
  lcd.clear();
  lcd.print("Angle: ");
  lcd.print(angle);
  if (distance<40){
    lcd.setCursor(0,1);
    lcd.print("Distance: ");
    lcd.print(distance);
  }
}

// Hàm tính toán và cập nhật thời gian chờ giữa các góc quay của servo
void calculateSpeed(){
  float v = analogRead(A0);
  v = (v/1023)*285;
  dl = int(15 + v);
}

// Hàm xử lý nút bấm
void button(){
  if (digitalRead(bt1)==0 && digitalRead(bt3)==0){
    direction = 0;
  }
  else if (digitalRead(bt2)==0){
    direction = 0;
  }
  else if (digitalRead(bt1)==0){
    direction = -1;
  }
  else if (digitalRead(bt3)==0){
    direction = 1;
  }
}

// Hàm hiển thị led khi có vật cản
void led(int array_led[]){
  if (led_on){
    if (index_led==length_arr){
      index_led = 0;
    }
    index_led_old = index_led - 3;
    if (index_led_old<0){
      index_led_old = length_arr+index_led_old;
    }
    digitalWrite(array_led[index_led],HIGH);
    digitalWrite(array_led[index_led_old],LOW);
    index_led = index_led + 1;
  }
  else{
    for (int i=0; i<length_arr; i++){
      digitalWrite(array_led[i],LOW);
    }
    index_led=0;
    index_led_old=0;
  }
}

// Hàm để hẹn giờ
bool Timer(unsigned long *time, int wait){
  current_time = millis();
  if (current_time-*time>wait){
    *time = current_time;
    return true;
  }
  else{
    return false;
  }
}


void setup()
{
  // Khởi tạo chân kết nối và servo motor
  pinMode(trigPin , OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(bt1, INPUT);
  pinMode(bt2, INPUT);
  pinMode(bt3, INPUT);
  for (int i=0; i<length_arr; i++){
    pinMode(array_led[i] , OUTPUT);
  }
  myservo.attach(servoPin);
  Serial.begin(9600);
  // Khởi tạo màn hình LCD và bộ đếm thời gian
  lcd.begin(16,2);
  lcd.clear();
}

void loop()
{
  // Không dùng delay nhé
  if (Timer(&time1, dl)){
    rada();
  }
  if (Timer(&time2, 30)){
    button();
  }
  if (Timer(&time3, 50)){
    calculateSpeed();
  }
  if (Timer(&time4, 100)){
    display();
  }
  if (Timer(&time5, 2*dl)){
    led(array_led);
  }
}










