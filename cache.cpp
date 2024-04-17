#include "cache.h"
#include <string>
#include <iostream>
#include <sstream>

// TODO: 필요한 함수 구현

Cache::Cache(){
  cur_size_ = 0;
  max_size_ = CACHE_SIZE;
  nodes = NULL;
  type_int_ = "int";
  type_double_ = "double";
  for(int i = 0; i < CACHE_SIZE; i++){
    hash_[i] = NULL;
  }
}

Cache::~Cache(){
  delete nodes;
}

Cache::Node::Node(std::string key, void *value, std::string type):key_(key), prev_(NULL), next_(NULL), hash_prev_(NULL), hash_next_(NULL), hash_size_(0){
  if(type == "double"){
    double_val_ = *(double *)value;
    type_ = type;
  }
  else{
    integer_val_ = *(int *)value;
    type_ = type;
  }
  
}

void Cache::Node::setNext(Cache::Node *node){
  next_ = node;
  if(node != NULL)
    next_ -> prev_ = this;
}

void Cache::Node::setHashNext(Cache::Node *node){
  hash_next_ = node;
  if(node != NULL)
    hash_next_ -> hash_prev_ = this;
}

void Cache::Node::setHashSize(){
  hash_size_++;
}

std::string Cache::Node::doubleToStringInCache() {
  std::ostringstream ss;
  ss << double_val_;
  return ss.str();
}
// int를 cache에 추가한다
void Cache::add(std::string key, int value){
  Node *newNode = new Node(key, &value, type_int_);
  Node *cur = hash_[hash_num_];
  hash_num_ = hash_str(key) % 10;
  if(hash_[hash_num_] != NULL){
    int count = 0;
    while(cur != NULL){
      cur = cur -> getHashNext();
      count++;
    }
    cur -> setHashNext(newNode);
    cur -> setHashSize();
  }
  else{
    hash_[hash_num_] = newNode;
    hash_[hash_num_] -> setHashSize();
  }
  cur = nodes;
  Node* pre = nodes;
  if(nodes != NULL)
    newNode -> setNext(nodes);
  nodes = newNode;
  if(cur_size_ >= max_size_){
    while(cur != NULL){
      pre = cur;
      cur = cur -> getNext();
    }
    pre = pre -> getPrev();
    pre -> setNext(NULL);
  }
  else{
    cur_size_++;
  }
  std::cout << "add" << std::endl;
}
  // double을 cache에 추가한다
void Cache::add(std::string key, double value){
  Node *newNode = new Node(key, &value, type_double_);
  
  Node *cur = hash_[hash_num_];
  hash_num_ = hash_str(key) % 10;
  if(hash_[hash_num_] != NULL){
    int count = 0;
    while(cur != NULL){
      cur = cur -> getHashNext();
      count++;
    }
    cur -> setHashNext(newNode);
    cur -> setHashSize();
  }
  else{
    hash_[hash_num_] = newNode;
    hash_[hash_num_] -> setHashSize();
  }
  cur = nodes;
  Node* pre = nodes;
  if(nodes != NULL)
    newNode -> setNext(nodes);
  nodes = newNode;
  if(cur_size_ >= max_size_){
    while(cur != NULL){
      pre = cur;
      cur = cur -> getNext();
    }
    pre = pre -> getPrev();
    pre -> setNext(NULL);
  }
  else{
    cur_size_++;
  }
  std::cout << "add" << std::endl;
}
// key에 해당하는 value를 cache에서 가져온다
// 타입과 key가 동일한 원소가 없다면 false를 반환한다.
bool Cache::get(std::string key, int &value){
  std::cout << "get" << std::endl;
  hash_num_ = hash_str(key) % 10;
  if(hash_[hash_num_] != NULL){
    std::cout << "no error" << std::endl;
    if(hash_[hash_num_] -> getKey() == key){
      value = hash_[hash_num_] -> getInt();
      if(hash_[hash_num_] -> getType() != "int"){
        return false;
      }
      return true;
    }
    else{
      Node *cur = hash_[hash_num_];
      while(cur != NULL){
        if(cur -> getKey() == key){
          value = cur->getInt();
          return true;
        }
        cur = cur -> getHashNext();
      }
    }
  }
  
  return false;
}
// key에 해당하는 value를 cache에서 가져온다.
// 타입과 key가 동일한 원소가 없다면 false를 반환한다.
bool Cache::get(std::string key, double &value){
  std::cout << "getDouble" << std::endl;
  hash_num_ = hash_str(key) % 10;
  if(hash_[hash_num_] != NULL){
    if(hash_[hash_num_] -> getKey() == key){
      value = hash_[hash_num_] -> getDouble();
      if(hash_[hash_num_] -> getType() != "double"){
        return false;
      }
      return true;
    }
    else{
      Node *cur = hash_[hash_num_];
      int count = 0;
      while(cur != NULL){
        if(cur -> getKey() == key){
          value = cur->getInt();
          return true;
        }
        cur = cur -> getHashNext();
        count++;
      }
    }
  }
  return false;
}

// 디버그 및 채점 용: 연결 리스트를 문자열로 표현하여 반환한다
// 다음과 같이 표현된 문자열을 반환한다
// [key1: value1] -> [key2: value2] -> ... -> [keyN: valueN]
std::string Cache::toString(){
  std::cout << "toString" << std::endl;
  std::string result = "";
  Node *cur = nodes;
  int value_i;
  double value_d;
  std::string cur_key = cur -> getKey();
  while(cur != NULL){
    if(this -> get(cur_key, value_i))
        result += "[" + cur_key + ": " + std::to_string(cur -> getInt()) + "]";
    else if(this -> get(cur_key, value_d))
        result += "[" + cur_key + ": " + cur -> doubleToStringInCache() + "]";
    cur = cur -> getNext();
    if(cur != NULL){
      result += " -> ";
      cur_key = cur -> getKey();
    }
  }

  return result;
  
}