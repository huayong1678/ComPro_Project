#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

#include <ESP8266WiFi.h>
#include <DHT.h>

#define BLYNK_PRINT Serial
#define DHTTYPE DHT11 
#define DHTPIN D3 /*temp and Humidity sensor*/
#define waterPump D5 /*Pumpwater*/
#define sensorPin A0 /*soil moisture sensor*/

int tempo=100;/*watering time*/

/* Standard Soil moisture */
int limitSoilmoisture = 300;
int limitMaxSoilmoisture = 900;


/*username and password your wifi*/
char auth[] = "7f3667afef1a41c4b6a3b244f5a58af9"; /*code send from email*/
char ssid[] = "AndroidAP"; /*nameWifi*/
char pass[] = "yui251571";/*Passwordwifi*/


/*program setting*/
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  pinMode(sensorPin, INPUT);
  pinMode(waterPump, OUTPUT);
}

/*DHT11*/
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  if (isnan(h) || isnan(t)) {
    Blynk.notify("Failed DHT sensor!");
  }
  else if(t>= 15 && t <= 40){
    Blynk.notify("Suitable Temperature Makes Plants grow!");
  }
  else if(t >= 20 && t <= 30){
    Blynk.notify("Transport Of Food In Plants Is Good!");
  }
 /* send Humidity Value and Temperature Value to V5 V6*/
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

  /*button on off */
BLYNK_WRITE(V1){
  digitalWrite(waterPump,param.asInt());
}
BLYNK_CONNECTED(){
  Blynk.syncAll();
}  

/*Atomatic PumpWater*/
void loop()
{ 
  Blynk.run();
  timer.run();
  int sensorValue = analogRead(sensorPin);
  Blynk.virtualWrite(V2, sensorValue); /*send Value soil moisture to V2 in blynk*/

  /*print to Serial Monitor*/
  Serial.print("Soil moisture: ");
  Serial.print(sensorValue);
  Serial.println(" %");
  delay(tempo);
  
  /* Control Pumpwater*/
  if (sensorValue > 0 && sensorValue < limitSoilmoisture){
    Blynk.notify("Water your plants");
    digitalWrite(waterPump,HIGH);
    delay(tempo); 
  }
  else if (sensorValue <= limitSoilmoisture && sensorValue < limitMaxSoilmoisture){
    digitalWrite(waterPump,LOW);
    Blynk.notify("plants happy!");
    delay(tempo);  
  }
}
