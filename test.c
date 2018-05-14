#include <stdio.h>
#include <stdlib.h>
#include "json.h"

#define K 2048
/**
 * A test program to use your library.  You may modify this as needed. Use:
 *   make test
 * to build this program, then use 'runtest' to run it with the library in
 * the current working directory.
 */
int main(int argc, char *argv[])
{
  VALUE *v;
  char c, *buf;
  int in = 0, chunk = K;
  FILE *fp;
  
  if (argc < 2) {
    printf("Usage: test <file>\n");
    exit(1);
  }

  if((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open file %s\n", argv[1]);
    exit(1);
  }

  buf = malloc(chunk * sizeof(char));

  while((c = fgetc(fp)) != EOF) {
    buf[in++] = c;
    if(in >= chunk) {
      chunk += chunk;
      buf = realloc(buf, chunk * sizeof(char));
    }
  }
  buf[in] = '\0';

  v = json_decode(buf);
  if (v == NULL) {
    printf("Error parsing input.\n");
    exit(1);
  }
  printf("%s\n", json_encode(v));
  return 0;
}
