

#include "vector_distance.h"
#include "util.h"


#include <memory>
#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <iomanip>

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


void VectorDistanceTest(const string & filename, const int N, const string & sentence_file){
  string line;  
  unique_ptr<StringConverter> sc = make_unique<StringConverter>();
  vector<WordVecPtr> word_vecs; 
  vector<string> sentences;
  double t0,t1;

  // 文ベクトルの場合元テキストをロード
  if (sentence_file != ""){
    ifstream sent_file(sentence_file);
    if(sent_file.fail()){
      cout << "Failed to load the original sentence file" << endl;
      exit(1);
    }
    while(getline(sent_file, line)){
      sentences.push_back(line);
    }
  }

  // 単語ベクトルのロード
  t0 = cur_time();
  ifstream vec_file(filename);
  if(vec_file.fail()){
    cout << "Failed to load the word vectors" << endl;
    exit(1);
  }
  int c = 0;
  while(getline(vec_file, line)){
    if (c == 0){ c++; continue;} // ベクトルファイルの初めの一行目は行数・次元の情報なので読み飛ばす
    auto strv = split(line);
    auto word_vec = make_shared<WordVec>();

    for(auto it = strv.begin()+1; it < strv.end(); it++){
      word_vec->push_back(stod(*it));
    }
    normalize(*word_vec);
    word_vecs.push_back(word_vec);
    sc->AddStr(strv[0]);
  }
  t1 = cur_time();
  printf("Elapsed time(Load files): %.2f \n", t1-t0);

  //for(int i = 0; i < word_vecs.size(); i++){
    //ShowWordVec(sc->id2str(i), *word_vecs[i]);
  //}

  string word;
  cout << "Input the target word ..."<< endl;
  while(cin >> word){
    int target_id = sc->str2id(word);

    if(target_id < 0){
      cout <<  "Can't find the token [" << word << "]" << endl;
      continue;
    }

    if (sentence_file == ""){
      cout << "Target Word : [" << word << "]" <<endl;
      cout << endl;
    }else{
      cout << "Target Sentence : [" << sentences[target_id] << "]" <<endl;
      cout << endl;
    }

    //auto res = FindSimilarWords(word_vecs, target_id, N, EuclideanDistance);
    //cout << "< EuclideanDistance > "   << endl;
    //for(auto it = res.begin(); it != res.end(); it++){
    //  cout << (it - res.begin()) << ": " << setw(10) << sc->id2str(get<0>(*it)) << " ... " << (get<1>(*it)) <<endl;
    //}


    auto res = FindSimilarWords(word_vecs, target_id, N, CosineSimilarity);
    cout << "< CosineSimilarity >" << endl;
    if (sentence_file == ""){
      for(auto it = res.begin(); it != res.end(); it++){
	cout << (it - res.begin()) << ": " << setw(10) << sc->id2str(get<0>(*it)) << " ... " << (get<1>(*it)) <<endl;
      }
    }else{
      for(auto it = res.begin(); it != res.end(); it++){
	cout << (it - res.begin()) << ": " << setw(10) << sentences[get<0>(*it)] << " ... " << (get<1>(*it)) <<endl;
      }
    }
    cout << endl;
    cout << "Input the target word ..."<< endl;

  }


}


