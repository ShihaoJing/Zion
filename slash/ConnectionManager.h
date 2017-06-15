//
// Created by Shihao Jing on 6/14/17.
//

#ifndef SLASH_CONNECTIONMANAGER_H
#define SLASH_CONNECTIONMANAGER_H

#include <set>
#include "Connection.h"
namespace HTTP {
namespace Server {
class ConnectionManager {
public:
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;

  ///Construct a connection manager.
  ConnectionManager();

  // Add the specified connection to the manager and start it.
  void start(connection_ptr c);

  // Stop the specified connection.
  void stop(connection_ptr c);

  // Stop all connections.
  void stop_all();

private:
  // The managed connections.
  std::set<connection_ptr> connections_;
};

} // namespace HTTP
} // namespace Server
#endif //SLASH_CONNECTIONMANAGER_H
