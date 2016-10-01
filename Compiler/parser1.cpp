#include <iostream>
using namespace std;
namespace parser1
{
#define ISPCF inline static pchar w(pchar ch)
#define PBEG(x) struct x { ISPCF {
#define PEND }};  
    using pchar = char*;
    using func = pchar(pchar);
    
    template <func f> inline pchar invoke(pchar ch)
    { return (*ch != 0) ? f(ch) : nullptr; }
    template <size_t l, func f, func ...s> PBEG(seq)
    ch = invoke<f>(ch); if (ch) ch = seq<l - 1, s...>::w(ch); return ch; PEND
    template <func f> struct seq<1, f> { ISPCF { return invoke<f>(ch); } };
    template <char c> PBEG(lis) return (*ch == c) ? ++ch : nullptr; PEND
    template <char a, char b> PBEG(lin)
    return ((*ch >= a) && (*ch <= b)) ? ++ch : nullptr; PEND
    template <size_t l, func f, func ...fs> PBEG(lor)
    pchar c = invoke<f>(ch); return c ? c : invoke<lor<l - 1, fs...>::w>(ch); PEND
    template <func f> struct lor<1, f> { ISPCF { return invoke<f>(ch); PEND
    template <size_t l, size_t m, func f> PBEG(cfor)
        size_t c = 0; pchar c2;
        for (; c < l; ++c) { ch = invoke<f>(ch); if (!ch) return nullptr; } 
        for (; c < m; ++c) { c2 = invoke<f>(ch); if (!c2) break; else ch = c2; }
        return ch;
    PEND
    
    template<func f> using any = cfor<0, 0xFFFFFFFF, f>;
    template<func f> using opt = cfor<0, 1, f>;
    template<func f> using orm = cfor<1, 0xFFFFFFFF, f>;
    template<size_t t, func f> using times = cfor<t, t, f>;
    
    using dec = lin<'0', '9'>;
    using oct = lin<'0', '7'>;
    using hex = lor<3, lin<'0', '9'>::w, lin<'a', 'f'>::w, lin<'A', 'F'>::w>;
    
    using digit_sequence = orm<dec::w>;
    using fractional_constant = lor<2, seq<2, digit_sequence::w, lis<'.'>::w>::w,
        seq<3, opt<digit_sequence::w>::w, lis<'.'>::w, digit_sequence::w>::w>;
    using sign = lor<2, lis<'+'>::w, lis<'-'>::w>;
    using exponent = seq<3, lor<2, lis<'E'>::w, lis<'e'>::w>::w, opt<sign::w>::w,
        digit_sequence::w>;
    using floating_suffix = lor<4, lis<'f'>::w, lis<'F'>::w, 
        lis<'l'>::w, lis<'L'>::w>;
    using floating_literal = seq<2, lor<2, 
        seq<2, fractional_constant::w, opt<exponent::w>::w>::w,
        seq<2, digit_sequence::w, exponent::w>::w>::w,
        opt<floating_suffix::w>::w>;
    
    using nonzero = lin<'1', '9'>;
    using decimal_literal = seq<2, nonzero::w, any<dec::w>::w>;
    using octal_literal = seq<2, lis<'0'>::w, any<oct::w>::w>;
    using hexadecimal_literal = seq<3, lis<'0'>::w, 
        lor<2, lis<'x'>::w, lis<'X'>::w>::w, any<hex::w>::w>;
    using unsigned_suffix = lor<2, lis<'u'>::w, lis<'U'>::w>;
    using long_suffix = lor<2, lis<'l'>::w, lis<'L'>::w>;
    using long_long_suffix = times<2, lor<2, lis<'l'>::w, lis<'L'>::w>::w>;
    using unified_long_suffix = lor<2, long_suffix::w, long_long_suffix::w>;
    using integer_suffix = lor<2,
        seq<2, unsigned_suffix::w, opt<unified_long_suffix::w>::w>::w,
        seq<2, unified_long_suffix::w, unsigned_suffix::w>::w>;
    using integer_literal = seq<2, 
        lor<3, decimal_literal::w, octal_literal::w, hexadecimal_literal::w>::w,
        opt<integer_suffix::w>::w>;
}

using namespace parser1;
template <func f> bool testif(pchar ch) { return invoke<f>(ch); }
int main()
{
	cout << testif<octal_literal::w>("01000000") << endl;
}
