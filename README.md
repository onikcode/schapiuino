# schapiuino

## Overview

A basic library to handle a Client HTTP using an Arduino UNO and an Ethernet Shield.
It wrappes Ethernet setup, begin, connect, and send/receive request/response.

## Contents

- ClientHandler.h: the Client Handler Class definition file to create HTTP requests.
- ClientHandler.cpp: Client Handler implementation file
- examples:
	- ClientHandler_Basic_GET: a basic example making a GET to a defined serverUrl, path, using header Accept: text/plain.
	- ClientHandler_LCD: a basic example making a GET to server and showing response on LCD.
- ServerHandler.h: the Server Handler Class definition file to listen HTTP request and create responses.
- ServerHandler.cpp: Server Handler implementation file
- examples:
    - Server_Basic: Basic example that responses avec a basic HTML file on GET and POST method
    - Server_Servo: Example exposing an API to manage a Servo Motor.


## API

[Client Handler](#clientHandler)

[Server Handler](#serverHandler)

## <a name="clientHandler"></a>Client Handler

### ClientHandler(EthernetClient *client, byte mac[], int ipRanges[]) - Constructor
To init a ClientHandler object, you need to specify
* **client**: Pointer to an EthernetClient
* **mac**: Mac address to be used by the EthernetShield
* **ip**: Array containing the ranges to set the IP for the EthernetShield; Example [192, 0, 0, 1]

### bool  init()
To be used within the setup function. It initialises the EthernetShield with the values passed in the constructor

### bool    initClientHandler(String httpMethod, char *server, int severPort, String path)
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
Allow inner logs to be show in the serial monitor

###void    disableLogs()
Disable inner logs to be show in the serial monitor. This is the default behavior

## <a name="serverHandler"></a>Server Handler

### ServerHandler(EthernetServer *server, byte mac[], int ipRanges[]) - Constructor
To init a ClientHandler object, you need to specify:
* **server**: Pointer to an EthernetServer object
* **mac**: Mac address to be used by the EthernetShield
* **ip**: Array containing the ranges to set the IP for the EthernetShield; Example [192, 0, 0, 1]

### void    init()
To be used within the setup function. It initialises the EthernetShield with the values passed in the constructor.

### bool    listenRequest()
Listen if a request is incoming. If it is, it blocks the process and start reading the request.
When it returns true, the request is already read and ready to be analysed.

### String      getHttpMethod()
Returns the HTTP Method used by the incoming request.

### String      getRequestPath()
Returns the path used by the incoming request.

### const char* getRequestHeaders()
Returns all the headers given by the incoming request.

### const char* getRequestBody()
Returns the body (payload) sent in the incoming request.

### ServerHandler & buildResponse(int statusCode, String message = "")
Set the beginning of the response line, setting status code.
* **status**: HTTP status code to send in the response.
* **message**: (OPTIONAL): You can use another code (not in the http status codes list, in this case, set a message to explain it)

### ServerHandler & appendHeaderResponse(String header)
Add a header to the response.

### ServerHandler & appendBodyResponse(String body)
Add a body to the response if wanted. It should be called once!. If it is used, don't forget to add the "Content-Type" header before.

### void    send()
It ends the request line, close the connection and send the client the response.

*buildResponse, appendHeaderResponse and appendBodyResponse are **chainable** methods, so you can write your code like this :*

```
responseHandler
    .buildResponse(201)
    .appendHeaderResponse("Content-Type: application/json")
    .appendHeaderResponse("Connection: close")
    .appendBodyResponse(bodyMessage)
    .send();
```

### void    sendHTMLBasicResponse(String message)
Builds a basic response (setting 200 as status code and sending a basic HTML template) with the message passed as parameter.

### void    sendNonImplementedMethodResponse(String message)
Builds a basic 405 error response.
* **message**: (Optional): Message to be sent in the body. The default message is:  "Error: The used HTTP method have not been implemented on this endpoint."

### void    sendNotFoundResponse(String message)
Builds a basic 404 error response.
* **message**: (Optional): Message to be sent in the body. The default message is:  "Error: Resource not found."

### void    enableLogs()
Allow inner logs to be show in the serial monitor

###void    disableLogs()
Disable inner logs to be show in the serial monitor. This is the default behavior
