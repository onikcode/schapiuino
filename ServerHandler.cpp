/*
  ServerHandler.cpp - Library for receive Http Requests and send Http responses.
  Created by Joan Ortega, jomaora@gmail.com Feb. 29, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include <Ethernet.h>
#include "ServerHandler.h"

void ServerHandler::init() {
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    Ethernet.begin(_mac, _ip);
    _server->begin();
    if (_activeLogs) {
        Serial.print("server is at ");
        Serial.println(Ethernet.localIP());
    }
}

bool ServerHandler::listenRequest() {
    _client = _server->available();
    if (!_client) {
        return false;
    } else {
        Serial.println("server available");
        readRequest();
        return true;
    }
}

void ServerHandler::sendResponse() {
    _client.println("HTTP/1.1 200 OK");
    _client.println("Content-Type: text/html");
    _client.println("Connection: close");  // the connection will be closed after completion of the response
    _client.println();
    _client.println("<!DOCTYPE HTML>");
    _client.println("<html>");
    _client.println("connected");
    _client.println("</html>");
    delay(1);
    _client.stop();

    Serial.println("client disonnected");
}

const char* ServerHandler::getHttpMethod() {
    return _requestLine.c_str();
}

const char* ServerHandler::getRequestHeaders() {
    return _requestHeaders.c_str();
}

const char* ServerHandler::getRequestBody() {
    return _requestBody.c_str();
}

void ServerHandler::readRequest() {
    boolean currentLineIsBlank = true;
    int countCLRF = 0;
    while (_client.connected() && !_requestReceived) {
        if (_client.available()) {
            char c = _client.read();
            if (c == '\n' || c == '\r') {
                _returnCharCount++;
            } else {
                _returnCharCount = 0;
            }
            if (_returnCharCount == 2) {
                _readingStatus = 1;
            }
            if (_returnCharCount == 4) {
                _readingStatus = 2;
            }

            switch (_readingStatus) {
                case 0:
                    _requestLine.concat(c);
                    break;
                case 1:
                    _requestHeaders.concat(c);
                    break;
                case 2:
                    _requestBody.concat(c);
                    break;
            }
        } else {
            _requestReceived = true;
            break;  // nothing more on buffer, exit while
        }
    }
}

void ServerHandler::resetRequest() {
    _readingStatus = 0;
    _returnCharCount = 0;
}