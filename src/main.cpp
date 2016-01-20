

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
  const char * dataset;
  int n_top;
  const char * sentence_file;
  opts() {
    dataset = "data/test.txt";
    n_top = 10;
    sentence_file = "";
  }
};


void usage(char * prog) {
  opts o;
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "  %s [options]\n", prog);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "  -d,--dataset  (%s)\n", o.dataset);
  fprintf(stderr, "  -n,--n_top  (%d)\n", o.n_top);
  fprintf(stderr, "  -s,--sentence_file  (%s)\n", o.sentence_file);
}

opts * parse_cmdline(int argc, char * const * argv, opts * o) {
  static struct option long_options[] = {
    {"dataset",     required_argument, 0, 'd' },
    {"n_top",     required_argument, 0, 'n' },
    {"sentence_file",     required_argument, 0, 's' },
    {0,         0,                 0,  0 }
  };

  while (1) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "hw:d:n:s:",
			long_options, &option_index);
    if (c == -1) break;

    switch (c) {
    case 'd':
      o->dataset = strdup(optarg);
      break;
    case 'n':
      o->n_top = atoi(optarg);
      break;
    case 's':
      o->sentence_file = strdup(optarg);
      break;
    case 'h':
      usage(argv[0]);
      exit(1);
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
  string sentence_file = o.sentence_file;
  srand((unsigned int)(cur_time()));


  double t0 = cur_time();
  VectorDistanceTest(filename, o.n_top, o.sentence_file);
  double t1 = cur_time();
  cout << "Elapsed time : " << (t1-t0) << " sec" << endl;
  return 0;
}




