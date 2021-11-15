

/*
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#                                                                     # 
# NodeMCU ESP8266/ESP12E    RFID MFRC522 / RC522                      #
#         D2       <---------->   SDA/SS                              #
#         D5       <---------->   SCK                                 #
#         D7       <---------->   MOSI                                #
#         D6       <---------->   MISO                                #
#         GND      <---------->   GND                                 #
#         D1       <---------->   RST                                 #
#         3V/3V3   <---------->   3.3V                                #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
*/


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#define SS_PIN D2  
#define RST_PIN D1
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define ON_Board_LED 2

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

const char* ssid = "YOUR_NETWORK_SSSID";
const char* password =  "YOUR_NETWORK_PASSWORD";

int getid() {  
  if(!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
 
  
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i];
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

void handleReadCard() {
  readsuccess = getid();
 
  if(readsuccess) {
    digitalWrite(ON_Board_LED, LOW);
    handleAttendIn();
    digitalWrite(ON_Board_LED, HIGH);
  }
}

void handleAttendIn(){
   if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;
   WiFiClient wifiClient;
   
   http.begin(wifiClient,"http://3.144.31.214/api/iot/attend/"+StrUID);
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");

   String httpRequestData = "key=CONTACT_ME_TO_GIVE_YOU_THE_KEY";
 
   int httpResponseCode = http.PUT(httpRequestData);   
 
   if(httpResponseCode>0){
    String response = http.getString();   
    Serial.write(1);
    Serial.println(response);  
 
   }else{
 
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
   }
 
   http.end();
 
 }else{
    Serial.println("Error in WiFi connection");
 }
 
  delay(1500);
}



void setup() {
 
  Serial.begin(9600); // KEEP FOR SERIAL COMMUNICATION
  SPI.begin();      
  mfrc522.PCD_Init(); 

  pinMode(ON_Board_LED,OUTPUT); 
  digitalWrite(ON_Board_LED, HIGH); // Turn off Led On Board

  ESP.eraseConfig();
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}
void loop() {
  handleReadCard();
}
