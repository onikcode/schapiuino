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
        Request(EthernetClient & client, bool requestSent = false){
        	client_ = client;
        	requestSent_ = requestSent;
        }

        bool send() const;
        void get();
        void post();

    private:
    	void readingResponse(char n);

    	EthernetClient client_;
    	
    	bool requestSent_;

    	/*Variables to store response from API*/
    	String status = "";
  		String headers = "";
  		String body = "";

  		/*Counter on \n and \r characters sent by the API in the response, to identify which element have been read*/
		int returnCharCount = 0;

};
#endif