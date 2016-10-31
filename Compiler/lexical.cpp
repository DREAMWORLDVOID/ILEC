#include "lexical.h"
#include "lex_literal.h"

std::string lexical::stage1_3(const std::string & i)
{
    using namespace lexical::preprocessing_token;
    using raw_string_literal = seq<3, opt<encoding_prefix>, _is<'R'>, raw_string>;
    using comment = _or <2,
        seq<4, _is < '/'>, _is < '/'>, any<_not<_is<'\n'>>>, _is<'\n'> >,
        seq<5, _is < '/'>, _is < '*'>, any<_not<seq<2, _is<'*'>, _is<'/'>>>>, 
        _is<'*'>, _is<'/'>>>;
    std::string ret;
    ret.resize(i.length());
    pchar iter = const_cast<pchar>(i.data());
    pchar end = iter + i.length();
    pchar p;
    while (iter < end)
    {
        p = raw_string_literal::w(iter);
        if (p)
        {
            iter = p; continue;
        }
        auto p = comment::w(iter);
        if (p)
        {
            iter = p; ret.push_back(' '); continue;
        }
        ret.push_back(*iter);
        ++iter;
    }
    return ret;
}
