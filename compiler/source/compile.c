#include "compile.h"
#include "util/dynarr.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
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

// will return TRUE if c is a special char, and set `type` to the enum value of the token IF AND ONLY IF c is a special character
char IS_SPECIAL(char c, lexer_token_type_e* type) 
{
    for (int i = 0; i < COMPILER_SPECIAL_CHARS_COUNT; i++) {
        if (c == compiler_special_chars[i].character) {
            *type = compiler_special_chars[i].token;
            return TRUE;
        }
        return FALSE;
    }
}
// Returns TRUE if c is in compiler_reserved_keywords, otherwise returns FALSE
char IS_RESERVED(char* c) 
{
    for (int i = 0; i < COMPILER_RESERVE_KEYWORD_COUNT; i++) {
        if (strcmp(compiler_reserved_keywords[i], c) == 0) {
            return TRUE;   
        }
    }
    return FALSE;
}

// Used for writing a single character to a token value. Helper macro not for use outside of this file
// TODO : This could obviously be optimized. Too many strlen calls
#define _TV_APPEND_CHAR(c, token) { int _s = strlen(token.value); token.value[_s] = c; token.value[++_s] = 0x00; }

// Different token types have to be pushed differently, this function takes in a token and pusehs it to the dynamic array
// based on how it needs to be pushed.
void _push_token(dyn_arr_t* arr, lexer_token_t token) 
{
    switch (token.type) {
        case YET_UNKOWN:
            // Determine token type
            // TODO : Check for 0x, 0b, etc

            if (IS_NUMBER(token.value[0])) {
                // Check to see if there's any letters in the number
                char* c = token.value[0];
                while (*c) {
                    if (IS_NUMBER(*c) == FALSE) {
                        thow_compiler_error("Expected number, but instead got something else");
                    }
                    c++; // Variable name deja-vu
                }
                token.type = NUMBER;
            }
            else if (IS_RESERVED(token.value)) {
                token.type = KEYWORD;
            }

            DYN_ARR_PUSH_BACK(*arr, token);
            break;
        default:
            DYN_ARR_PUSH_BACK(*arr, token);
            break;
    }
}

// TODO : The use of lexer state is probably unnecessary. We could instead store what we're currently reading in
// the current_token and simply read from that
dyn_arr_t lex(char* filesrc) 
{
    lexer_state_t lexer_state;
    lexer_state_initialize(&lexer_state);

    

    dyn_arr_t all_lexer_tokens = DYN_ARR_CREATE(lexer_token_t);
    lexer_token_t current_token;

    // Should increment to the next char at end of while
    char inc_current_char;

    char* current_char = filesrc;
    while (*current_char) {
        inc_current_char = TRUE;
        lexer_token_type_e special_type;

        if (*current_char == ';' || *current_char == '\n') {
            // Newline has appeared, done reading current token
            if (lexer_state.currently_reading != NONE) {
                _push_token(&all_lexer_tokens, current_token);
            }
            lexer_token_initialize(&current_token);
            current_token.type = ENDL;
            _push_token(&all_lexer_tokens, current_token);
            lexer_state.currently_reading = NONE;
        }

        else if (IS_SPECIAL(*current_char, &special_type)) {
            // Special token has appeared, done reading current token
            if (lexer_state.currently_reading != NONE) {
                _push_token(&all_lexer_tokens, current_token);
            }

            lexer_token_initialize(&current_token);

            current_token.type = special_type;
            _TV_APPEND_CHAR(*current_char, current_token);
            _push_token(&all_lexer_tokens, current_token);
            lexer_state.currently_reading = NONE;
        }

        if (lexer_state.currently_reading == NONE) {
            if (IS_LETTER(*current_char)) {
                lexer_state.currently_reading = YET_UNKOWN;
                lexer_token_initialize(&current_token);
                current_token.type = YET_UNKOWN;
                _TV_APPEND_CHAR(*current_char, current_token)
            }

            else if (IS_NUMBER(*current_char)) {
                lexer_state.currently_reading = NUMBER;
                lexer_token_initialize(&current_token);
                current_token.type = NUMBER;
                _TV_APPEND_CHAR(*current_char, current_token)
            }

            else if (*current_char == '"') {
                // Start a string literal
                lexer_state.currently_reading = STRING;
                lexer_token_initialize(&current_token);
                current_token.type = STRING;
            }
        }

        else if (lexer_state.currently_reading == YET_UNKOWN) {
            // Currently reading a symbol or number,
            if (IS_LETTER(*current_char) || IS_NUMBER(*current_char)) {
                // More letters, append to symbol name
                _TV_APPEND_CHAR(*current_char, current_token);
            }
            else if (IS_WHITESPACE(*current_char)) {
                // Whitespace has appeared, and the symbol name has ended
                // Add lexer token to the dynarr
                _push_token(&all_lexer_tokens, current_token);
                lexer_state.currently_reading = NONE;
            }
            else {
                // Eventually throw unrecognized character error
            }
        }

        else if (lexer_state.currently_reading == STRING) {
            if (*current_char == '"') {
                // End of string literal
                _push_token(&all_lexer_tokens, current_token);
                lexer_state.currently_reading = NONE;
            } else {
                _TV_APPEND_CHAR(*current_char, current_token);
            }
        }

        if (inc_current_char)  {
            current_char++;
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

