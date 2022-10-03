#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

void ScanWiFi ();

#define GSRPin 32

//Kunal’s iPhone //kennylin0124
const char* ssid = "Kunal’s iPhone";
const char* password =  "12345678";

int sensorValue, GSRAverage;
  
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
  
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  
  if(type == WS_EVT_CONNECT){
  
    Serial.println("Websocket client connection received");
     
  } else if(type == WS_EVT_DISCONNECT){
 
    Serial.println("Client disconnected");
  
  }
}
  
void setup(){
  Serial.begin(115200);

  pinMode(GSRPin, INPUT);

   WiFi.mode(WIFI_STA);
   ScanWiFi();
   WiFi.begin(ssid, password);
   Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }


  Serial.print("WebSocket Local IP Address: ");
  Serial.println(WiFi.localIP());
  
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();

}
  
void loop(){

  long sum=0;
  for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch
      {
      sensorValue=analogRead(GSRPin);
      sum += sensorValue;
      delay(5);
      }
   GSRAverage = sum/10;
   
   Serial.println(GSRAverage);
   
   String toSent =  String (GSRAverage);
   ws.textAll(toSent+";");
 
  delay(250);
}

void ScanWiFi()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}
