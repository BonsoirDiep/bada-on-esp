// BaDa Software
// Code for NodeMCU
// Diep Nguyen - MTA

#include <math.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Set
#define FIREBASE_HOST "badaiots-kz.firebaseio.com"
#define WIFI_SSID "y5.cafe" // don't mind
#define WIFI_PASSWORD "12345678diepk3r" // don't mind

String productKey = String("hFrPLqQKIbmU");



unsigned long time1 = 0;
unsigned long time2 = 0;

int a[] = {0,0,0,0,0}; // analog sensor
int ia = 0;
int b[] = {0,0,0,0,0}; // oneWire sensor
int ib = 0;

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
  delay(3000);
}

void loop() {
  if ( (unsigned long) abs(millis() - time1) > 1000 ){
	int i = 0;
    String data;
    char str[11];
    char *dataDesign[4];
	ia = 0; ib = 0;
	for(i = 0;i<5;i++){
		a[i] = 0;
		b[i] = 0;
	}
	data = Firebase.getString("product/"+productKey+"/kit/espv2r");
	// ":16$0;    :5$124;    :5;4;    :1;"
	if (Firebase.failed()) {
	  return;
	}
	if(data.length()>0){
		data.toCharArray(str, 11) ;
		char * pch;
		pch = strtok (str,";");
		while (pch != NULL){
			dataDesign[i++] = pch;
			pch = strtok (NULL, ":");
		}
		if(i!=4) return;
		for(i=0; i<4;i++){
			pch = strtok (dataDesign[i],";");
			while (pch != NULL){
				int node = atoi(pch);
				if( (i==0 || i==1) && node>0 ){
					// do // pch is "16$1"
					char* pch2 = strchr(pch, '$');
					if (pch2 != NULL){
						*pch2 = 0; ++pch2;
						int val = atoi(pch2);
						if(val==0) digitalWrite(node, LOW);
						else digitalWrite(node, HIGH);
					}
				}
				else if(i==1 && node>0){
					// ao // pch is "5$240"
					char* pch2 = strchr(pch, '$');
					if (pch2 != NULL){
						*pch2 = 0; ++pch2;
						int val = atoi(pch2);
						analogWrite(node, (val*1023)/255);
					}
				}
				else if(i==2 && node>0){
					// ai
					if(ia<5) a[ia++] = node;
				}
				else if(i==3 && node>0){
					// onewire
					if(ib<5) b[ib++] = node;
				}
				pch = strtok (NULL, ";");
			}
		}
	};
	time1 = millis();
  }
  if ( (unsigned long) abs(millis() - time2) > 3000 ){
	String res1("");
	String res2("");
	for(int i=0;i<ia;i++){
		int node = a[i];
		if(node>0){
			int reading = analogRead(node);  
			float voltage = reading * 5.0 / 1023.0; 
			float val = voltage * 100.0;
			/*int aTmp  = analogRead(node);
			float mV = aTmp * 5.0 * 1000 / 1023.0;
			int val = round(mV / 10);*/
			res1+= String(node)+ String("$") + String(val)+ String(";");
		}
	}
	for(int i=0;i<ib;i++){
		int node = b[i];
		if(node>0){
			OneWire oneWire(node);
			DallasTemperature sensors(&oneWire);
			sensors.begin();
			sensors.requestTemperatures();
			float val = sensors.getTempCByIndex(0);
			res2+= String(node)+ String("$") + String(val)+ String(";");
		}
	}
	bool con1 = (res1 == String(""));
	bool con2 = (res2 == String(""));
	if(con1&&con2) { time2 = millis(); return; }
	if(con1) res1+= "0$0;";
	if(con2) res2+= "0$0;";
	String response(":");
	response+= res1+ String(":") + res2;
	Firebase.setString("product/"+productKey+"/kit/espv2w", response);
		
	time2 = millis();
  }
}
