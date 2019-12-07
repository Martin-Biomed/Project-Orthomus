/* Buttons */
const int up_button = 5; //Normal Digital Pins
const int down_button = 6;
const int on_button = 3;

/* Relays */
const int main_power_relay = 9;
const int solenoid_relay_1 = 2;
const int solenoid_relay_2 = 7;

/* Servos */
const int servo_1 = 10; //PWM pins
const int servo_2 = 11;

/* Key variables */
int pwm1; // Variable that we send to the servos
int pwm2;
int deg; // The angular positions that we want servo to go to
int reverse_deg; // t

const int bottom_extreme = 15; //Extrema for device movement
const int top_extreme = 155;
#define gear_ratio 0.5294117647

#define extend1 digitalWrite(solenoid_relay_1, HIGH) //Defining the extension/retraction functions for both solenoids 
#define retract1 digitalWrite(solenoid_relay_1, LOW)
#define extend2 digitalWrite(solenoid_relay_2, HIGH)
#define retract2 digitalWrite(solenoid_relay_2, LOW)

bool up_button_state;
bool down_button_state;
bool on_button_state;
bool lock;
bool device_on;

void setup() {
    
    Serial.begin(9600);
    pinMode(up_button, INPUT);
    pinMode(down_button, INPUT);
    pinMode(on_button, INPUT);

    pinMode(main_power_relay, OUTPUT);
    pinMode(solenoid_relay_1, OUTPUT);
    pinMode(solenoid_relay_2, OUTPUT);
    
    reverse_deg = 180-bottom_extreme;
    lock = LOW; 
    device_on = LOW;
    
}

void loop() {
   
    main_power_control();
       
    device_movement_control();
    
 }

void main_power_control(){
   on_button_state = digitalRead(on_button); 
   if (on_button_state == HIGH && lock == LOW) {  //Buttons start off LOW until they are pressed
      lock = HIGH;                                //Once button has been pressed, lock = HIGH, meaning that this "if" statement is executed once until on_button is pressed again
      delay(20);
      pwm1 = map(deg/gear_ratio, 0, 280, 116, 267);
      pwm2 = map(reverse_deg/gear_ratio, 0, 280, 116, 267);
      analogWrite(servo_1, pwm1); //New lines supposed to turn off the servos before we start moving them
      analogWrite(servo_2, pwm2); 
        
  
      if (device_on == HIGH) {                    //If this loop is executed a second time, it means the on_button was hit again, at which point everything will be off
        Serial.println("Power supply is turned off");
        digitalWrite(main_power_relay, LOW);
        reset_procedure();
        extend1;
        extend2;
        device_on = LOW;
      }
      else {                                        //When the "if" statement is executed, runs through once, retracting the solenoids and enabling main power supply 
        Serial.println("Power supply is turned on");
        digitalWrite(main_power_relay, HIGH);
        retract1;
        retract2;
        device_on = HIGH;
      }
   }
   if (on_button_state == LOW && lock == HIGH) { //When the on_button is pushed again, lock = HIGH from the first turning on function
      lock = LOW;
   } 
}

void device_movement_control(){
   if (device_on == HIGH) {
       up_button_state = digitalRead(up_button);
       down_button_state = digitalRead(down_button);
       delay(30);
       
       if (up_button_state == HIGH) {
          retract1;                              //Retracting the solenoids in preparation for movement
          retract2;   
          Serial.println("Button up pressed");
          delay(300);
          up_button_loop();
       }
       if (down_button_state == HIGH) {
          retract1;                               //Retracting the solenoids in preparation for movement
          retract2;
          delay(300);
          Serial.println("Button down pressed");
          down_button_loop();
       }
   }
}
void reset_procedure(){
    while (deg>15){
        pwm1 = map(deg/gear_ratio, 0, 280, 114, 267);
        pwm2 = map(reverse_deg/gear_ratio, 0, 280, 114, 267);
        analogWrite(servo_1, pwm1);
        analogWrite(servo_2, pwm2);
        deg = deg - 10;
        reverse_deg = reverse_deg + 10;
        delay(100);   
    }
}

void up_button_loop() {
  while (up_button_state == HIGH && deg < top_extreme) {   //If the user keeps button pressed, the state will be LOW for that duration
    up_button_state = digitalRead(up_button);
    Serial.println("up 10 degrees: " + String(deg));
    pwm1 = map(deg/gear_ratio, 0, 280, 114, 267);
    pwm2 = map(reverse_deg/gear_ratio, 0, 280, 114, 267);
    analogWrite(servo_1, pwm1);
    analogWrite(servo_2, pwm2);
    deg = deg + 10;
    reverse_deg = reverse_deg - 10;
    delay(500);
  }
  extend1;                                  //Extend the solenoids once the movement concludes
  extend2;
  deg = deg - 10;
  reverse_deg = reverse_deg + 10;
}
void down_button_loop() {
  while (down_button_state == HIGH && deg > bottom_extreme) {
    down_button_state = digitalRead(down_button);
    Serial.println("down 10 degrees: " + String(deg));
    pwm1 = map(deg/gear_ratio, 0, 280, 114, 267);
    pwm2 = map(reverse_deg/gear_ratio, 0, 280, 114, 267);
    analogWrite(servo_1, pwm1);
    analogWrite(servo_2, pwm2);
    deg = deg - 10;
    reverse_deg = reverse_deg + 10;
    delay(500);
  }
  extend1;                                //Extend the solenoids once the movement concludes
  extend2;
  deg = deg + 10;
  reverse_deg = reverse_deg - 10;
}
