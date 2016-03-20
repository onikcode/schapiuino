#include <SPI.h>
#include <Ethernet.h>
#include <ServerHandler.h>
#include <Servo.h>

// Ethernet configuration
EthernetServer server(80);
byte mac[] = { 0xCE, 0xCD, 0xCE, 0xCF, 0xCE, 0xCD };
int ip[] = {192, 168, 1, 178};
ServerHandler responseHandler(&server, mac, ip);
String bodyMessage;

//ServoMotor configuration
Servo myServo;
int angle;

void setup() {
    Serial.begin(9600);
    myServo.attach(9);   //digital port for white cable on servo motor
    responseHandler.enableLogs();
    responseHandler.init();
}

void loop() {
    bool requestReceived = responseHandler.listenRequest();

    if (requestReceived) {
        Serial.println(responseHandler.getHttpMethod());
        Serial.println(responseHandler.getRequestPath());
        String method = responseHandler.getHttpMethod();
        if (responseHandler.getRequestPath().equals("/servomotor/angle")) {
          if (method.equals("GET")) {
            bodyMessage = "{\"angle\": ";  bodyMessage.concat(angle);  bodyMessage.concat("}");
            responseHandler
                .buildResponse(200)
                .appendHeaderResponse("Access-Control-Allow-Origin: *")
                .appendHeaderResponse("Access-Control-Allow-Credentials: true")
                .appendHeaderResponse("Content-Type: application/json")
                .appendHeaderResponse("Connection: close")
                .appendBodyResponse(bodyMessage)
                .send();
          }
          else if (method.equals("POST")) {
            String bodyMessage = "Servo motor will turn from ";  bodyMessage.concat(angle);
            bodyMessage.concat(" to ");
            angle = atoi(responseHandler.getRequestBody());
            bodyMessage.concat(angle);

            Serial.println(responseHandler.getRequestBody());
            Serial.println(angle);

            responseHandler
                .buildResponse(201)
                .appendHeaderResponse("Access-Control-Allow-Origin: *")
                .appendHeaderResponse("Access-Control-Allow-Credentials: true")
                .appendHeaderResponse("Content-Type: text/plain")
                .appendHeaderResponse("Connection: close")
                .appendBodyResponse(bodyMessage)
                .send();

          } else {
            responseHandler.sendNonImplementedMethodResponse();
          }
        }
        else if (responseHandler.getRequestPath().equals("/servomotor/reset")) {
          if (method.equals("POST") || method.equals("OPTIONS")) {
            angle = 0;
            bodyMessage = "{\"angle\": ";  bodyMessage.concat(angle);  bodyMessage.concat("}");
            responseHandler
                .buildResponse(201)
                .appendHeaderResponse("Access-Control-Allow-Origin: *")
                .appendHeaderResponse("Access-Control-Allow-Credentials: true")
                .appendHeaderResponse("Content-Type: application/json")
                .appendHeaderResponse("Connection: close");
            if (method.equals("POST")) {
                responseHandler.appendBodyResponse(bodyMessage);
            }
            responseHandler.send();
          } else {
            responseHandler.sendNonImplementedMethodResponse();
          }
        }
        else {
            responseHandler.sendNotFoundResponse();
        }
    }
    myServo.write(angle);
    delay(1000);
}