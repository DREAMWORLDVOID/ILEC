//
//  fileio.h
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#ifndef fileio_h
#define fileio_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace IO{
    string read(string path){
        ifstream t(path);
        stringstream buffer;
        buffer << t.rdbuf();
        string contents(buffer.str());
        if (contents.compare("") == 0)
            return {(char)error::bad_io};
        for (int i = 0; i < contents.length(); ++i){
            contents[i] = tolower(contents[i]);
        }
        return contents;
    }
}

#endif /* fileio_h */
