// BaDa Software
// Code for NodeMCU
// Diep Nguyen - MTA

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Set these to run example.
#define FIREBASE_HOST "badaiots-kz.firebaseio.com"
#define WIFI_SSID "y5.cafe" // don't mind
#define WIFI_PASSWORD "12345678diepk3r" // don't mind

//Design oneWire max 2, ai max 1...
int mySensors[] = {0,0,0,0,0}; // analog sensor
int myLights[] = {0,0,0,0,0}; // on/off
int myAnalogs[] = {0,0,0,0,0}; // analog output
int myOnes[] = {0,0,0,0,0}; // oneWire sensor
String productKey = String("hFrPLqQKIbmU");

unsigned long time1 = 0;
void getDataDesign(){
  int i = 0;
  String data;
  char str[11];
  // reset
  for(i = 0;i<5;i++){
    mySensors[i] = 0;
	myLights[i] = 0;
	myAnalogs[i] = 0;
	myOnes[i] = 0;
  }
  data = Firebase.getString("product/"+productKey+"/kit/one");
  if (Firebase.failed()) {
      Serial.print("load data error: ");
      Serial.println(Firebase.error());
      data = String(";0");
  }
  if(data.length()>0){
    data.toCharArray(str, 11) ;
    char * pch;
    pch = strtok (str,";");
    while (pch != NULL)
    {
      if(i>4) break;
      int node = atoi(pch);
      if(node>0){
         myOnes[i] = node;
         i++;
         pch = strtok (NULL, ";");
      } else break;
     
    }
  };

  data = Firebase.getString("product/"+productKey+"/kit/sens");
  if (Firebase.failed()) {
      Serial.print("load data error: ");
      Serial.println(Firebase.error());
      data = String(";0");
  }
  if(data.length()>0){
    data.toCharArray(str, 11) ;
    char * pch;
    pch = strtok (str,";");
    while (pch != NULL)
    {
      if(i>4) break;
      int node = atoi(pch);
      if(node>0){
         mySensors[i] = node;
         i++;
         pch = strtok (NULL, ";");
      } else break;
     
    }
  };
  data = Firebase.getString("product/"+productKey+"/kit/lights");
  if (Firebase.failed()) {
      Serial.print("load data error: ");
      Serial.println(Firebase.error());
      data = String(";0");
  }
  i = 0;
  if(data.length()>0){
    data.toCharArray(str, 11) ;
    char * pch;
    pch = strtok (str,";");
    while (pch != NULL)
    {
      if(i>4) break;
      int node = atoi(pch);
      if(node>0){
         myLights[i] = node;
         i++;
         pch = strtok (NULL, ";");
      } else break;
     
    }
  };

  data = Firebase.getString("product/"+productKey+"/kit/analogs");
  if (Firebase.failed()) {
      Serial.print("load data error: ");
      Serial.println(Firebase.error());
      data = String(";0");
  }
  i = 0;
  if(data.length()>0){
    data.toCharArray(str, 11) ;
    char * pch;
    pch = strtok (str,";");
    while (pch != NULL)
    {
      if(i>4) break;
      int node = atoi(pch);
      if(node>0){
         myAnalogs[i] = node;
         i++;
         pch = strtok (NULL, ";");
      } else break;
     
    }
  };
}
void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  //pinMode(D2, OUTPUT); // oneWire support
  // pinMode(D3, OUTPUT);// not-use: GPIO0
  //pinMode(D4, OUTPUT); // oneWire support
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST);
  delay(900);
  getDataDesign();
  Firebase.setBool("product/"+productKey+"/kit/rebuild", false);
}

void loop() {
  bool check = Firebase.getBool("product/"+productKey+"/kit/rebuild");
  if (Firebase.failed()){
    return;
  }
  if(check){ //rebuild
    Firebase.setBool("product/"+productKey+"/kit/rebuild", false);
    // handle error
    if (Firebase.failed()) {
      return;
    }
    getDataDesign();
  }
  
  int i = 0;
  for (i = 0; i < 5; i = i + 1) {
    if(myLights[i]>0){
      bool data = Firebase.getBool("product/"+productKey+"/kit/node"+ String(myLights[i]));
      if (!Firebase.failed()) {
          if(data){
            digitalWrite(myLights[i], HIGH);
          }
          else{
            digitalWrite(myLights[i], LOW);
          }
       }
     }
  }
  for (i = 0; i < 5; i = i + 1) {
    if(myAnalogs[i]>0){
      int data = Firebase.getInt("product/"+productKey+"/kit/node"+ String(myAnalogs[i]));
      if (!Firebase.failed()) analogWrite(myAnalogs[i], (data*1023)/255);
    }
  }
  
  if((unsigned long) abs(millis() - time1) > 1000){
	  for (i = 0; i < 5; i = i + 1) {
		if(myOnes[i]>0){
		  OneWire oneWire(myOnes[i]);
		  DallasTemperature sensors(&oneWire);
		  sensors.begin();
		  sensors.requestTemperatures();
		  Firebase.setFloat("product/"+productKey+"/sens/node"+ String(myOnes[i]), sensors.getTempCByIndex(0));
		}
	  }
	  for (i = 0; i < 5; i = i + 1) {
		if(mySensors[i]>0){
		  int analogValue = analogRead(mySensors[i]);
		  Firebase.setFloat("product/"+productKey+"/sens/node"+ String(mySensors[i]), analogValue);
		}
	  }
	  time1 = millis();
  }
}
