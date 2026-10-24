#include <iostream>
using namespace std;

class HttpRequest {
    string url;
    string method;
    string body;
    string headers;
    string params;
    string cookies;

    HttpRequest(){}
public:
    friend class HttpRequestBuilder;

    void execute(){
        cout<<"Executing request with url: "<<url<<", method: "<<method<<", body: "<<body<<", headers: "<<headers<<", params: "<<params<<", cookies: "<<cookies<<endl;
    }
};

class HttpRequestBuilder {
    HttpRequest* req;
public:
    HttpRequestBuilder(){
        req = new HttpRequest();
    }

    HttpRequestBuilder withUrl(string url){
        req->url = url;
        return *this;
    }
    HttpRequestBuilder withMethod(string method){
        req->method = method;
        return *this;
    }
    HttpRequestBuilder withHeaders(string headers){
        req->headers = headers;
        return *this;
    }
    HttpRequestBuilder withParams(string params){
        req->params = params;
        return *this;
    }
    HttpRequestBuilder withBody(string body){
        req->body = body;
        return *this;
    }
    HttpRequestBuilder withCookies(string cookies){
        req->cookies = cookies;
        return *this;
    }
    HttpRequest* build(){
        if (req->url == "" || req->method == ""){
            throw new runtime_error("Url and Method shouldn't be empty");
        }
        if (req->method != "GET" && req->method != "HEAD" && req->body == "" ){
            throw new runtime_error("Without GET, HEAD, we need request body");
        }
        return req;
    }
};

int main(){
    HttpRequestBuilder builder;
    HttpRequest* req = builder.withUrl("https://test.com").withMethod("GET").build();

    req->execute();
    return 0;
}

