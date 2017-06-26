//
// Created by fanshiliang on 2017/6/23.
//

#ifndef ZION_MIME_H
#define ZION_MIME_H

#include <string>

namespace zion {
namespace MIME {

struct mapping
{
  const char* extension;
  const char* mime;
}

mappings[] =
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

} //namespace MIME
} // namespace zion
#endif //ZION_MIME_H
