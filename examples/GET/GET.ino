#include <Request.h>
#include <SPI.h>
#include <Ethernet.h>

EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int ip[] = {192, 168, 1, 177};
Request request(&client, mac, ip);

int pinIn = 6;
int currentState = LOW;
int readState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(pinIn, INPUT);
  request.init();
}

void loop() {
  readState = digitalRead(pinIn);
  if (currentState != readState) {
    request.initRequest("GET", "jomaora-restapi.herokuapp.com", 80, "/reviews");
    request.addHeader("Accept: text/plain");
    request.send();
  }
}