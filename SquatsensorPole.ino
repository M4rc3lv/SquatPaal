#include <Wire.h>
#include <VL53L0X.h>
#include <Keyboard.h>

// Max. distance for squat sensor to detect human (i.e. not a squat)
#define MAX_DISTANCE_CM 100

#define LED_ON         10
#define LED_SENSE      11
#define BUT_ON         5

VL53L0X sensor;
boolean IsOn=true;

void setup() {
 delay(3000); 
 pinMode(LED_ON,OUTPUT);
 pinMode(LED_SENSE,OUTPUT);
 pinMode(BUT_ON,INPUT_PULLUP);
 
 Wire.begin(); 
 sensor.init(); 
 while(!sensor.init()) {
  // Unable to init sensor! This indicates a wireing/hardware problem.
  for(int i=0; i<20; i++) {
   digitalWrite(LED_SENSE,LOW);
   digitalWrite(LED_ON,HIGH);
   delay(80);
   digitalWrite(LED_SENSE,HIGH);
   digitalWrite(LED_ON,LOW);
   delay(80);
  }
 } 
 sensor.setTimeout(500);
 sensor.setMeasurementTimingBudget(20000); // 20ms timing budget==>(snelste modus)   

 digitalWrite(LED_ON,IsOn?HIGH:LOW);
 digitalWrite(LED_SENSE,LOW);     
}

void loop() { 
 
 if(digitalRead(BUT_ON)==LOW) {    
  IsOn = !IsOn;
  digitalWrite(LED_ON,IsOn?HIGH:LOW);
  delay(20);
  while(digitalRead(BUT_ON)==LOW) {
   // Debounce
   delay(20);
  }  
 }

 // Do nothing if the console is switched off
 if(!IsOn) {  
  Keyboard.releaseAll();
  digitalWrite(LED_SENSE,LOW);
  digitalWrite(LED_ON,LOW);  
  return;
 }
 
 int cm=sensor.readRangeSingleMillimeters()/10;  
 //Serial.println(cm);
 if(cm<MAX_DISTANCE_CM) {Keyboard.press(KEY_LEFT_CTRL);digitalWrite(LED_SENSE,HIGH);}
 else {Keyboard.releaseAll(); digitalWrite(LED_SENSE,LOW);}
}
