/*
  Request.cpp - Library for create Http Requests.
  Created by Joan Ortega, jomaora@gmail.com Feb. 15, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Request.h"

void Request::get() {
  client_.println("GET /reviews HTTP/1.1");
  client_.println("Host: jomaora-restapi.herokuapp.com");
  client_.println("Accept: text/plain");
  client_.println("Connection: close");
  client_.println();  
}

void Request::post() {
  String PostData="{\"name\":\"test-arduino-depuis-ekino\",";
  PostData=PostData+"\"placeType\":\"ekino\",";
  PostData=PostData+"\"stars\":3";
  PostData=PostData+"}";
  
  client_.println("POST /reviews HTTP/1.1");
  client_.println("Host: jomaora-restapi.herokuapp.com");
  client_.println("Accept: application/json");
  client_.println("User-Agent: Arduino/1.0");
  client_.println("Content-Type: application/json");
  client_.println("Connection: close");
  client_.print("Content-Length: ");
  client_.println(PostData.length());
  client_.println();
  client_.println(PostData);
}


void Request::readingResponse(char c) {
  if (c == '\n' || c == '\r') {
    returnCharCount++;
  } else {
    returnCharCount = 0;
  }
  /*Serial.print(c);
  Serial.print(returnCharCount);
  Serial.print(readingheaders);
  Serial.println(readingBody);
  */
  if (returnCharCount == 2) {
    readingStatus = false;
    readingheaders = true;
    readingBody = false;
  }
  if (returnCharCount == 4) {
    readingStatus = false;
    readingheaders = false;
    readingBody = true;
  }
  
  if (readingStatus) {
    status.concat(c);
  }
  if (readingBody) {
    body.concat(c);
  } 
  if (readingheaders) {
    headers.concat(c);  
  }
}

bool Request::send() {
  headers = "";
  body = "";
  returnCharCount = 0;
  
  int connected = 0;
  Serial.println(server_);
  Serial.println(serverPort_);
  while (!connected) {
    if (client_.connect(server_, serverPort_)) {
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
    char c = client_.read();
    readingResponse(c);
    // lecture de la reponse
    if (!client_.available() && !client_.connected()) {
      receivedResponse = true;
    }
  }

  return requestSent_;
}