//
//  error.h
//  Compiler
//
//  Created by 佐々木みこと on 2016/10/2.
//  Copyright © 2016年 Infinideastudio. All rights reserved.
//

#ifndef error_h
#define error_h

#include <exception>
#include <string>

namespace error
{
    enum class error_id : size_t
    {
        not_found, ambiguity, bad_io
    };
    
    const std::string errors[] =
    {
        "not found", "ambiguity", "bad_io"
    };
    
    class runtime_error : std::exception
    {
    public:
        runtime_error(error_id i) noexcept : m_id(i) {}
        virtual ~runtime_error() noexcept = default;
        virtual const char* what() const noexcept
        {
            return errors[static_cast<size_t>(m_id)].c_str();
        };
    private:
        error_id m_id;
    };
}

#endif /* error_h */
