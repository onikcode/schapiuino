/*
  ClientHandler.h - Library for create and send Http Requests.
  Created by Joan Ortega, jomaora@gmail.com Feb. 15, 2016.
  Released into the public domain.
*/

#ifndef ClientHandler_h
#define ClientHandler_h

#include "Arduino.h"
#include <Ethernet.h>

class ClientHandler
{
    public:
        ClientHandler(EthernetClient *client, byte mac[], int ipRanges[]){
            _client = client;
            for (int i = 0; i < 6; i++) {
                _mac[i] = mac[i];
            }
            _ip = IPAddress(ipRanges[0], ipRanges[1], ipRanges[2], ipRanges[3]);
            _requestReady = false;
        }

        bool    init();
        bool    initClientHandler(String httpMethod, char *server, int severPort, String path);
        void    addHeader(String header);
        const char* send();
        
        void    setPath(String path) { _path = path; }
        String  getPath() { return _path; }
        int     getResponseStatusCode();
        const char* getResponseBody();

        void    enableLogs() { _activeLogs = true; }
        void    disableLogs() { _activeLogs = false; }

    private:
        void readingResponse(char n);
        void buildResponse();
        void resetRequest();

        EthernetClient *_client;
        byte _mac[6];
        IPAddress _ip;

        char*  _server;
        int    _serverPort;
        String _path;
        String _headers[6];

        bool _requestReady = false;
        bool _receivedResponse = false;

        bool _requestSent;
        bool _readingStatus = false;
        bool _readingheaders = false;
        bool _readingBody = false;

        /*Variables to store response from API*/
        String _status = "";
        String _body = "";
        String _responseHeaders = "";

        /*Counter on \n and \r characters sent by the API in the response, to identify which element have been read*/
        int _returnCharCount = 0;

        bool  _activeLogs = false;
};
#endif