
#include <SparkFunMPU9250-DMP.h>

#define SAMPLING_FREQUENCY 15 //Hz, we start testing a relatively low value with a maximum possible value of 1000

MPU9250_DMP sensor;

float current_angle;
float adjusted_roll;
float velocity;
float time_taken=0;
float sampling_period_ms;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  if (sensor.begin() != INV_SUCCESS)
     {
        while (1)
        {
          Serial.println("Unable to communicate with MPU-9250");
          Serial.println("Check connections, and try again.");
          Serial.println();
          delay(5000);
        }
     }
      sensor.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL | DMP_FEATURE_SEND_CAL_GYRO, 40);
      sampling_period_ms = round(1000*(1.0/SAMPLING_FREQUENCY));
}

void loop() {
  // put your main code here, to run repeatedly:
   time_taken=millis();
   Get_Angle();
   Get_Velocity();
   Serial.println("Angular displacement: " + String(adjusted_roll) + " Time (ms): " + String(millis()));
   Serial.println("Angular velocity: " + String(velocity) + " degrees per second");
    while((millis()-time_taken)<(sampling_period_ms)){
         delay(1);          
    }
 }

float Get_Angle(void){
    float angle;
    if ( sensor.fifoAvailable() )
        {
        if ( sensor.dmpUpdateFifo() == INV_SUCCESS)
        {
          sensor.computeEulerAngles();
          angle = sensor.roll;
          if((angle)>180)
            {
             adjusted_roll=(angle)-360;
            }
          else if (angle<=180)
            {
             adjusted_roll=angle; // +/- 180 degrees
            }
        }
    }
}
float Get_Velocity(void){
    velocity=sensor.calcGyro(sensor.gx);
}
