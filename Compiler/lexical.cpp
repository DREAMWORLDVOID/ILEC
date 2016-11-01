#include "lexical.h"
#include "lex_literal.h"
#include "fileio.h"
#include <map>
#include <vector>
#include <functional>
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

    namespace preprocessing_token
    {
        std::string makestring(pchar pos1, pchar pos2);
    }

    std::list<std::unique_ptr<preprocessing_token::token>> physical_file::analyze()
    {
        std::vector<lexical_character> file;
        {
            auto list = std::list<lexical_character>();
            {
                auto c = IO::readText(m_physical_path);
                auto pos = 0u;
                for (auto ch : c)
                    list.push_back(lexical_character{ ch, pos++ });
                auto end = list.end();
                //set newlines to LF
                for (auto iter = list.begin(); iter != end;)
                {
                    auto base = iter;
                    iter++;
                    if (iter != end)
                    {
                        if (base->set == '\r' && iter->set == '\n')
                        {
                            list.erase(base);
                            iter++;
                            continue;
                        }
                    }
                    if (base->set == '\r')
                    {
                        base->set = '\n';
                        continue;
                    }
                }
            }
            //remove \ +  \n sequence
            {
                auto end = list.end();
                end--;
                for (auto iter = list.begin(); iter != end;)
                {
                    auto base = iter;
                    iter++;
                    if (base->set == '\\' && iter->set == '\n')
                    {
                        auto sec = iter;
                        iter++;
                        list.erase(base);
                        list.erase(sec);
                        continue;
                    }
                }
            }
            //replace comments by a space
            {
                auto end = list.end();
                end--;
                size_t cmode = 0;
                for (auto iter = list.begin(); iter != end;)
                {
                    switch (cmode)
                    {
                    case 0:
                    {
                        auto base = iter;
                        iter++;
                        if (iter != end)
                        {
                            if (base->set == '/' && iter->set == '/')
                            {
                                list.erase(base);
                                iter->set = ' ';
                                iter++;
                                cmode = 1;
                                continue;
                            }
                            if (base->set == '/' && iter->set == '*')
                            {
                                list.erase(base);
                                iter->set = ' ';
                                iter++;
                                cmode = 2;
                                continue;
                            }
                        }
                    }
                    break;
                    case 1:
                    {
                        if (iter->set == '\n')
                            cmode = 0;
                        auto base = iter;
                        iter++;
                        list.erase(base);
                    }
                    break;
                    case 2:
                    {
                        auto base = iter;
                        iter++;
                        if (iter != end)
                        {
                            if (base->set == '*' && iter->set == '/')
                            {
                                auto sec = iter;
                                iter++;
                                list.erase(base);
                                list.erase(sec);
                                cmode = 0;
                                continue;
                            }
                        }
                        list.erase(base);
                    }
                    break;
                    }
                    if (cmode == 2)
                    {
                        throw error::runtime_error(error::error_id::L1001);
                    }
                }
            }
            file.resize(list.size());
            for (auto&& i : list)
                file.push_back(i);
            list.clear();
        }
    
        //tokenlize
        auto ret = std::list<std::unique_ptr<preprocessing_token::token>>();
        {
            using namespace lexical::preprocessing_token;
            auto funcmap =
                std::map<preprocessing_token::tokens,
                std::function<std::unique_ptr<preprocessing_token::token>(pchar& ch)>>
            {
                { t_header_name, [](pchar& c)->auto {return std::make_unique<tk_header_name>(c); } },
                { t_identifier,[](pchar& c)->auto {return std::make_unique<tk_identifier>(c); } },
                { t_pp_number, [](pchar& c)->auto {return std::make_unique<tk_pp_number>(c); } },
                { t_character_literal,[](pchar& c)->auto {return std::make_unique<tk_character_literal>(c); } },
                { t_user_defined_character_literal, [](pchar& c) { return std::make_unique<tk_user_defined_character_literal>(c); } },
                { t_string_literal, [](pchar& c)->auto {return std::make_unique<tk_string_literal>(c); } },
                { t_user_defined_string_literal,[](pchar& c)->auto {return std::make_unique<tk_string_literal>(c); } },
                { t_preprocessing_op_or_punc,[](pchar& c)->auto{return std::make_unique<tk_preprocessing_op_or_punc>(c); } },
                { t_others,[](pchar& c)->auto{ return std::make_unique<tk_others>(c); } }
            };
            pchar iter = const_cast<pchar>(file.data());
            pchar end = iter + file.size();
            while (iter < end)
            {
                pchar arr[] =
                {
                    header_name::w(iter),
                    identifier::w(iter),
                    pp_number::w(iter),
                    character_literal::w(iter),
                    user_defined_character_literal::w(iter),
                    string_literal::w(iter),
                    user_defined_string_literal::w(iter),
                    preprocessing_op_or_punc::w(iter),
                    iter + 1
                };
                auto p = max_element(arr, arr + 9);
                //remove null characters since they are no longer significant 
                if (!(((p - arr) == 8) && (iter->set != '\n')))
                    ret.push_back(funcmap[static_cast<tokens>(p - arr)](iter));
                iter = *p;
            }
        }
        return ret;
    }

}
