/**
 * This header file should be included by any program that wants
 * to use your library and so represents the API for the library and should not
 * be modified.  Probably should have better, more JSONy names for things.
 */
#ifndef _JSON_H
#define _JSON_H 1

// JSON value types
typedef enum {
  DT_STRING, DT_NUMBER, DT_OBJECT, DT_ARRAY, DT_TRUE, DT_FALSE, DT_NULL
} value_t;

struct value {
  value_t type;
  union {
    void *any;		// Can be used to quickly assign a pointer value (but not double)
    char *s;
    double n;
    struct pair *o;	// An object
    struct array *a;
  };
};

struct array {
  long length;
  struct value *a;	// a may be NULL if length == 0.
};

// An object is a list of pairs:
struct pair {
  char *name;
  struct value v;
  struct pair *next;
};

typedef struct array array_t;
typedef struct pair pair_t;
typedef struct value VALUE;

// Exported functions:
VALUE *json_decode(char *s);
char *json_encode(VALUE *v);

#endif
