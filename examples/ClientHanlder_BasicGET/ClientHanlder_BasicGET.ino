#include <ClientHandler.h>
#include <SPI.h>
#include <Ethernet.h>

EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int ip[] = {192, 168, 1, 177};
ClientHandler requestHandler(&client, mac, ip);

int pinIn = 6;
int currentState = LOW;
int readState = LOW;
bool requestReady = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinIn, INPUT);
  requestHandler.init();
}

void loop() {
  readState = digitalRead(pinIn);
  if (currentState != readState) {
    //request.enableLogs();
    requestReady = requestHandler.initClientHandler("GET", "jomaora-restapi.herokuapp.com", 80, "/reviews");
    if (requestReady) {
      requestHandler.addHeader("Accept: text/plain");
      const char* response = requestHandler.send();
      Serial.print("API Response : ");
      Serial.println(response);
      Serial.println(requestHandler.getResponseStatusCode());
    }
    else {
      Serial.println("Error during set up request.");
    }
  }
}