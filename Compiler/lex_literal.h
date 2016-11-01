#ifndef lex_literal_h
#define lex_literal_h

#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <memory>
#include "lexical.h"

namespace lexical
{
    using pchar = lexical_character*;
    
    template <class f>
    inline pchar invoke(pchar ch)
    {
        return (ch->set != 0) ? f::w(ch) : nullptr;
    }
    
    template <size_t l, class f, class ...s>
    struct seq
    {
        inline static pchar w(pchar ch)
        {
            ch = invoke<f>(ch);
            if (ch)
                ch = seq<l - 1, s...>::w(ch);
            return ch;
        }
    };
    
    template <class f>
    struct seq<1, f>
    {
        inline static pchar w(pchar ch)
        {
            return invoke<f>(ch);
        }
    };
    
    template <char c>
    struct _is
    {
        inline static pchar w(pchar ch)
        {
            return (ch->set == c) ? ++ch : nullptr;
        }
    };
    
    template <char a, char b>
    struct _in
    {
        inline static pchar w(pchar ch)
        {
            return ((ch->set >= a) && (ch->set <= b)) ? ++ch : nullptr;
        }
    };
    
    template <size_t l, class f, class ...fs>
    struct _or
    {
        inline static pchar w(pchar ch)
        {
            return std::max(invoke<f>(ch), invoke<_or<l - 1, fs...> >(ch));
        }
    };
    
    template <class f>
    struct _or<1, f>
    {
        inline static pchar w(pchar ch)
        {
            return invoke<f>(ch);
        }
    };
    
    template <size_t l, size_t m, class f>
    struct _for
    {
        inline static pchar w(pchar ch)
        {
            size_t c = 0;
            pchar c2;
            for (; c < l; ++c)
            {
                ch = invoke<f>(ch);
                if (!ch)
                    return nullptr;
            }
            for (; c < m; ++c)
            {
                c2 = invoke<f>(ch);
                if (!c2)
                    break;
                else
                    ch = c2;
            }
            return ch;
        }
    };
    
    template <size_t l, class f, class ...s>
    struct _and
    {
        inline static pchar w(pchar ch)
        {
            return std::min(invoke<f>(ch), _and<l - 1, s...>::w(ch));
        }
    };
    
    template <class f>
    struct _and<1, f>
    {
        inline static pchar w(pchar ch)
        {
            return invoke<f>(ch);
        }
    };
    
    template <class f>
    struct _not
    {
        inline static pchar w(pchar ch)
        {
            pchar c = invoke<f>(ch);
            return c ? nullptr : ++ch;
        }
    };
    
    template <class f>
    using any = _for<0, 0xFFFFFFFF, f>;
    template <class f>
    using opt = _for<0, 1, f>;
    template <class f>
    using orm = _for<1, 0xFFFFFFFF, f>;
    template <size_t t, class f>
    using tim = _for<t, t, f>;
    
    using dec = _in<'0', '9'>;
    using oct = _in<'0', '7'>;
    using hex = _or<3, _in<'0', '9'>, _in<'a', 'f'>, _in<'A', 'F'> >;
    using sign = _or<2, _is<'+'>, _is<'-'> >;
    using universal_character_name =
    seq<2, _is<'\\'>,_or<2,seq<2, _is<'u'>, tim<4, hex> >,
    seq<2, _is<'U'>, tim<8, hex> > > >;
    
    namespace preprocessing_token
    {
        //header name token
        using no_new_line = _not<_is<'\n'> >;
        using h_char = _and<2, no_new_line, _not<_is<'>'> > >;
        using h_char_sequence = orm<h_char>;
        using q_char = _and<2, no_new_line, _not<_is<'"'> > >;
        using q_char_sequence = orm<q_char>;
        using header_name =
        _or<2, seq<3, _is<'<'>, h_char_sequence, _is<'>'> >,
        seq<3, _is<'"'>, q_char_sequence, _is<'"'> > >;
        
        //identifiers
        using nondigit = _or<3, _in<'a', 'z'>, _in<'A', 'Z'>, _is<'_'> >;
        using identifier_nondigit = _or<2, nondigit, universal_character_name>;
        using identifier =
        seq<2, identifier_nondigit, any<_or<2, identifier_nondigit, dec> > >;
        
        //pp number
        using pp_number = seq<3, opt<_is<'.'> >, dec, any<_or<7,
        dec, seq<2, _is<'\''>, dec>, seq<2, _is<'\''>, nondigit>, 
        identifier_nondigit, seq<2, _is<'e'>, sign>, 
        seq<2, _is<'E'>, sign>, _is<'.'> > > >;
        
