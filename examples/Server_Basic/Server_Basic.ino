#include <SPI.h>
#include <Ethernet.h>
#include <ServerHandler.h>

EthernetServer server(80);
byte mac[] = { 0xCE, 0xCD, 0xCE, 0xCF, 0xCE, 0xCD };
int ip[] = {192, 168, 1, 178};
ServerHandler responseHandler(&server, mac, ip);

void setup() {
    Serial.begin(9600);
    responseHandler.enableLogs();
    responseHandler.init();
}

void loop() {
    bool requestReceived = responseHandler.listenRequest();

    if (requestReceived) {
        String method = responseHandler.getHttpMethod();
        
        if (responseHandler.getRequestPath().equals("/")) {
          if (method.equals("GET")) {
            responseHandler
                .buildResponse(200)
                .appendHeaderResponse("Content-Type: text/plain")
                .appendHeaderResponse("Connection: close")
                .appendBodyResponse("You have sent a request and we received.")
                .send();
          } if (method.equals("POST")) {
            String bodyMessage = "You have sent a request and we received having as payload ";
            bodyMessage.concat(responseHandler.getRequestBody());
            responseHandler
                .buildResponse(200)
                .appendHeaderResponse("Content-Type: text/plain")
                .appendHeaderResponse("Connection: close")
                .appendBodyResponse(bodyMessage)
                .send();
          } else {
            responseHandler
                .buildResponse(405)
                .appendHeaderResponse("Content-Type: text/plain")
                .appendHeaderResponse("Connection: close")
                .appendBodyResponse("Error: Only GET and POST have been implemented on this endpoint.")
                .send();
          }
        } else {
            responseHandler.sendNotFoundResponse();
        }
    }
}