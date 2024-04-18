#include "cached_runner.h"
#include "task_runner.h"
#include <string>
#include <iostream>
#include <sstream>

// TODO: 필요한 함수 구현

CachedRunner::CachedRunner(Cache &cache){
  hits_ = 0;
  misses_ = 0;
  cache_ = &cache;
}

// 파일의 숫자를 전부 곱하여 반환한다
double CachedRunner::multiply(std::string filename){
  key_ = "multiply(" + filename + ")";
  if(!(cache_ -> get(key_, double_val_))){
    double_val_ = TaskRunner::multiply(filename);
    misses_++;
  }
  else{
    hits_++;
  }
  cache_ -> add(key_, double_val_);
  return double_val_;
}

// 파일에서 가장 긴 팰린드롬을 찾아 인덱스를 반환한다
int CachedRunner::palindrome(std::string filename){
  key_ = "palindrome(" + filename + ")";
  if(!(cache_ -> get(key_, integer_val_))){
    integer_val_ = TaskRunner::palindrome(filename);
    misses_++;
  }
  else{
    hits_++;
  }
  cache_ -> add(key_, integer_val_);
  return integer_val_;
}

// 캐시에 히트한 횟수를 반환한다
int CachedRunner::hits(){
  return hits_;
}

// 캐시에 미스한 횟수를 반환한다
int CachedRunner::misses(){
  return misses_;
}