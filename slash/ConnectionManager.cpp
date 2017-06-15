//
// Created by Shihao Jing on 6/14/17.
//

#include "ConnectionManager.h"

namespace HTTP {
namespace Server {

ConnectionManager::ConnectionManager()
{
}

void ConnectionManager::start(connection_ptr c)
{
  connections_.insert(c);
  c->start();
}

void ConnectionManager::stop(connection_ptr c)
{
  connections_.erase(c);
  c->stop();
}

void ConnectionManager::stop_all()
{
  for (auto c: connections_)
    c->stop();
  connections_.clear();
}

} // namespace HTTP
} // namespace Server