#include <map>
#include <list>
#include <tuple>
#include <regex>
#include <string>
#include <memory>
#include <iostream>
#include <cstring>
#include <functional>

enum class Stage : size_t
{
    comment_mule, comment_mulc, comment_mulb, commentl, float_sci, float_int,
    hexnum, binnum, decnum, lstr, str, chr, symbol, text
};

class Parser1
{
private:
    bool comment_mul;
    using stage = std::tuple<Stage, std::regex, std::function<bool()>, std::function<void()>>;
    std::vector<stage> stages;
public:
    struct peer
    {
        std::string str;
        size_t position;
        Stage s;
    };
    Parser1()
    {
        stages.push_back(stage(Stage::comment_mule, std::regex(R"(.*\*/)"), 
            [this]()->bool { return comment_mul; }, [this]() { comment_mul = false; }));
        stages.push_back(stage(Stage::comment_mulc, std::regex(R"(/\*.*\*/)"),
            []()->bool { return true; }, nullptr));
        stages.push_back(stage(Stage::comment_mulb, std::regex(R"(/\*.*)"),
            [this]()->bool { return !comment_mul; }, [this]() { comment_mul = true; }));
        stages.push_back(stage(Stage::commentl, std::regex(R"(//.*)"),
            nullptr, nullptr));
        stages.push_back(stage(Stage::float_sci, std::regex(R"([0-9]\.([0-9]+)[Ee]([0-9]+))"),
            nullptr, nullptr));
        stages.push_back(stage(Stage::float_int, std::regex(R"(([0-9]*)\.([0-9]+))"),
            nullptr, nullptr));
        stages.push_back(stage(Stage::hexnum, std::regex("0x[0-9A-Fa-f]+"),
            nullptr, nullptr));
        stages.push_back(stage(Stage::binnum, std::regex("0b[01]+"),
            nullptr, nullptr));
        stages.push_back(stage(Stage::lstr, std::regex(R"(L".*")"), nullptr, nullptr));
        stages.push_back(stage(Stage::str, std::regex(R"(".*")"), nullptr, nullptr));
        stages.push_back(stage(Stage::chr, std::regex("'.'"), nullptr, nullptr));
        stages.push_back(stage(Stage::decnum, std::regex("[0-9]+"), nullptr, nullptr));
        stages.push_back(stage(Stage::symbol,
            std::regex(R"([\!\@\#\$\%\^\&\*\(\)\_\-\+\=\{\}\|\[\]\\\:\"\;\'\<\>\?\,\.\/\~])"),
            nullptr, nullptr));
        stages.push_back(stage(Stage::text, std::regex("[^ ]+"), nullptr, nullptr));
    }

    void reset()
    {
        comment_mul = false;
    }

    std::list<peer> parseLine(std::string line)
    {
        std::map<size_t, peer> res;
        for (auto s : stages)
        {
            if (std::get<2>(s))
                if (!(std::get<2>(s)()))
                    continue;
            for (std::sregex_iterator p(line.cbegin(), line.cend(), std::get<1>(s)), q; p != q; ++p)
            {
                res.insert({ static_cast<size_t>(p->position(0)), { p->str(), static_cast<size_t>(p->position(0)), std::get<0>(s) } });
                memset(const_cast<char*>(line.data()) + p->position(0), 0, p->str().length());
            }
        }
        std::list<peer> ret;
        for (auto i : res)
            ret.push_back(i.second);
        return ret;
    }
};

int main()
{
    Parser1 p;
    std::string line;
    std::getline(std::cin, line);
    p.reset();
    auto list = p.parseLine(line);
    for (auto i : list)
    {
        std::cout << i.str << " " << i.position << std::endl;
    }
    system("pause");
    return 0;
}
