#pragma once

namespace http {
  class RequestInterface {
  public:
    virtual void setUrl(String url) = 0;
    virtual void setMethod(Method m) = 0;
    virtual void setBody(String body) = 0;
    virtual void addHeader(String key, String value) = 0;
    virtual Response execute() = 0;
  };
}