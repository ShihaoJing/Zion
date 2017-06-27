//
// Created by Shihao Jing on 6/14/17.
//

#ifndef ZION_CONNECTIONMANAGER_H
#define ZION_CONNECTIONMANAGER_H

#include <set>
#include "connection.h"

namespace zion {

template <typename Handler>
class connection_manager {
public:
  typedef std::shared_ptr<connection<Handler, connection_manager>> connection_ptr;
  connection_manager(const connection_manager&) = delete;
  connection_manager& operator=(const connection_manager&) = delete;

  ///Construct a connection manager.
  connection_manager()
  {
  }

  // Add the specified connection to the manager and start it.
  void start(connection_ptr c) {
    connections_.insert(c);
    c->start();
  }

  // Stop the specified connection.
  void stop(connection_ptr c) {
    connections_.erase(c);
    c->stop();
  }

  // Stop all connections.
  void stop_all() {
    for (auto c: connections_)
      c->stop();
    connections_.clear();
  }

private:
  // The managed connections.
  std::set<connection_ptr> connections_;
};

} // namespace zion
#endif //ZION_CONNECTIONMANAGER_H
