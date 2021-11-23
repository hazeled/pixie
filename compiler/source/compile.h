#pragma once

#include <stdint.h>
#include "util/dynarr.h"

#define TRUE 1
#define FALSE 0

// Used for the lexer. Gets the next character in the source and throws and error if there
// isn't one
#define GET_NEXT_CHAR(current_char) if(*++current_char == 0x00) { throw_error("Did not expect end of file!!"); }
#define IS_WHITESPACE(c) c == ' ' || c == '\t'
#define IS_NUMBER(c) c >= '0' && c <= '9' // Check if character c is in number ascii range
#define IS_LETTER(c) (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')


typedef struct keyword_t {
    uint32_t keyword_id;
    char* keyword;
} keyword_t;

typedef enum lexer_token_type_e {
    SYMBOL,
    STRING,
    NUMBER,
    TYPE,
    ENDL,
    NONE,
} lexer_token_type_e;
typedef struct lexer_token_t {
    lexer_token_type_e type;
    char value[100];
} lexer_token_t;
void lexer_token_initialize(lexer_token_t* token);

typedef struct lexer_state_t { 
    lexer_token_type_e currently_reading;
} lexer_state_t;
void lexer_state_initialize(lexer_state_t* compile_state);

void add_keyword(const char* keyword);

int compile_file(const char* filepath);
int compile_tokens(dyn_arr_t tokens);
dyn_arr_t lex(char* filesrc);
int compile(char* filesrc);
