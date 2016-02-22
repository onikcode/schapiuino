/*
  Request.cpp - Library for create Http Requests.
  Created by Joan Ortega, jomaora@gmail.com Feb. 15, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include <Ethernet.h>
#include "Request.h"

String Request::init() {
  _requestReady = false;
  String result = "";
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (Ethernet.begin(_mac) == 0) {
    result = "Failed to configure Ethernet using DHCP";
    Serial.println(result);
    Ethernet.begin(_mac, _ip);
  }
  delay(1000);
  result = "Ethernet Shield connected";
  Serial.println(result);
  return result;
}

void Request::initRequest(String httpMethod, char *server, int serverPort, String path) {
  _server = server;
  _path = path;

  int connected = 0;
  Serial.println(_server);
  Serial.println(serverPort);
  while (!connected) {
    if (_client->connect(_server, serverPort)) {
      Serial.println("connected");
      connected = 1;
    } 
    else {
      Serial.println("connection failed");
    }
  }

  String requestLine = httpMethod + " ";
  requestLine.concat(path);
  requestLine.concat(" HTTP/1.1");
  String hostLine = "Host: " + String(server);
  _client->println(requestLine);
  _client->println(hostLine);

  _requestReady = true;
}

void Request::addHeader(String header) {
  if (_requestReady) {
    _client->println(header);  
  }
}

void Request::send() {
  if (_requestReady) {
    _client->println("Connection: close");
    _client->println();

    buildResponse();
  }
}

void Request::buildResponse() {
  bool receivedResponse = false;
  while(!receivedResponse) {
    char c = _client->read();
    readingResponse(c);
    if (!_client->available() && !_client->connected()) {
      receivedResponse = true;
    }
  }
}

void Request::readingResponse(char c) {
  if (c == '\n' || c == '\r') {
    _returnCharCount++;
  } else {
    _returnCharCount = 0;
  }
  /*Serial.print(c);
  Serial.print(returnCharCount);
  Serial.print(readingheaders);
  Serial.println(readingBody);
  */
  if (_returnCharCount == 2) {
    _readingStatus = false;
    _readingheaders = true;
    _readingBody = false;
  }
  if (_returnCharCount == 4) {
    _readingStatus = false;
    _readingheaders = false;
    _readingBody = true;
  }
  
  if (_readingStatus) {
    _status.concat(c);
  }
  if (_readingBody) {
    _body.concat(c);
  } 
  if (_readingheaders) {
    _responseHeaders.concat(c);  
  }
}

/*void Request::get() {
  _client.println("GET /reviews HTTP/1.1");
  _client.println("Host: jomaora-restapi.herokuapp.com");
  _client.println("Accept: text/plain");
  _client.println("Connection: close");
  _client.println();  
}

void Request::post() {
  String PostData="{\"name\":\"test-arduino-depuis-ekino\",";
  PostData=PostData+"\"placeType\":\"ekino\",";
  PostData=PostData+"\"stars\":3";
  PostData=PostData+"}";
  
  _client.println("POST /reviews HTTP/1.1");
  _client.println("Host: jomaora-restapi.herokuapp.com");
  _client.println("Accept: application/json");
  _client.println("User-Agent: Arduino/1.0");
  _client.println("Content-Type: application/json");
  _client.println("Connection: close");
  _client.print("Content-Length: ");
  _client.println(PostData.length());
  _client.println();
  _client.println(PostData);
}*/

/*bool Request::send() {
  headers = "";
  body = "";
  returnCharCount = 0;
  
  int connected = 0;
  Serial.println(server_);
  Serial.println(serverPort_);
  while (!connected) {
    if (_client.connect(server_, serverPort_)) {
      Serial.println("connected");
      connected = 1;
    } 
    else {
      Serial.println("connection failed");
    }
  }
  
  Serial.println("Sending request");
  get();
  //post();
  delay(1000);
  
  requestSent_ = true;

  bool receivedResponse = false;
  while(!receivedResponse) {
    char c = _client.read();
    readingResponse(c);
    // lecture de la reponse
    if (!_client.available() && !_client.connected()) {
      receivedResponse = true;
      Serial.println("disconnecting.");
      Serial.println("");
      Serial.println("status");
      Serial.println(status);
      Serial.println("");
      Serial.print("headers");
      Serial.print(headers);
      Serial.println("");
      Serial.print("body");
      Serial.print(body);
    }
  }

  return requestSent_;
}*/