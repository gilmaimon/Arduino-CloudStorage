#pragma once

#include "RequestInterface.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

namespace http {
  class Esp8266RequestImpl : public RequestInterface {
  public:
    Esp8266RequestImpl() : RequestInterface() {}

    void setUrl(String url) override {
      WiFiClient client;
      _http.begin(client, url);
    }

    void setMethod(Method m) override {
      _method = m;
    }

    void setBody(String body) override {
      this->_body = body;
    }

    void addHeader(String key, String value) override {
      _http.addHeader(key, value);
    }
    
    Response execute() override {
      int httpCode = -1;
      if(this->_method == Method::GET) httpCode = this->_http.sendRequest("GET", this->_body);
      else if(this->_method == Method::POST) httpCode = this->_http.sendRequest("POST", this->_body);
      
      Response response;
      response.statusCode = httpCode;
      
      if(httpCode == 200) response.body = this->_http.getString();
      else response.body = this->_http.errorToString(httpCode);

      this->_http.end();
      
      return response;
    }

    virtual ~Esp8266RequestImpl() {}

  private:
    Method _method;
    String _body, _url;
    HTTPClient _http;
  };
};