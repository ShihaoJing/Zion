//
// Created by fanshiliang on 2017/6/22.
//
#include <boost/numeric/conversion/cast.hpp>
#include "request_handler.h"
#include <fstream>
#include <iostream>

namespace HTTP {
namespace Server {

    //constructor
    request_handler::request_handler(const std::string& doc_root)
    : doc_root_(doc_root)
    {
        std::cout << doc_root << std::endl;
    }

    void request_handler::handle_request(const request& req, response& rep)
    {
        std::string path;
        if (!url_decode(req.uri, path))
        {
            rep = response::stock_reply(response::bad_request);
            return;
        }
        if(path.empty() || path[0] != '/' || path.find("..") != std::string::npos)
        {
            rep = response::stock_reply(response::bad_request);
            return;
        }
        if (path[path.size()-1] == '/')
        {
            path += "index.html";
        }

        // find file extension. find the last dot, if last dot is behind last slash, it is valid and
        // file extension(type) is the string behind last dot
        std::size_t last_slash_pos = path.find_last_of("/");
        std::size_t last_dot_pos = path.find_last_of(".");
        std::string fileExtension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
        {
            fileExtension = path.substr(last_dot_pos+1);
        }

        // open the target file and send the file back in response content
        // file location is determined by root dir and path
        std::string absolute_path = doc_root_ + path;
        std::ifstream is(absolute_path, std::ios::in | std::ios::binary);
        if (!is)
        {
            rep = response::stock_reply(response::not_found);
            return;
        }

        // wrap file into buffer and send to client
        rep.status = response::ok;
        char buffer[512];
        while (is.read(buffer, sizeof(buffer)).gcount() > 0)
        {
            rep.content.append(buffer, boost::numeric_cast<long>(is.gcount()));
        }
        rep.headers.resize(2);
        rep.headers[0].key = "Content-Length";
        rep.headers[0].value = std::to_string(rep.content.size());
        rep.headers[1].key = "Content-Type";
        rep.headers[1].value = MIME::extension_to_mime(fileExtension);
    }

    bool request_handler::url_decode(const std::string& in, std::string& out)
    {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); i++)
        {
            if (in[i] == '+')
            {
                out += ' ';
            }
            else if (in[i] == '%')
            {
                if (i + 3 <= in.size())
                {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value)
                    {
                        out += static_cast<char>(value);
                        i = i + 2;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                out += in[i];
            }
        }
        return true;
    }
}
}