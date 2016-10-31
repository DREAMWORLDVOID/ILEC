#include "lexical.h"
#include "lex_literal.h"
#include "fileio.h"
namespace lexical
{
    physical_file::physical_file(std::string physical_path):
        m_physical_path(physical_path)
    {
        m_name = m_physical_path.substr(
            std::max(m_physical_path.find_last_of('\\'),
                m_physical_path.find_last_of('/')) + 1
        );
    }

    std::list<std::unique_ptr<preprocessing_token::token>> physical_file::analyze()
    {
        auto list = std::list<lexical_character>();
        auto c = IO::readText(m_physical_path);
        auto pos = 0u;
        for (auto ch : c)
        {
            list.push_back(lexical_character{ ch, pos }); pos++;
        }
        //transformation should be done here
        //set newlines to LF
        for (auto iter = list.begin(); iter != list.end();)
        {
            auto base = iter; 
            iter++;
            if (base->set == '\r' && iter->set == '\n')
            {
                list.erase(base);
                iter++;
                continue;
            }
            if (base->set == '\r')
            {
                base->set = '\n';
                continue;
            }
        }
        for (auto i : list)
        {
            std::cout << i.set;
        }
        return std::list<std::unique_ptr<preprocessing_token::token>>();
    }

}
