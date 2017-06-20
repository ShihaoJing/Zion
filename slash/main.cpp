//
// Created by Shihao Jing on 5/31/17.
//

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server.h"
#include <iostream>

int StartUp(unsigned short &port)
{
  int ServerSock = -1;
  ServerSock = socket(AF_INET, SOCK_STREAM, 0);
  if (!ServerSock)
  {
    //todo: handle error
    exit(-1);
  }

  sockaddr_in Address = {};
  Address.sin_family = AF_INET;
  Address.sin_port = htons(port);
  Address.sin_addr.s_addr = INADDR_ANY;

  if (bind(ServerSock, (sockaddr*)&Address, sizeof(Address)) < 0)
  {
    //todo: handle error
  }

  if (listen(ServerSock, 3) < 0)
  {
    //todo: handle error
    exit(EXIT_FAILURE);
  }

  return ServerSock;
}

int main(int argc, char const *argv[])
{
  /*int ServerSock = -1;
  unsigned short port = 8080;

  ServerSock = StartUp(port);
  printf("server running on port %d\n", port);

  int ClientSock = -1;
  sockaddr_in ClientAddr = {};
  int ClientAddrLen = sizeof(ClientAddr);

  bool isRunning = true;
  while (isRunning)
  {
    ClientSock = accept(ServerSock, (sockaddr*)&ClientAddr, (socklen_t*)&ClientAddrLen);
    if (ClientSock == -1)
    {
      //todo: handle error
    }

    //todo: Accetp request
  }*/
  std::cout << "start listening on 8080" << std::endl;
  HTTP::Server::Server server("127.0.0.1", "8080", "./");
  server.run();
}