/*
  Request.h - Library for create Http Requests.
  Created by Joan Ortega, jomaora@gmail.com Feb. 15, 2016.
  Released into the public domain.
*/

#ifndef Request_h
#define Request_h

#include <Ethernet.h>
#include "Arduino.h"

class Request
{
    public:
        Request(EthernetClient & client, char* server, int serverPort, bool requestSent = false){
        	client_ = client;
        	server_ = server;
        	serverPort_ = serverPort;
        	requestSent_ = requestSent;
        }

        bool send();
        void get();
        void post();

    private:
    	void readingResponse(char n);

    	EthernetClient client_;
    	char* server_;
    	int serverPort_;
    	
    	bool requestSent_;
    	bool readingStatus = false;
    	bool readingheaders = false;
    	bool readingBody = false;

    	/*Variables to store response from API*/
    	String status = "";
  		String headers = "";
  		String body = "";

  		/*Counter on \n and \r characters sent by the API in the response, to identify which element have been read*/
		int returnCharCount = 0;

};
#endif