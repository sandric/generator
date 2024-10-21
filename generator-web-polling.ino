#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "sandfic poco";
const char* password = "aegsm333";

String serverName = "http://192.168.148.26:3000/generators/0/";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

uint8_t onPin = 21;
uint8_t offPin = 0;

void offButtons() {
  digitalWrite(onPin, LOW);
  digitalWrite(offPin, LOW);
}

void on() {
  Serial.println("HANDLE ON START");
  
  digitalWrite(onPin, HIGH);

  delay(1000);
  offButtons();

  Serial.println("HANDLE ON DONE");
}

void off() {
  Serial.println("HANDLE OFF START");

  digitalWrite(offPin, HIGH);

  delay(1000);
  offButtons();

  Serial.println("HANDLE OFF DONE");
}

void setup() {
  pinMode(onPin, OUTPUT);
  pinMode(offPin, OUTPUT);

  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 1 seconds, it will take 1 seconds before publishing the first reading.");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "ping?ip=" + WiFi.localIP().toString();
      Serial.println(serverPath);
      
      http.begin(serverPath.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0) {
        //Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);
        //String payload = http.getString();
        //Serial.println(payload);

        if(httpResponseCode == 200) {
          Serial.print("All good, do nothing.");
        }
        else if(httpResponseCode == 201) {
          Serial.print("Switching ON");
          on();
        }
        else if(httpResponseCode == 202) {
          Serial.print("Switching OFF");
          off();
        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
 
    lastTime = millis();
  }
}