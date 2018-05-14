#include "internal.h"

extern struct token _cur;
static VALUE *parse_object(void);

static VALUE *newvalue(value_t type, void *value)
{
  VALUE *v;

  v = malloc(sizeof(VALUE));
  v->type = type;
  switch(type) {
    case DT_TRUE:
    case DT_FALSE:
    case DT_NULL:
    case DT_STRING: v->s = strdup((char *)value); break;
    case DT_NUMBER: v->n = *(double *)value; break;
    case DT_OBJECT: v->o = (pair_t *)value; break;
    case DT_ARRAY:  v->a = (array_t *)value; break;
  }

  return v;
}

static pair_t *newpair(char *name, VALUE *v)
{
  pair_t *p;

  p = malloc(sizeof(pair_t));
  p->name = name;
  p->v = *v;
  p->next = NULL;

  return p;
}

/**
 * array   : []
 * 	   | [ elements ]
 * 
 * elements: value
 *	   | value , elements
 */

static VALUE *parse_array(void)
{
  VALUE *v, *a = NULL;
  array_t *arr;
  long len = 0;

  arr = malloc(sizeof(array_t));

  do {
    a = realloc(a, (len+1)*sizeof(VALUE));
    switch(_cur.tok) {
      case T_OCBRACE:
        match(T_OCBRACE);
	a[len++] = *parse_object();
        match(T_CCBRACE);
        break;
      case T_OBRAC:
        match(T_OBRAC);
        a[len++] = *parse_array();
        match(T_CBRAC);
        break;
      case T_STRING:
	a[len++] = *newvalue(DT_STRING, _cur.buf);
	match(_cur.tok);
	break;
      case T_NUMBER:
	a[len++] = *newvalue(DT_NUMBER, &_cur.val);
	match(_cur.tok);
	break;
      case T_TRUE:
	a[len++] = *newvalue(DT_TRUE, _cur.buf);
	match(_cur.tok);
	break;
      case T_FALSE:
	a[len++] = *newvalue(DT_FALSE, _cur.buf);
	match(_cur.tok);
	break;
      case T_NULL:
	a[len++] = *newvalue(DT_NULL, _cur.buf);
	match(_cur.tok);
	break;
    }
  } while(accept(T_COMMA));
  
  arr->length = len;
  arr->a = a;
  v = newvalue(DT_ARRAY, arr);

  return v;
}

/**
 * pair: string : value
 */
static pair_t *parse_pair(void)
{
  pair_t *p;
  VALUE *v;

  char *n = strdup(_cur.buf);
  
  match(_cur.tok);
  if(accept(T_COLON)) {
    v = parse_value();
    p = newpair(n, v);
    return p;
  }
  return NULL;
}
/**
 * object  : {}
 * 	   | { members } 
 * 
 * members : pair
 * 	   | pair , members
 */
static VALUE *parse_object(void)
{
  VALUE *v;
  pair_t *p, *tail, *head = NULL;

  do {
    p = parse_pair();
    if(head == NULL) {
      head = tail = p;
    } else {
      tail->next = p;
      tail = p;
    }
  } while(accept(T_COMMA));
  
  v = newvalue(DT_OBJECT, head);

  return v;
}

/**
 * JSON grammar: From https://json.org/
 *
 * JSON : value
 *
 * value: string
 *	| number
 *	| object
 *	| array
 *	| true
 *	| false
 *	| null
 */
//static VALUE *newvalue(value_t type, void *value)
VALUE *parse_value(void)
{
  VALUE *v;

  switch(_cur.tok) {
    case T_OBRAC:
      match(T_OBRAC);
      v = parse_array();
      match(T_CBRAC);
      break;
    case T_OCBRACE:
      match(T_OCBRACE);
      v = parse_object();
      match(T_CCBRACE);
      break;
    case T_STRING:
      v = newvalue(DT_STRING, _cur.buf);
      match(_cur.tok);
      break;
    case T_NUMBER:
      v = newvalue(DT_NUMBER, &_cur.val);
      match(_cur.tok);
      break;
    case T_TRUE:
      v = newvalue(DT_TRUE, _cur.buf);
      match(_cur.tok);
      break;
    case T_FALSE:
      v = newvalue(DT_FALSE, _cur.buf);
      match(_cur.tok);
      break;
    case T_NULL:
      v = newvalue(DT_NULL, _cur.buf);
      match(_cur.tok);
      break;
    case T_EOI:
    case T_UNKNOWN:
      return NULL;
  }

  return v;
}
