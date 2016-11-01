#include "lex_literal.h"

namespace lexical
{
    namespace preprocessing_token
    {
        std::string makestring(pchar pos1, pchar pos2)
        {
            std::string ret;
            for (; pos1 < pos2; pos1++)
                ret.push_back(pos1->set);
            return ret;
        }
        tk_header_name::tk_header_name(pchar & pos)
        {
            type = t_header_name;
            std::cout << name;
            name = makestring(pos, header_name::w(pos));
        }

        tk_identifier::tk_identifier(pchar & pos)
        {
            type = t_identifier;
            data = makestring(pos, identifier::w(pos));
            std::cout << data;
        }

        tk_pp_number::tk_pp_number(pchar & pos)
        {
            type = t_pp_number;
            data = makestring(pos, pp_number::w(pos));
            std::cout << data;
        }

        tk_character_literal::tk_character_literal(pchar & pos)
        {
            type = t_character_literal;
            data = makestring(pos, pp_number::w(pos));
            std::cout << data;
        }

        tk_user_defined_character_literal::tk_user_defined_character_literal(pchar & pos)
        {
            type = t_user_defined_character_literal;
            character = tk_character_literal(pos);
            udsuffix = tk_identifier(pos);
        }

        tk_string_literal::tk_string_literal(pchar & pos)
        {
            type = t_string_literal;
            data = makestring(pos, pp_number::w(pos));
            std::cout << data;
        }

        tk_user_defined_string_literal::tk_user_defined_string_literal(pchar & pos)
        {
            type = t_user_defined_string_literal;
            string = tk_string_literal(pos);
            udsuffix = tk_identifier(pos);
        }

        tk_preprocessing_op_or_punc::tk_preprocessing_op_or_punc(pchar & pos)
        {
            type = t_preprocessing_op_or_punc;
            data = makestring(pos, pp_number::w(pos));
            std::cout << data;
        }

        tk_others::tk_others(pchar & pos)
        {
            type = t_others;
            data = pos->set;
        }

    }
}
