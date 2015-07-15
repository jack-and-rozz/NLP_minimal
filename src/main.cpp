

#include "util.h"

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
  const char * method;
  int rareword_threshold;
  opts() {
    method = "learn";
    rareword_threshold = 10;
  }
};


void usage(char * prog) {
  opts o;
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "  %s [options]\n", prog);
  fprintf(stderr, "options:\n");
  fprintf(stderr, "  -m,--method  (%s)\n", o.method);
  fprintf(stderr, "  -t,--threshold  (%d)\n", o.rareword_threshold);
}

opts * parse_cmdline(int argc, char * const * argv, opts * o) {
  static struct option long_options[] = {
    {"method",     required_argument, 0, 'm' },
    {"rareword_threshold",     required_argument, 0, 't' },
    {0,         0,                 0,  0 }
  };

  while (1) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "m:t:",
			long_options, &option_index);
    if (c == -1) break;

    switch (c) {
    case 'm':
      o->method = strdup(optarg);
      break;
    case 't':
      o->rareword_threshold = atoi(strdup(optarg));
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
  
  if (argc < 1){
    exit(1);
  }
  srand((unsigned int)(cur_time()));

  // opts o;
  // parse_cmdline(argc, argv, &o);
  // const string method = o.method;
  auto sc = make_shared<StringConverter>();
  sc->AddStr("word");
  sc->AddStr("word");
  cout << sc->id2str(0) <<endl;
  cout << sc->GetCount() <<endl;
  return 0;
}



