#ifndef lex_literal_h
#define lex_literal_h

#include <iostream>

namespace lexical
{
    using pchar = char*;
    using func = pchar(pchar);
    
    template <func f>
    inline pchar invoke(pchar ch)
    {
        return (*ch != 0) ? f(ch) : nullptr;
    }
    
    template <size_t l, func f, func ...s>
    struct seq
    {
        inline static pchar w(pchar ch)
        {
            ch = invoke<f>(ch); if (ch) ch = seq<l - 1, s...>::w(ch);
            return ch;
        }
    };
    
    template <func f>
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
            return (*ch == c) ? ++ch : nullptr;
        }
    };
    
    template <char a, char b>
    struct _in
    {
        inline static pchar w(pchar ch)
        {
            return ((*ch >= a) && (*ch <= b)) ? ++ch : nullptr;
        }
    };
    
    template <size_t l, func f, func ...fs>
    struct _or
    {
        inline static pchar w(pchar ch)
        {
            pchar c = invoke<f>(ch);
            return c ? c : invoke<_or<l - 1, fs...>::w>(ch);
        }
    };
    
    template <func f>
    struct _or<1, f>
    {
        inline static pchar w(pchar ch)
        {
            return invoke<f>(ch);
        }
    };
    
    template <size_t l, size_t m, func f>
    struct _for
    {
        inline static pchar w(pchar ch)
        {
            size_t c = 0;
            pchar c2;
            for (; c < l; ++c) { ch = invoke<f>(ch); if (!ch) return nullptr; }
            for (; c < m; ++c) { c2 = invoke<f>(ch); if (!c2) break; else ch = c2; }
            return ch;
        }
    };
    
    template <size_t l, func f, func ...s>
    struct _and
    {
        inline static pchar w(pchar ch)
        {
            ch = invoke<f>(ch);
            pchar c  = _and<l - 1, s...>::w(ch);
            return min(ch, c);
        }
    };
    
    template <func f>
    struct _and<1, f>
    {
        inline static pchar w(pchar ch)
        {
            return invoke<f>(ch);
        }
    };
    
    template <func f>
    struct _not
    {
        inline static pchar w(pchar ch)
        {
            pchar c = invoke<f>(ch);
            return c ? nullptr : ++ch;
        }
    };
    
    template <func f>
    using any = _for<0, 0xFFFFFFFF, f>;
    template <func f>
    using opt = _for<0, 1, f>;
    template <func f>
    using orm = _for<1, 0xFFFFFFFF, f>;
    template <size_t t, func f>
    using times = _for<t, t, f>;
    
    using dec = _in<'0', '9'>;
    using oct = _in<'0', '7'>;
    using hex = _or<3, _in<'0', '9'>::w, _in<'a', 'f'>::w, _in<'A', 'F'>::w>;
    
    using digit_sequence = orm<dec::w>;
    using fractional_constant =
    _or<2, seq<2, digit_sequence::w, _is<'.'>::w>::w,
    seq<3, opt<digit_sequence::w>::w, _is<'.'>::w, digit_sequence::w>::w>;
    using sign = _or<2, _is<'+'>::w, _is<'-'>::w>;
    using exponent =
    seq<3, _or<2, _is<'E'>::w, _is<'e'>::w>::w, opt<sign::w>::w, digit_sequence::w>;
    using floating_suffix =
    _or<4, _is<'f'>::w, _is<'F'>::w,_is<'l'>::w, _is<'L'>::w>;
    using floating_literal =
    seq<2, _or<2, seq<2, fractional_constant::w, opt<exponent::w>::w>::w,
    seq<2, digit_sequence::w, exponent::w>::w>::w,
    opt<floating_suffix::w>::w>;
    
    using nonzero = _in<'1', '9'>;
    using decimal_literal = seq<2, nonzero::w, any<dec::w>::w>;
    using octal_literal = seq<2, _is<'0'>::w, any<oct::w>::w>;
    using hexadecimal_literal = seq<3, _is<'0'>::w,
    _or<2, _is<'x'>::w, _is<'X'>::w>::w, any<hex::w>::w>;
    using unsigned_suffix = _or<2, _is<'u'>::w, _is<'U'>::w>;
    using long_suffix = _or<2, _is<'l'>::w, _is<'L'>::w>;
    using long_long_suffix = times<2, _or<2, _is<'l'>::w, _is<'L'>::w>::w>;
    using unified_long_suffix =_or<2, long_suffix::w, long_long_suffix::w>;
    using integer_suffix =
    _or<2,seq<2, unsigned_suffix::w, opt<unified_long_suffix::w>::w>::w,
    seq<2, unified_long_suffix::w, unsigned_suffix::w>::w>;
    using integer_literal =
    seq<2,_or<3, decimal_literal::w, octal_literal::w, hexadecimal_literal::w>::w,
    opt<integer_suffix::w>::w>;
}

#endif
