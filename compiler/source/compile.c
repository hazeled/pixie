#include "compile.h"
#include "util/dynarr.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
// Takes in a compile state struct and fills it with default data
void lexer_state_initialize(lexer_state_t* lexer_state)
{
    lexer_state->currently_reading = NONE;
}

void lexer_token_initialize(lexer_token_t* token)
{
    token->type = NONE;
    token->value[0] = 0x00;
}


// Used for writing a single character to a token value. Helper macro not for use outside of this file
#define _TV_APPEND_CHAR(c) current_token.value[token_value_index++] = c; current_token.value[token_value_index] = 0x00

dyn_arr_t lex(char* filesrc) 
{
    lexer_state_t lexer_state;
    lexer_state_initialize(&lexer_state);

    dyn_arr_t all_lexer_tokens = DYN_ARR_CREATE(lexer_token_t);
    lexer_token_t current_token;
    int token_value_index = 0;

    char* current_char = filesrc;
    while (*current_char) {

        if (*current_char == ';') {
            // Semicolon has appeared, done reading current token
            DYN_ARR_PUSH_BACK(all_lexer_tokens, current_token);
            printf("%d\n", all_lexer_tokens.length); 
            lexer_token_initialize(&current_token);
            current_token.type = ENDL;
            current_char++;
        }

        if (lexer_state.currently_reading == NONE) {
            if (IS_WHITESPACE(*current_char)) { current_char++; continue; };
            if (IS_LETTER(*current_char)) {
                lexer_state.currently_reading = SYMBOL;
                lexer_token_initialize(&current_token);
                token_value_index = 0;
            }
        }

        else if (lexer_state.currently_reading == SYMBOL) {
            // Currently reading a symbol
            if (IS_LETTER(*current_char)) {
                // More letters, append to symbol name
                _TV_APPEND_CHAR(*current_char);

                current_char++;
            }
            else if (IS_WHITESPACE(*current_char)) {
                // Whitespace has appeared, and the symbol name has ended
                // Add lexer token to the dynarr
                DYN_ARR_PUSH_BACK(all_lexer_tokens, current_token);
                printf("%d\n", all_lexer_tokens.length); 
                lexer_state.currently_reading = NONE;

                current_char++;
            }
            else {
                // Eventually throw unrecognized character error
                current_char++;
            }
        }
    }

    return all_lexer_tokens;
}

int compile(char* filesrc)
{
    dyn_arr_t tokens = lex(filesrc);
    int ret = compile_tokens(tokens);
    
    return ret;
}

int compile_tokens(dyn_arr_t tokens)
{
    printf("b %d\n", tokens.length);
    for (int i = 0; i < tokens.length; i++) {
        lexer_token_t token = DYN_ARR_ACCESS(tokens, lexer_token_t, i);
        printf("(%d, %s)\n", token.type, token.value);
    }
    return 0;
}

int compile_file(const char* filepath) {
    
}

