

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <deque>

double cur_time();

std::wstring widen(const std::string &);
std::string narrow(const std::wstring &src);


std::vector<std::string> split(const std::string &, const char delim = ' ');




class StringConverter{
  private:
  std::unique_ptr<std::unordered_map<std::string, int>> m_str2id ;
  std::unique_ptr<std::vector<std::string>> m_id2str ;
  int m_count;
  public:
  StringConverter();
  virtual  ~StringConverter() = default;
  int str2id(const std::string &str) const;
  std::string id2str(const int id) const;
  void AddStr(const std::string & str);
  inline int GetCount(){return m_count;};
};

#endif
