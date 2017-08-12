![Zion logo](https://github.com/ShihaoJing/Zion/blob/master/logo.png)

Header only micro web framework implemented in C++11, inspired by python flask and Crow.

### Easy To Write and Setup

A minimal Zion application looks something like this.
```
#include "zion.h"
using namespce zion;
int main() {    
  zion::Zion app;
  ROUTE(app, "/")([](){
    return "Hello World!";
  });
}
```
1. First, we create a instance of Class Zion, here we use default constructor.
2. Next we use macro ROUTE to tell Zion what URL should be triggered to user-defined handler.
Handlers can be designated to a Lambda function.
3. Return value of handlers will be convert to HTTP response and send back to request end.

### Features
* Easy routing
    * similar to flask
    * syntax checking at compile time
* Bench marking
    * In development
* JSON parser
    * [rapidjson](https://github.com/miloyip/rapidjson)
* Header Only
* Middleware support
    * In development
* Use google test for unit test


### Routing
As you have seen above, 'ROUTE' macro receives two parameters, a instance of Zion and a URL, and returns a instance of routing rule.
Rule class have overloaded '()' operator to receive a Lambda function as handler of URL.

```
ROUTE(app, "/")([](){
    return "Hello World!";
});
  
ROUTE(app, "/index")([](){
    return "index page";
});
```

### Variable Rules
To add variable parts to a URL you can mark these special sections as <variable_type>. 
Such a part is then passed as a keyword argument to your function. 

```
ROUTE(app, "/name/<string>/id/<int>/weight/<float>")
([](std::string name, int id, float_t weight) {   // resp should be response type
    std::ostringstream os;
    os << "name is " << name << " id is " << id << " and weight is " << weight;
    return zion::response(os.str());
});
```

Following 3 types are supported by now
 - < int > : convert to integers
 - < float > : convert to floaing point values
 - < string > : convert to string variables
 
 In addition, you can access request data by passing request argument in handler.
```
ROUTE(app, "/hello")([](const request &req){
    string url = req.uri;
    string method = req.method;
    return "hello world!";
});
 ```
 
 ##### note: arguments type checking happens during compile, which allows you to check syntax before runtime.
 
### JSON request and response
Zion uses [rapidjson](https://github.com/miloyip/rapidjson) for JSON request and response parsing. As aways, you can use other json libraries.
```
ROUTE(app, "/json")([](const zion::request &req){
    // 1. parse json from request body.
    Document d;
    d.Parse(req.body.data());
    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
    
    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    return buffer.GetString();
});
```
More json examples: [rapidjson](https://github.com/miloyip/rapidjson)
 
 ### Rendering Templates
 Zion provides mustache template engine by default. However, you can easily implement or use other template engine.
 ```
 ROUTE(app, "/id/<string>")
([](std::string name) {
    mustache tmpl{"<html> <h1>Hello {{name}}!</h1> </html>"};
    return zion::response(tmpl.render({"name", name}));
});
 ```
 
 ### How to build
 Copy '/zion' to your include directory and include 'zion.h'
 
 #### Requirements
 * C++ compiler with C++11 support
 * boost library
 * CMake for build examples and tests
 
 ### Attributions
 Zion uses the following libraries.
 * [Crow](https://github.com/ipkn/crow)
 * [rapidjson](https://github.com/miloyip/rapidjson)
 * [http-parser](https://github.com/nodejs/http-parser)
 * [Mustache](https://github.com/kainjow/Mustache)

 
 
 
 
 
 
