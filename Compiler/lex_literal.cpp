#include "lex_literal.h"

namespace lexical
{
    namespace preprocessing_token
    {
        tk_header_name::tk_header_name(pchar & pos)
        {
        }

        tk_identifier::tk_identifier(pchar & pos)
        {
        }

        tk_pp_number::tk_pp_number(pchar & pos)
        {
        }

        tk_character_literal::tk_character_literal(pchar & pos)
        {
        }

        tk_user_defined_character_literal::tk_user_defined_character_literal(pchar & pos)
        {
            character = tk_character_literal(pos);
            udsuffix = tk_identifier(pos);
        }

        tk_string_literal::tk_string_literal(pchar & pos)
        {
        }

        tk_user_defined_string_literal::tk_user_defined_string_literal(pchar & pos)
        {
        }

        tk_preprocessing_op_or_punc::tk_preprocessing_op_or_punc(pchar & pos)
        {
        }

    }
}
