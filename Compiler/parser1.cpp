namespace parser1
{
    using pchar = char*;
    using func = pchar(pchar);
    template <size_t l, func f, func ... fs>
    struct sequence
    {
        static pchar w(pchar ch)
        {
            ch = f(ch);
            if (ch) ch = sequence<l - 1, fs...>::w(ch);
            return ch;
        }
    };

    template <func f>
    struct sequence<1, f>
    {
        static pchar w()(pchar ch)
        {
            return f(ch);
        }
    };

    template<char c>
    struct lis
    {
        static pchar w(pchar ch)
        {
            return (*ch == c) ? ++ch : nullptr;
        }
    };

    template<char a, char b>
    struct lin
    {
        static pchar w(pchar ch)
        {
            return ((*ch >= a) && (*ch <= b)) ? ++ch : nullptr;
        }
    };

    template<size_t l, func f, func ...fs>
    struct lor
    {
        static pchar w(pchar ch)
        {
            pchar c = f(ch);
            return c ? c : lor<l - 1, fs...>::w(ch);
        }
    };

    template<func f>
    struct lor<1, f>
    {
        static pchar w(pchar ch)
        {
            return f(ch);
        }
    };

    template <size_t lst, size_t mst, func f>
    struct cfor
    {
        static pchar w(pchar ch)
        {
            for (size_t c = 0; c < lst; ++c)
                if (!f(ch++)) return nullptr;
        }
    };

    using dec = lin<'0', '9'>;
    using hex = lor<3, lin<'0', '9'>::w, lin<'a', 'f'>::w, lin<'A', 'F'>::w>;
}