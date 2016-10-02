//
//  error.h
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#ifndef error_h
#define error_h

namespace error {
    const int not_found = -1;
    const int ambiguity = not_found - 1;
    const int bad_io = ambiguity - 1;
}

#endif /* error_h */
