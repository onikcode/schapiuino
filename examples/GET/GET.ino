#include <Request.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192,168,1,177);
EthernetClient client;

int switchState = 0;
bool response = false;

void initEthernet() {
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("Ethernet Shield connected");
}

void setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  initEthernet();
}

void loop() {
  switchState = digitalRead(2);
  if (switchState == HIGH) {
    digitalWrite(3, HIGH);
    Request request = Request(client, "jomaora-restapi.herokuapp.com", 80);
    response = request.send();
  }
  
  if (response) {
    Serial.print("Response received");
    digitalWrite(3, LOW);    
    client.stop();
    response = false;
  }
}