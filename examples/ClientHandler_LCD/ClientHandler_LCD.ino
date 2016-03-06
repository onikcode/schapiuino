#include <Request.h>
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <LCDHandler.h>

// Ethernet Client objects
EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int ip[] = {192, 168, 1, 177};
Request request(&client, mac, ip);

// LCD objects
LiquidCrystal lcd(8, 9, 5, 7, 3, 2);
LCDHandler lcdHandler(&lcd, 300);

// Communication pins
int pinIn = 6;
int currentState = LOW;
int readState = LOW;
bool requestReady = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinIn, INPUT);
  lcdHandler.init("Ethernet set up", "please wait...");
  if (request.init()) {
    lcdHandler.clear();
    lcdHandler.print("Press to request");
  }
}

void loop() {
  readState = digitalRead(pinIn);
  if (currentState != readState) {
    requestReady = request.initRequest("GET", "jomaora-restapi.herokuapp.com", 80, "/reviews/55ad2d1e284b240300aa9796");
    if (requestReady) {
      request.addHeader("Accept: text/plain");
      const char* response = request.send();
      lcdHandler.print("Sending...");
      lcdHandler.clear();
      lcdHandler.print(response);
      
      int status = request.getResponseStatusCode();
      lcdHandler.print(status, 1);
      lcdHandler.clear(5000);
      lcdHandler.print("Press to request");
    }
    else {
      lcdHandler.print("Error during set up request.");
    }
  }
}