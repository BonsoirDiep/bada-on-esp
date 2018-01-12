#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>

// Set these to run example.
#define FIREBASE_HOST "badaiots-kz.firebaseio.com"
#define WIFI_SSID "BMTDH3"
#define WIFI_PASSWORD "0979818125"
int myPins[] = {0,0,0,0,0}; // toi da 5 cam bien
int myLights[] = {0,0,0,0,0}; // toi da 5 den
int myAnalogs[] = {0,0,0,0,0}; // toi da 5 den xoay

void setup() {
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
  int i = 0;
  String data;
  char str[11];

  data = Firebase.getString("product/hFrPLqQKIbmU/kit/sens");
  if (Firebase.failed()) {
      Serial.print("load data error: ");
      Serial.println(Firebase.error());
      return;
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
      return;
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
      return;
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

void loop() {
  /*int i = 0;
  for (i = 0; i < 5; i = i + 1) {
    if(myPins[i]>0){
      Serial.print(myPins[i]);
      Serial.print("; ");
    }
  }
  Serial.println("-------------");
  for (i = 0; i < 5; i = i + 1) {
    if(myLights[i]>0){
      Serial.print(myLights[i]);
      Serial.print("; ");
    }
  }
  Serial.println("-------------");
  for (i = 0; i < 5; i = i + 1) {
    if(myAnalogs[i]>0){
      Serial.print(myAnalogs[i]);
      Serial.print("; ");
    }
  }
  Serial.println("-------------");*/
  bool data = Firebase.getBool("product/hFrPLqQKIbmU/kit/node1");
  pinMode(LED_BUILTIN, OUTPUT);
  if(data){
    digitalWrite(LED_BUILTIN, LOW);
  }
  else digitalWrite(LED_BUILTIN, HIGH);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing, getting /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(200);
}
