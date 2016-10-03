//
//  fileio.h
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#ifndef fileio_h
#define fileio_h

#include <string>
#include <fstream>
#include <sstream>

namespace IO
{
    std::string readText(std::string path)
    {
        std::ifstream t(path);
        if (t.good())
        {
            std::stringstream buffer;
            buffer << t.rdbuf();
            return buffer.str();
        }
        else
            throw error::runtime_error(error::error_id::bad_io);
        return "";
    }
}

#endif /* fileio_h */
