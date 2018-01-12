#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>

// Set these to run example.
#define FIREBASE_HOST "badaiots-kz.firebaseio.com"
#define WIFI_SSID "y5.cafe"
#define WIFI_PASSWORD "12345678diepk3r"
int myPins[] = {0,0,0,0,0}; // toi da 5 cam bien
int myLights[] = {0,0,0,0,0}; // toi da 5 den
int myAnalogs[] = {0,0,0,0,0}; // toi da 5 den xoay

void getDataDesign(){
  int i = 0;
  String data;
  char str[11];

  data = Firebase.getString("product/hFrPLqQKIbmU/kit/sens");
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
         myPins[i] = node;
         i++;
         pch = strtok (NULL, ";");
      } else break;
     
    }
  };
  data = Firebase.getString("product/hFrPLqQKIbmU/kit/lights");
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

  data = Firebase.getString("product/hFrPLqQKIbmU/kit/analogs");
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
  //pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
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
  delay(500);
  getDataDesign();
}

void loop() {
  bool check = Firebase.getBool("product/hFrPLqQKIbmU/kit/rebuild");
  if (Firebase.failed()){
    return;
  }
  if(check){ //rebuild
    Firebase.setBool("product/hFrPLqQKIbmU/kit/rebuild", false);
    // handle error
    if (Firebase.failed()) {
      return;
    }
    getDataDesign();
  }
  
  int i = 0;
  for (i = 0; i < 5; i = i + 1) {
    if(myPins[i]>0){
      /*
      float data = Firebase.getFloat("product/hFrPLqQKIbmU/sens/node"+ String(myPins[i]));
      if (!Firebase.failed()) {
           //Serial.println(data);
      }
      */
      int analogValue = analogRead(myPins[i]);
      //float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
      //float celsius = millivolts/10;
      Firebase.setFloat("product/hFrPLqQKIbmU/sens/node"+ String(myPins[i]), analogValue);
      delay(200);
    }
  }
  for (i = 0; i < 5; i = i + 1) {
    if(myLights[i]>0){
      bool data = Firebase.getBool("product/hFrPLqQKIbmU/kit/node"+ String(myLights[i]));
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
      int data = Firebase.getInt("product/hFrPLqQKIbmU/kit/node"+ String(myAnalogs[i]));
      if (!Firebase.failed()) analogWrite(myAnalogs[i], (data*1023)/255);
    }
  }
  delay(200);
}
