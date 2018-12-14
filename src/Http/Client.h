#pragma once

namespace http {
  enum class Method {
    GET, POST
  };

  struct Response {
    int statusCode;
    String body;
  };

  template <class ClientImpl>
  class Request {
  public:
    Request(String url, Method m, String body = "") {
      _impl.setUrl(url);
      _impl.setMethod(m);
      _impl.setBody(body);
    }

    void setBody(String body) {
      _impl.setBody(body);
    }

    void addHeader(String key, String value) {
      _impl.addHeader(key, value);
    }

    Response execute() {
      return _impl.execute();
    }

  private:
    ClientImpl _impl; 
  };
};