/* 
 * Project LO10_010_HelloPlant in cloud
 * Author: Evelyn Rosenberg 
 * Date: Nov. 2023
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "IoTClassroom_CNM.h"
//#include "tclPlatDecls.h"
#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"


TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

/****************************** Feeds ***************************************/ 
// Setup Feeds to publish or subscribe 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Subscribe waterButtonFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/wateronoff"); 
Adafruit_MQTT_Publish moistureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/moisturenumber");

SYSTEM_MODE(AUTOMATIC);
// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);

const int PUMPPIN  = D9;
const int SENSORPIN = A2;
bool doorOpen;
int pumpValue, sensorValue;
unsigned long currentTime;
unsigned long lastSecond;
float subValue;

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
    
    if((currentTime - lastSecond) > 60000){
    if (sensorValue > 2930){ 
    digitalWrite(PUMPPIN,HIGH);
    delay(500); 
    digitalWrite(PUMPPIN,LOW);

      }
    
   lastSecond = millis();
    if(mqtt.Update()) {
      moistureFeed.publish(sensorValue);
      Serial.printf("Publishing %i\n",sensorValue); 
      }
    //randomTime = millis();
  }
  
 Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10000))) {
    if (subscription == &waterButtonFeed) {
      subValue = atof((char *)waterButtonFeed.lastread);
    if (subValue){
        digitalWrite(PUMPPIN,HIGH);
            delay(500);
        digitalWrite(PUMPPIN,LOW);

    }
    else{
      digitalWrite (PUMPPIN,LOW);
    }
      }
    }
  

}

void MQTT_connect() {
  int8_t ret;//Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}

      
    
   
   
    
  
   




