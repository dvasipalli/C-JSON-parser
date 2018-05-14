#include "internal.h"

struct token _cur;

static int nextc(void);
static void unget(int c);
static token_t checkkeys(char *word);

/**
 * JSON strings are always double-quoted and may contain 0 or more characters
 * with the following allowed for characters:
 * char	  : any-Unicode-character-except-"-or-\-or-control-character
 *	  | \" | \\ | \/ | \b | \f | \n | \r | \t| \u four-hex-digits 
 */
static token_t lexstring(char *word)
{
  int wp = 0;
  char c, p;
 
  while((c = nextc()) != '"') {
  /* Support for escape characters should be added here */
    p = c;
    if(p == '\\') {
      word[wp++] = p;
      c = nextc();
    }
    word[wp++] = c;
  }
  word[wp] = '\0';
  return T_STRING;
}

static token_t lex(char *word, double *val)
{
  char stop[] = " \t\n[]{},:\"";
  int wp = 0;
  char c = nextc(), *end;
  word[wp] = '\0';

  while(isspace(c))
    c = nextc();

  switch(c) {
    case '\0':
    case EOF: return T_EOI;
    case '[': return T_OBRAC;
    case ']': return T_CBRAC;
    case '{': return T_OCBRACE;
    case '}': return T_CCBRACE;
    case ',': return T_COMMA;
    case ':': return T_COLON;
    case '"': return lexstring(word);
    default:
      do {
        word[wp++] = c;
	c = nextc();
      } while(c != '\0' && c != EOF && strchr(stop,c) == NULL);
      unget(c);
      word[wp] = '\0';
      if(isdigit(word[0]) || word[0] == '-') {
        *val = strtod(word, &end);
	if(*end != '\0') {
	  fprintf(stderr, "Invalid number\n");
	  exit(0);
	}
	return T_NUMBER; 
      }
      return checkkeys(word);
  }
}
static token_t checkkeys(char *word)
{
  if(strcasecmp(word, "true") == 0 || strcasecmp(word, "t") == 0)
    return T_TRUE;
  if(strcasecmp(word, "false") == 0 || strcasecmp(word, "f") == 0)
    return T_FALSE;
  if(strcasecmp(word, "null") == 0);
    return T_NULL;
  return T_UNKNOWN;
}
char *input = NULL;
static int save[K];
static int saved = 0;

static int nextc()
{
  char c;

  if(saved) {
    c = save[--saved];
  } else if(input) {
    c = *input;
    if(c)
      input++;
  } else {
    c = getchar();
  }

  return c;
}

static void unget(int c)
{
  save[saved++] = c;
}

void startlex(void)
{
  _cur.tok = lex(_cur.buf, &_cur.val);
}

void match(token_t tok)
{
  if(tok == _cur.tok)
    _cur.tok = lex(_cur.buf, &_cur.val);
  else {
    fprintf(stderr, "Unexpected '%s'\n", _cur.buf);
    exit(0);
  }
}

int accept(token_t tok)
{
  if(tok == _cur.tok) {
    match(tok);
    return 1;
  }
  return 0;
}
