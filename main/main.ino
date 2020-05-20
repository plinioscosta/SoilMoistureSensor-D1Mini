#include <ESP8266WiFi.h>
#include <PubSubClient.h> //MQTT Service

#define SensorPin A0 
IPAddress mqtt_server(192, 168, 15, 48);
WiFiClient espClient;
PubSubClient client(espClient);
char MQTTmsg[50];

void setup() {
  Serial.begin(9600);

  //Wifi connection
  IPAddress arduino_ip_1 (192, 168, 15, 21);
  IPAddress dns_ip_1     (  8,   8,   8,   8);
  IPAddress gateway_ip_1 (192, 168, 15, 1);
  IPAddress subnet_mask_1 (255, 255, 255, 0);

  WiFi.config(arduino_ip_1,gateway_ip_1,subnet_mask_1);
  WiFi.begin("Gorduchos", "ntpl2201");  

  for (int i=0; i <= 10; i++){
    if(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
      if(i==10){
        Serial.print("Not possible to connected to Naty's home\n");
      }
    }
    else{
    Serial.print("Connected to home\n");
    break;
        }
   }

  //MQTT server configuration
  client.setServer(mqtt_server, 1883); 
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  client.connect(clientId.c_str(),"pcosta", "PlinioPc1623" );
  
  for (int i=0; i <= 20; i++){
    if(!client.connected()){
    clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    client.connect(clientId.c_str(),"pcosta", "PlinioPc1623" );
    delay(500);
    Serial.print(".");
      if(i==10){
        Serial.print("Not possible to connected to MQTT server\n");
        Serial.println("Sleeping");
        ESP.deepSleep(5*60e6); // Deep Sleep for 1 minute
      }
    }
    else{
    float sensorValue = analogRead(SensorPin);
    Serial.println(sensorValue);
    snprintf (MQTTmsg, 75, "%f", sensorValue);
    Serial.println(MQTTmsg);
    client.publish("umidadePlanta", MQTTmsg);
    delay(2000);
    Serial.println("Sleeping");
    ESP.deepSleep(60*60e6); // Deep Sleep for 1 minute  
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
