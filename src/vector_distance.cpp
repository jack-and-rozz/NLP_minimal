

#include "vector_distance.h"
#include "util.h"


#include <memory>
#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;


typedef vector<double> WordVec;
typedef shared_ptr<WordVec> WordVecPtr;

static void ShowWordVec(const string & word, const WordVec &wv){
  cout << word <<endl;
  for(auto it = wv.begin(); it != wv.end(); it++){
    cout << *it << " " ;
  }
  cout <<endl;
}



static vector<tuple<int ,double>> FindSimilarWords(vector<WordVecPtr>& word_vecs, const int id, const int N , double (*func)(const WordVecPtr, const WordVecPtr))
{
  vector<tuple<int, double>> res;
  vector<tuple<int, double>> res_selected;
  for(int i=0; i < word_vecs.size(); i++){
    double sim = func(word_vecs[i], word_vecs[id]);
    if(i != id){
      res.push_back(tuple<int, double>(i, sim));
    }
  }

  auto compare = [](const tuple<int, double> t1, const tuple<int, double> t2){
    return get<1>(t1) > get<1>(t2);
  };

  sort(res.begin(), res.end(), compare);
  for(int i = 0; i < N; i++){
    res_selected.push_back(res[i]);
  }

  return res_selected;
}


double EuclideanDistance(const WordVecPtr wv, const WordVecPtr wv2)
{
  double d = 0;
  for(int i = 0; i< wv->size(); i++){
    d += pow((*wv)[i] - (*wv2)[i], 2);
  }
  return 1-sqrt(d);
}


double CosineSimilarity(const WordVecPtr wv, const WordVecPtr wv2)
{
  double d = 0;
  for(int i = 0; i< wv->size(); i++){
    d += (*wv)[i] * (*wv2)[i];
  }
  return d;
}


void VectorDistanceTest(const string & filename, const string & word){

  ifstream ifs(filename);
  if(ifs.fail()){
    cout << "Failed to load n_gram data" << endl;
    exit(1);
  }
  string line;

  unique_ptr<StringConverter> sc = make_unique<StringConverter>();

  vector<WordVecPtr> word_vecs; 
  while(getline(ifs, line)){
    auto strv = split(line);

    auto word_vec = make_shared<WordVec>();

    for(auto it = strv.begin()+1; it < strv.end(); it++){
      word_vec->push_back(stod(*it));
    }
    normalize(*word_vec);
    word_vecs.push_back(word_vec);
    sc->AddStr(strv[0]);
  }

  for(int i = 0; i < word_vecs.size(); i++){
    ShowWordVec(sc->id2str(i), *word_vecs[i]);
  }
  int target_id = sc->str2id(word);

  if(target_id < 0){
    cout <<  "Can't find such word" << endl;
    exit(1);
  }
  
  

  auto res = FindSimilarWords(word_vecs, target_id, 3, EuclideanDistance);

  cout << "[EuclideanDistance]: " << word  << endl;
  for(auto it = res.begin(); it != res.end(); it++){
    cout << sc->id2str(get<0>(*it)) << " : " << (get<1>(*it)) <<endl;
  }

  res = FindSimilarWords(word_vecs, target_id, 3, CosineSimilarity);


  cout << "[CosineSimilarity]: " << word  << endl;
  for(auto it = res.begin(); it != res.end(); it++){
    cout << sc->id2str(get<0>(*it)) << " : " << (get<1>(*it)) <<endl;
  }

}


