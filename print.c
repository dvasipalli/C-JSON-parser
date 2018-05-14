#include "internal.h"

char *output = NULL;
long opos, osize;

/**
 * This function is provided for you if you wish to use it.  It prints into
 * a string buffer (output) that is dynamically re-sized to fit.  It otherwise
 * has the same syntax as printf().  It returns the current position in the
 * output buffer, but you will probably not need it.
 */
static long oprint(char *fmt, ...)
{
  long size, printed;
  va_list ap;

  va_start(ap, fmt);
  size = osize-opos;
  printed = vsnprintf(output+opos, size, fmt, ap);
  va_end(ap);
  if (printed < size) return (opos += printed);

  // if printed >= size, then not enough space was available in the output buffer
  // grow it and do it again.  Grow it in 1K increments until it is definitely
  // enough to contain what we want to print:
  do {
    printed -= 1024;
    osize += 1024;
  } while (printed >= 0);
  output = realloc(output, sizeof(char) * osize);

  va_start(ap, fmt);
  size = osize-opos;
  printed = vsnprintf(output+opos, size, fmt, ap);
  va_end(ap);

  return opos += printed;
}

/**
 * char	  : any-Unicode-character-except-"-or-\-or-control-character
 *	  | \" | \\ | \/ | \b | \f | \n | \r | \t| \u four-hex-digits 
 */
static long print_string(char *s)
{
  oprint("\"%s\"", s);
}

static long print_array(array_t *a)
{
  long i;
  for(i=0; i<a->length; i++) {
    print_value(&a->a[i]);
    if(i != a->length-1) oprint(", ");
  }
}

static long print_pair(pair_t *p)
{
  oprint("\"%s\" : ", p->name);
  print_value(&(p->v));
}

static long print_object(pair_t *o)
{
  pair_t *p;

  for(p = o; p; p=p->next) {
    print_pair(p);
    if(p->next != NULL) oprint(", ");
  }
}

long print_value(VALUE *v)
{
  switch(v->type) {
    case DT_OBJECT:
      oprint(" {");
      print_object(v->o);
      oprint("} ");
      break;
    case DT_ARRAY:
      oprint(" [");
      print_array(v->a);
      oprint("] ");
      break;
    case DT_STRING:
      print_string(v->s); break;
    case DT_NUMBER:
      oprint("%lf", v->n); break;
    case DT_TRUE:
      oprint("True"); break;
    case DT_FALSE:
      oprint("False"); break;
    case DT_NULL:
      oprint("NULL"); break;
  }
}
