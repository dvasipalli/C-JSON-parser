/**
 * This header is for the functions internal to your library.  You may add /
 * remove things to it as needed.  You should not modify the json.h header file
 * as it exports the API information necessary to use the library.
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include "json.h"

#define K	1024

typedef enum {
  T_STRING, T_NUMBER, T_OBRAC, T_CBRAC, T_OCBRACE, T_CCBRACE, T_COMMA, T_COLON,
  T_TRUE, T_FALSE, T_NULL,
  T_EOI, T_UNKNOWN
} token_t;

struct token {
  token_t tok;
  char buf[K];
  double val;
};

void startlex(void);
void match(token_t tok);
int accept(token_t tok);

VALUE *parse_value(void);
long print_value(VALUE *v);
