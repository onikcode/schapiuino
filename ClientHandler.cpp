/*
  ClientHandler.cpp - Library for create Http Requests.
  Created by Joan Ortega, jomaora@gmail.com Feb. 15, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include <Ethernet.h>
#include "ClientHandler.h"

bool ClientHandler::init() {
  _requestReady = false;
  String result = "";
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  int beginOK = Ethernet.begin(_mac);
  if (beginOK == 0) {
    result = "Failed to configure Ethernet using DHCP";
    Serial.println(result);
    Ethernet.begin(_mac, _ip);
  }
  delay(1000);
  result = "Ethernet Shield connected";
  Serial.println(result);
  return (beginOK != 0);
}

bool ClientHandler::initClientHandler(String httpMethod, char *server, int serverPort, String path) {
  _server = server;
  _path = path;
  resetRequest();

  int connected = 0;
  while (!connected) {
    Serial.println("Trying connection");
    if (_client->connect(_server, serverPort)) {
      Serial.println("connected");
      connected = 1;
    } 
    else {
      Serial.println("connection failed");
      _requestReady = false;
      return _requestReady;
    }
  }

  String requestLine = httpMethod + " ";
  requestLine.concat(path);
  requestLine.concat(" HTTP/1.1");
  String hostLine = "Host: " + String(server);
  _client->println(requestLine);
  _client->println(hostLine);

  _requestReady = true;
  return _requestReady;
}

void ClientHandler::addHeader(String header) {
  if (_requestReady) {
    _client->println(header);  
  }
}

const char* ClientHandler::send() {
  if (_requestReady) {
    _client->println("Connection: close");
    _client->println();

    buildResponse();

    if (_activeLogs) {
      Serial.println("disconnecting.");
      Serial.println("");
      Serial.println("status");
      Serial.println(_status);
      Serial.println("");
      Serial.print("headers");
      Serial.print(_responseHeaders);
      Serial.println("");
      Serial.print("body");
      Serial.println(_body);
    }

    _client->stop();

    return _body.c_str();
  }
  else {
    return "Request is not ready to be sent";
  }
}

void ClientHandler::buildResponse() {
  // values needed before reading response
  _returnCharCount = 0; _readingStatus = true;  _readingheaders = false;  _readingBody = false;
  while(!_receivedResponse) {
    if (_client->available()) {
      char c = _client->read();
      readingResponse(c);
    }
    if (!_client->available() && !_client->connected()) {
      _receivedResponse = true;
    }
  }
}

void ClientHandler::readingResponse(char c) {
  if (c == '\n' || c == '\r') {
    _returnCharCount++;
  } else {
    _returnCharCount = 0;
  }

  if (_returnCharCount == 2) {
    _readingStatus = false; _readingheaders = true;   _readingBody = false;
  }
  if (_returnCharCount == 4) {
    _readingStatus = false; _readingheaders = false;  _readingBody = true;
  }
  
  if (_readingStatus) {
    _status.concat(c);
  }
  if (_readingBody && c != '\n') {
    _body.concat(c);
  } 
  if (_readingheaders) {
    _responseHeaders.concat(c);  
  }
}

int ClientHandler::getResponseStatusCode() {
  if (_receivedResponse) {
    int blankSpaceIndex = _status.indexOf(" ") + 1;
    String statusCode = _status.substring(blankSpaceIndex, blankSpaceIndex + 3);
    return statusCode.toInt();  
  } else {
    return 0;
  }
}

const char* ClientHandler::getResponseBody() {
  if (_receivedResponse) {
    return _body.c_str();  
  } else {
    return "";
  }
}

void ClientHandler::resetRequest() {
  _receivedResponse = false;  // setting the flag that indicates the response recpetion to false;
  _body = "";
  _status = "";
  _responseHeaders = "";
}