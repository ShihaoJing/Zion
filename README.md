# Slash 
micro web framework

1. Create server socket for connection
    * Async connect, read, write
    * Boost asio http://www.boost.org/doc/libs/1_64_0/doc/html/boost_asio.html
2. Read from socket and parse http request
    * http_parser https://github.com/nodejs/http-parser or write our own
3. Routing
    * request handler
4. Send response
    * response handler