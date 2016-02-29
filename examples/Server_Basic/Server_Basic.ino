#include <SPI.h>
#include <Ethernet.h>
#include <ServerHandler.h>

EthernetServer server(80);
byte mac[] = { 0xCE, 0xCD, 0xCE, 0xCF, 0xCE, 0xCD };
int ip[] = {192, 168, 1, 178};
ServerHandler responseHandler(&server, mac, ip);

void setup() {
    responseHandler.enableLogs();
    responseHandler.init();
}

void loop() {
    bool requestReceived = responseHandler.listenRequest();

    if (requestReceived) {
        Serial.println(responseHandler.getHttpMethod());
        Serial.println(responseHandler.getRequestHeaders());
        Serial.println(responseHandler.getRequestBody());
        responseHandler.sendResponse();
    }
}