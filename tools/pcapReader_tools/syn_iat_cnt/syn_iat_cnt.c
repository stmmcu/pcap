#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

#define MAX_IAT 300000

int
main(int argc, char *argv[])
{
  uint64_t h, m, s, us, useless, i;
  uint64_t prev, curr, iat, max = 0;
  FILE *fp;
  uint64_t table[MAX_IAT] = {0};

  if (argc != 2) {
    fprintf(stderr, "usage: %s [FILE NAME]\n", argv[0]);
    return 0;
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "cannot open file: %s\n", argv[1]);
    return 0;
  }

  fscanf(fp, "%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n", &h, &m, &s, &us, &useless);
  prev = ((h * 60 + m) * 60 + s) * 1000000 + us;

  while (!feof(fp)) {
    fscanf(fp, "%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n", &h, &m, &s, &us, &useless);
    curr = ((h * 60 + m) * 60 + s) * 1000000 + us;

    assert(curr >= prev);

    iat = curr - prev;
    if (iat >= MAX_IAT) {
      fprintf(stderr, "very large inter-arrival time: %"PRIu64"\n", iat);
      fprintf(stderr, "prev: %"PRIu64"\n", prev);
      fprintf(stderr, "curr: %"PRIu64"\n", curr);
      fprintf(stderr, "%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64"\n", h, m, s, us);
      continue;
    }

    table[iat]++;

    if (iat > max)
      max = iat;
    prev = curr;
  }

  fclose(fp);

  for (i = 0; i <= max; i++)
    printf("%"PRIu64" %"PRIu64"\n", i, table[i]);

  return 0;
}
