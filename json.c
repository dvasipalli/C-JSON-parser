#include "internal.h"

extern char *input, *output;
extern long opos, osize;

/**
 * The following is a GCC option to make these functions globally visible
 * (i.e. non-hidden) which we've made the default with the -fvisibility=hidden
 * compile option. Ideally a library only exports symbols that are part of the
 * API, and nothing else, to minimize name-space pollution. In this case it is
 * not necessary to make functions and variables that are not used outside of
 * the file in which they are declared 'static' (i.e. file level scope,) but
 * I have done this in most cases anyway.  If you need to you can remove the
 * static attributes if it helps you.
 */
VALUE *json_decode(char *s) __attribute__ ((visibility ("default") ));
char *json_encode(VALUE *v) __attribute__ ((visibility ("default") ));

/**
 * These are the interfaces to the library.
 * json_decode should return NULL if it fails to parse the input for any reason.
 * json_encode probably should too, but I didn't put any error checking in mine
 */
VALUE *json_decode(char *s)
{
  input = s;
  startlex();
  return parse_value();
}

char *json_encode(VALUE *v)
{
  char *s;
  output = malloc(sizeof(char) * (osize=1024));
  opos = 0;
  print_value(v);
  return output;
}
