//
// Created by fanshiliang on 2017/6/23.
//

#ifndef SLASH_HEADER_H
#define SLASH_HEADER_H

#include <string>

namespace HTTP {
namespace Server {
    struct header
    {
        std::string key;
        std::string value;
    };
}//namespace Server
}//namespace HTTP
#endif //SLASH_HEADER_H
