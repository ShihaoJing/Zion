//
// Created by fanshiliang on 2017/6/23.
//

#include "mime.h"

namespace HTTP {
namespace Server {
namespace MIME {

struct mapping
{
    const char* extension;
    const char* mime;
} mappings[] =
{
        { "gif", "image/gif" },
        { "htm", "text/html" },
        { "html", "text/html" },
        { "jpg", "image/jpeg" },
        { "png", "image/png" }
};

std::string extension_to_mime(std::string extension)
{
    for (mapping m: mappings)
    {
        if (m.extension == extension)
        {
            return m.mime;
        }
    }
    return "text/plain";
}

}//namespace MIME
}//namespace Server
}//namespace HTTP