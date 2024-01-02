/*
  ServerHandler.cpp - Library for receive Http Requests and send Http responses.
  Created by Joan Ortega, jomaora@gmail.com Feb. 29, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include <Ethernet.h>
#include "ServerHandler.h"

void ServerHandler::init() {
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
        if (_activeLogs) {
            Serial.println("incomming request");
        }
        resetRequest();
        readRequest();
        return true;
    }
}

void ServerHandler::sendHTMLBasicResponse(String message) {
    if (_requestReceived) {
        _client.println("HTTP/1.1 200 OK");
        _client.println("Content-Type: text/html");
        _client.println("Connection: close");
        _client.println();
        _client.println("<!DOCTYPE HTML>");
        _client.println("<html>");
        _client.println(message);
        _client.println("</html>");
        delay(1);
        _client.stop();
        if (_activeLogs) {
            Serial.println("client disconnected");
        }
    }
}

ServerHandler & ServerHandler::buildResponse(int statusCode, String message) {
    if (_requestReceived) {
        parseStatusCode(statusCode, message);
    }
    return *this;
}

ServerHandler & ServerHandler::appendHeaderResponse(String header) {
    if (_requestReceived) {
        _client.println(header);
    }
    return *this;
}

ServerHandler & ServerHandler::appendBodyResponse(String body) {
    if (_requestReceived) {
        _client.println();
        _client.println(body);
    }
    return *this;
}

void ServerHandler::send() {
    if (_requestReceived) {
        delay(1);
        _client.stop();
        if (_activeLogs) {
            Serial.println("client disconnected");
        }
    }
}

String ServerHandler::getHttpMethod() {
    if (_requestReceived) {
        int blankSpaceIndex = _requestLine.indexOf(" ");
        String httpMethod = _requestLine.substring(0, blankSpaceIndex);
        return _requestLine.substring(0, blankSpaceIndex);
    } else {
        return "";
    }
}

String ServerHandler::getRequestPath() {
    if (_requestReceived) {
        int firstBlankSpaceIndex = _requestLine.indexOf(" ") + 1;
        int secondBlankSpaceIndex = _requestLine.indexOf(" ", firstBlankSpaceIndex);
        String path = _requestLine.substring(firstBlankSpaceIndex, secondBlankSpaceIndex);
        return _requestLine.substring(firstBlankSpaceIndex, secondBlankSpaceIndex);
    } else {
        return "";
    }
}

const char* ServerHandler::getRequestHeaders() {
    return _requestHeaders.c_str();
}

const char* ServerHandler::getRequestBody() {
    return _requestBody.c_str();
}

void ServerHandler::readRequest() {
    // boolean currentLineIsBlank = true; unused variable
    // int countCLRF = 0; unused variable
    while (_client.connected() && !_requestReceived) {
        if (_client.available()) {         
            char c = _client.read();
            Serial.write(c);  
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
                    if (c != '\n') {
                        _requestBody.concat(c);
                    }
                    break;
            }
        } else {
            _requestReceived = true;
        }
    }  
}

void ServerHandler::resetRequest() {
    _readingStatus = 0;
    _returnCharCount = 0;
    _requestLine = "";
    _requestBody = "";
    _requestHeaders = "";
    _requestReceived = false;
}

void ServerHandler::parseStatusCode(int status, String message) {
    _client.print("HTTP/1.1 ");
    _client.print(status);
    switch (status) {
        case 200:
            _client.println("OK");
            break;
        case 201:
            _client.println("Accepted");
            break;
        case 204:
            _client.println("No Content");
            break;
        case 400:
            _client.println("Bad Request");
            break;
        case 401:
            _client.println("Unauthorize");
            break;
        case 403:
            _client.println("Forbidden");
            break;
        case 404:
            _client.println("Not Found");
            break;
        case 405:
            _client.println("Method Not Allowed");
            break;
        case 406:
            _client.println("Not Acceptable");
            break;
        case 500:
            _client.println("Internal server error");
            break;
        default:
            _client.println(message);
            break;
    }
}

void ServerHandler::sendNonImplementedMethodResponse(String message) {
    if (_requestReceived) {
        buildResponse(405)
            .appendHeaderResponse("Content-Type: text/plain")
            .appendHeaderResponse("Connection: close")
            .appendBodyResponse(message)
            .send();
        delay(1);
        _client.stop();
    }
}

void ServerHandler::sendNotFoundResponse(String message) {
    if (_requestReceived) {
        buildResponse(405)
            .appendHeaderResponse("Content-Type: text/plain")
            .appendHeaderResponse("Connection: close")
            .appendBodyResponse(message)
            .send();
        delay(1);
        _client.stop();
    }
}