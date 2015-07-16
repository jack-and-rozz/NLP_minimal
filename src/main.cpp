

#include "util.h"
#include "vector_distance.h"

#include <iostream>
#include <memory>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;



//=========================
//   About Options
//=========================

struct opts {
  const char * word;
  const char * dataset;
  int n_top;
  opts() {
    word = "cat";
    dataset = "data/test.txt";
    n_top = 5;
  }
};


void usage(char * prog) {
  opts o;
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "  %s [options]\n", prog);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "  -w,--word  (%s)\n", o.word);
  fprintf(stderr, "  -d,--dataset  (%s)\n", o.dataset);
  fprintf(stderr, "  -n,--n_top  (%d)\n", o.n_top);
}

opts * parse_cmdline(int argc, char * const * argv, opts * o) {
  static struct option long_options[] = {
    {"word",     required_argument, 0, 'w' },
    {"dataset",     required_argument, 0, 'd' },
    {"n_top",     required_argument, 0, 'n' },
    {0,         0,                 0,  0 }
  };

  while (1) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "w:d:n:",
			long_options, &option_index);
    if (c == -1) break;

    switch (c) {
    case 'w':
      o->word = strdup(optarg);
      break;
    case 'd':
      o->dataset = strdup(optarg);
      break;
    case 'n':
      o->n_top = atoi(optarg);
      break;
    default:
      usage(argv[0]);
      exit(1);
    }
  }
  return o;
}



//=========================
//         Main
//=========================


int main(int argc, char** argv){
 
  opts o;
  parse_cmdline(argc, argv, &o);
  string filename = o.dataset;
  string word = o.word;

  srand((unsigned int)(cur_time()));



  VectorDistanceTest(filename, word, o.n_top);

  return 0;
}




