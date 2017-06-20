//
// Created by Shihao Jing on 6/14/17.
//

#ifndef SLASH_CONNECTIONMANAGER_H
#define SLASH_CONNECTIONMANAGER_H

#include <set>
#include "connection.h"
namespace HTTP {
namespace Server {
class connection_manager {
public:
  typedef std::shared_ptr<connection> connection_ptr;
  connection_manager(const connection_manager&) = delete;
  connection_manager& operator=(const connection_manager&) = delete;

  ///Construct a connection manager.
  connection_manager();

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
