#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LEN 100

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("missing FILE argument\n");
    exit(1);
  }
  char *filename = argv[1];

  FILE *file;
  char buffer[BUFFER_LEN];
  file = fopen(filename, "r");

  // read all lines
  while(fgets(buffer, BUFFER_LEN, file)) {
    // print the line
    printf("%s", buffer);
  }

  fclose(file);
}
