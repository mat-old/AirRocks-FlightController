#define MOTOR_ZERO_LEVEL  125
#define MOTOR_ARM_START  140
#define MOTOR_MAX_LEVEL  254
#define MOTOR0 9

void setup(){
  Serial.begin(115200);
  motorInit();
  motorArm();
}
void loop() {
  for(int i = 0; i < 114; i++)
  {
    analogWrite(MOTOR0,MOTOR_ARM_START+i);
    delay(100);
    
  }
  analogWrite(MOTOR0,MOTOR_ZERO_LEVEL);
  delay(10000);
  /*analogWrite(MOTOR0,MOTOR_ARM_START+20);
  delay(2000);
  Serial.print("tick");
  analogWrite(MOTOR0,MOTOR_ZERO_LEVEL);
  delay(2000);
  Serial.print("tock");*/
}

void motorInit(){
  pinMode(MOTOR0,OUTPUT); 
  analogWrite(MOTOR0,MOTOR_ZERO_LEVEL);
}

void motorArm(){
  analogWrite(MOTOR0,MOTOR_ZERO_LEVEL);
}
