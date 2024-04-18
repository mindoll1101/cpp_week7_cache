#include "cache.h"
#include <string>
#include <iostream>
#include <sstream>

// TODO: 필요한 함수 구현

Cache::Cache(){
  cur_size_ = 0;
  max_size_ = CACHE_SIZE;
  nodes_ = NULL;
  tail_ = NULL;
  type_int_ = "int";
  type_double_ = "double";
  for(int i = 0; i < CACHE_SIZE; i++){
    hash_[i] = NULL;
    hash_tail_[i] = NULL;
  }
}

Cache::~Cache(){
  delete nodes_;
}

Cache::Node::Node(std::string key, void *value, std::string type):key_(key), prev_(NULL), next_(NULL), hash_prev_(NULL), hash_next_(NULL){
  if(type == "double"){
    double_val_ = *(double *)value;
    type_ = type;
  }
  else{
    integer_val_ = *(int *)value;
    type_ = type;
  }
  
}

void Cache::Node::setNext(Node *node){
  next_ = node;
  if(node != NULL)
    next_ -> prev_ = this;
}

void Cache::Node::setHashNext(Node *node){
  hash_next_ = node;
  if(node != NULL)
    hash_next_ -> hash_prev_ = this;
}

std::string Cache::Node::doubleToStringInCache() {
  std::ostringstream ss;
  ss << double_val_;
  return ss.str();
}


int Cache::hash_str(std::string key){
  int mul = 0;
  for(int i = 0; i < key.length(); i++){
    mul *= key[i];
  }
  return mul % CACHE_SIZE;
}


// key에 해당하는 value를 cache에서 가져온다
// 타입과 key가 동일한 원소가 없다면 false를 반환한다.
bool Cache::get(std::string key, int &value){
  hash_num_ = hash_str(key);
  if(hash_[hash_num_] != NULL){
    if(hash_[hash_num_] -> getKey() == key){
      if(hash_[hash_num_] -> getType() == "int"){
        value = hash_[hash_num_] -> getInt();
        return true;
      }
    }
    else{
      Node *cur = hash_[hash_num_];
      while(cur != NULL){
        if(cur -> getKey() == key && cur -> getType() == "int"){
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
  hash_num_ = hash_str(key);
  if(hash_[hash_num_] != NULL){
    if(hash_[hash_num_] -> getKey() == key){
      if(hash_[hash_num_] -> getType() == "double"){
        value = hash_[hash_num_] -> getDouble();
        return true;
      }
    }
    else{
      Node *cur = hash_[hash_num_];
      while(cur != NULL){
        if(cur -> getKey() == key && cur -> getType() == "double"){
          value = cur->getDouble();
          return true;
        }
        cur = cur -> getHashNext();
      }
    }
  }
  return false;
}
void Cache::del(){
  std::string del_key = tail_ -> getKey();
  hash_num_ = hash_str(del_key);
  Node *cur = hash_[hash_num_];
  while(cur -> getKey() != del_key){
    cur = cur -> getHashNext();
  }
  if(cur -> getHashPrev() != NULL){
    cur -> getHashPrev() -> setHashNext(cur -> getHashNext());
  }
  else{
    hash_[hash_num_] = cur -> getHashNext();
  }
  Node *pre = tail_ -> getPrev();
  pre -> setNext(NULL);
  delete tail_;
  tail_ = pre;
  cur_size_--;
}
// int를 cache에 추가한다
void Cache::add(std::string key, int value){
  cur_size_++;
  hash_num_ = hash_str(key);
  Node *newNode = new Node(key, &value, type_int_);
  Node *cur = hash_[hash_num_];
  int value_i;
  // cache에 이미 있는 key일 경우
  if(this -> get(key, value_i)){
    while(cur -> getKey() != key){
      cur = cur -> getHashNext();
    }
    if(cur -> getHashPrev() != NULL){
      cur -> getHashPrev() -> setHashNext(cur -> getHashNext());
    }
    else{
      hash_[hash_num_] = hash_[hash_num_] -> getHashNext();
    }
    if(cur -> getPrev() != NULL){
      if(tail_ == cur){
        tail_ = tail_ -> getPrev();
      }
      cur -> getPrev() -> setNext(cur -> getNext());
    }
    else
      nodes_ = nodes_ -> getNext();
    cur_size_--;
  }

  cur = nodes_;
  if(nodes_ != NULL){
    newNode -> setNext(nodes_);
  }
  else{
    tail_ = newNode;
  }
  nodes_ = newNode;


  if(hash_[hash_num_] != NULL){
    newNode -> setHashNext(hash_[hash_num_]);
  }
  hash_[hash_num_] = newNode;

  if(cur_size_ > max_size_){
    del();
  }
}
  // double을 cache에 추가한다
void Cache::add(std::string key, double value){
  cur_size_++;
  hash_num_ = hash_str(key);
  Node *newNode = new Node(key, &value, type_double_);
  Node *cur = hash_[hash_num_];
  double value_d;
  // cache에 이미 있는 key일 경우
  if(this -> get(key, value_d)){
    while(cur -> getKey() != key){
      cur = cur -> getHashNext();
    }
    if(cur -> getHashPrev() != NULL){
      cur -> getHashPrev() -> setHashNext(cur -> getHashNext());
    }
    else{
      hash_[hash_num_] = hash_[hash_num_] -> getHashNext();
    }
    if(cur -> getPrev() != NULL){
      if(tail_ == cur){
        tail_ = tail_ -> getPrev();
      }
      cur -> getPrev() -> setNext(cur -> getNext());
    }
    else
      nodes_ = nodes_ -> getNext();
    cur_size_--;
  }

  cur = nodes_;
  if(nodes_ != NULL){
    newNode -> setNext(nodes_);
  }
  else{
    tail_ = newNode;
  }
  nodes_ = newNode;


  if(hash_[hash_num_] != NULL){
    newNode -> setHashNext(hash_[hash_num_]);
  }
  hash_[hash_num_] = newNode;

  if(cur_size_ > max_size_){
    del();
  }
}
// 디버그 및 채점 용: 연결 리스트를 문자열로 표현하여 반환한다
// 다음과 같이 표현된 문자열을 반환한다
// [key1: value1] -> [key2: value2] -> ... -> [keyN: valueN]
std::string Cache::toString(){
  std::string result = "";
  Node *cur = nodes_;
  while(cur != NULL){
    if(cur -> getType() == "int")
        result += "[" + cur -> getKey() + ": " + std::to_string(cur -> getInt()) + "]";
    else if(cur -> getType() == "double")
        result += "[" + cur -> getKey() + ": " + cur -> doubleToStringInCache() + "]";
    cur = cur -> getNext();
    if(cur != NULL){
      result += " -> ";
    }
  }
  result += "\n";
  return result;
  
}