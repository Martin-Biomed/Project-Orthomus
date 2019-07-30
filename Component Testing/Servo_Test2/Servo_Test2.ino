#include<Servo.h>

Servo Test_Servo;

int pos=0;
int map_angle; //angle calculated from the map function


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Test_Servo.attach(4); // attaches servo to pin 4

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.read(pos);
  map_angle=map(pos,0,167.93,0,180); //Servo has 280 degree range, to limit it to 180, we don't use the normal limit 255
  Test_Servo.write(map_angle);
  Serial.print(map_angle);

  
  if (pos>180){
    pos=0;
  }
}