        //character
        using hexadecimal_escape_sequence = seq<3, _is<'\\'>, _is<'x'>, orm<hex> >;
        using octal_escape_sequence = seq<2, _is<'\\'>, _for<1, 3, oct> >;
        using simple_escape_sequence = seq<2, _is<'\\'>, _or<11, 
        _is<'\''>, _is<'\"'>, _is<'\?'>, _is<'\\'>, _is<'a'>, _is<'b'>,
        _is<'f'>, _is<'n'>, _is<'r'>, _is<'t'>, _is<'v'> > >;
        using escape_sequence = _or<3, hexadecimal_escape_sequence,
        octal_escape_sequence, simple_escape_sequence>;
        using c_char = _or<3, 
        _and<3, no_new_line, _not<_is<'\''> >, _not<_is<'\\'> > >,
        escape_sequence, universal_character_name>;
        using c_char_sequence = orm<c_char>;
        using character_literal = seq<4, 
        opt<_or<3, _is<'u'>, _is<'U'>, _is<'L'> > >, 
        _is<'\''>, c_char_sequence, _is<'\''> >;
        
        //ud character
        using user_defined_character_literal = seq<2, character_literal, identifier>;
        
        //string literal
        using s_char = _or<3, 
        _and<3, no_new_line, _not<_is<'\"'> >, _not<_is<'\\'> > >,
        escape_sequence, universal_character_name>;
        using s_char_sequence = orm<s_char>;
        using d_char = _not<_or<8, _is<'\n'>, _is<'\t'>, _is<'\v'>, _is<'\f'>,
        _is<'('>, _is<')'>, _is<' '>, _is<'\\'> > >;
        using d_char_sequence = orm<d_char>;
        inline pchar str_comp(char* b1, char* e1, pchar b2)
        {
            for (; b1 < e1; ++b1, ++b2)
                if ((*b1 != b2->set) || (b2->set == 0))
                    return nullptr;
            return b2;
        }
        inline pchar str_comp(pchar b1, pchar e1, pchar b2)       
        {
            for (; b1 < e1; ++b1, ++b2)
                if ((b1->set != b2->set) || (b2->set == 0))
                    return nullptr;
            return b2;
        }
        struct raw_str_content
        {
            static inline pchar w(pchar ch)
            {
                pchar dchar = invoke<opt<d_char_sequence> >(ch);
                pchar i = invoke<_is<'('> >(dchar);
                if (i == nullptr) return nullptr;
                for (; i->set != 0; ++i)
                    if (i->set == ')')
                    {
                        pchar e = str_comp(ch, dchar, i + 1);
                        if (e)
                            return e;
                    }
                return nullptr;
            }
        };
        using raw_string = seq<3, _is<'\"'>, raw_str_content, _is<'\"'> >;
        using encoding_prefix = _or<4, seq<2, _is<'u'>, _is<'8'> >,
        _is<'U'>, _is<'u'>, _is<'L'> >;
        using string_literal = seq<2, opt<encoding_prefix>, _or<2,
        seq<3, _is<'\"'>, opt<s_char_sequence>, _is<'\"'> >,
        seq<2, _is<'R'>, raw_string> > >;
        
        //ud string
        using user_defined_string_literal = seq<2, string_literal, identifier>;
        struct preprocessing_op_or_punc
        {
            static inline pchar w(pchar ch)
            {
                pchar res = nullptr;
                for (auto i = 0; i < len_preprocessing_op_or_punc; ++i)
                {
                    auto&& base = b_preprocessing_op_or_punc[i];
                    res = std::max(res, str_comp(const_cast<char*>(base.data()),
                        const_cast<char*>(base.data()) + base.length(), ch));
                }
                return res;
            }
        };

        class tk_header_name : public token
        {
        public:
            std::string name;
            tk_header_name() = default;
            tk_header_name(pchar& pos);
        };
        
        class tk_identifier : public token
        {
        public:
            std::string data;
            tk_identifier() = default;
            tk_identifier(pchar& pos);
        };

        class tk_pp_number : public token
        {
        public:
            std::string data;
            tk_pp_number() = default;
            tk_pp_number(pchar& pos);
        };
            
        class tk_character_literal : public token
        {
        public:
            std::string data;
            tk_character_literal() = default;
            tk_character_literal(pchar& pos);
        };
        
        class tk_user_defined_character_literal : public token
        {
        public:
            tk_character_literal character;
            tk_identifier udsuffix;
            tk_user_defined_character_literal() = default;
            tk_user_defined_character_literal(pchar& pos);
        };
            
        class tk_string_literal :public token
        {
        public:
            std::string data;
            tk_string_literal() = default;
            tk_string_literal(pchar& pos);
        };
            
        class tk_user_defined_string_literal :public token
        {
        public:
            tk_string_literal string;
            tk_identifier udsuffix;
            tk_user_defined_string_literal() = default;
            tk_user_defined_string_literal(pchar& pos);
        };
        
        class tk_preprocessing_op_or_punc : public token
        {
        public:
            std::string data;
            tk_preprocessing_op_or_punc() = default;
            tk_preprocessing_op_or_punc(pchar& pos);
        };

        class tk_others : public token
        {
        public:
            char data;
            tk_others() = default;
            tk_others(pchar& pos);
        };
               
    }
    
}

#endif
