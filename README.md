# schapiuino

## Overview

A basic library to handle a Client HTTP using an Arduino UNO and an Ethernet Shield.
It wrappes Ethernet setup, begin, connect, and send/receive request/response.

## Contents

- Request.h: the Request Class definition file.
- Request.cpp: Request Class implementation file
- examples: 
	- GET: a basic example making a GET to a defined serverUrl, path, using header Accept: text/plain.

## API

### Request(EthernetClient *client, byte mac[], int ipRanges[]) - Constructor
To init a Request object, you need to specifiy
* **client**: Pointer to an EthernetClient
* **mac**: Mac address to be used by the EthernetShield
* **ip**: Array containing the ranges to set the IP for the EthernetShield; Exemple [192, 0, 0, 1]

### bool  init()
To be used within the setup function. It initialises the EthernetShield with the values passed in the constructor

### bool    initRequest(String httpMethod, char *server, int severPort, String path)
Set up the parameters to make an HTTP request. Currently it doesn't accept body for the request. It returns true if the client could be connected to the server. 

* **httpMethod**: Http method to be used in the request: GET, POST, PUT, DELETE, PATCH
* **server**: serverUrl or IP
* **serverPort**: serverPort, normally 80
* **path**: Path to the requested service

Attention: it is not because the request was initialise that it was already send. This method initialises the [request String](http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html) but it doesn't set up the end of it. **See send method**.

### void    addHeader(String header)
Allow addition of headers to make the request. The string cotains the key:value format. Example: "Accept:application/json"

### const char* send()
Sends the request to the server by adding "Connection: close" header and a CLRF to the request String. It returns the body catched in the server response in a const char* format.

### int     getResponseStatusCode()
Returns the Response's status code

### const char* getResponseBody()
Return the Response's body

### void    enableLogs()
Allow inner logs to be show in the serial moniteur

###void    disableLogs()
Disable inner logs to be show in the serial moniteur. This is the default behavior
