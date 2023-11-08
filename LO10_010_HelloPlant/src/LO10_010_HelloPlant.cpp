/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IoTClassroom_CNM.h"
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);

const int PUMPPIN  = D9;
const int SENSORPIN = A2;
bool doorOpen;
int pumpValue, sensorValue;
unsigned long currentTime;
unsigned long lastSecond;

IoTTimer soilTimer;

void setup() {
    Serial.begin(9600);
    pinMode(SENSORPIN,INPUT);
    pinMode(PUMPPIN,OUTPUT);
    currentTime = millis();
}

void loop() {
   sensorValue=analogRead(SENSORPIN);
   Serial.printf("%i\n",sensorValue);
    
    if((currentTime - lastSecond) > 1800000){
    if (sensorValue > 2930){ 
    digitalWrite(PUMPPIN,HIGH);
    delay(500); 
    digitalWrite(PUMPPIN,LOW);
  

      }
      
   lastSecond = millis();

      
    }  
   }
   
    
  
   




