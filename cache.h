#ifndef CACHE_H
#define CACHE_H

#include <string>

#define CACHE_SIZE 10

class Cache {
private:
  // TODO: private inner struct/class 선언 가능
  // TODO: private 멤버 변수와 함수 추가 가능
  class Node {
  private:
    std::string key_;
    std::string type_;
    int integer_val_;
    double double_val_;
    Node *prev_;
    Node *next_;
    Node *hash_prev_;
    Node *hash_next_;
    
  public:
    Node(std::string key, void *value, std::string type);

    Node *getNext(){return next_;};
    Node *getPrev(){return prev_;};
    Node *getHashNext(){return hash_next_;};
    Node *getHashPrev(){return hash_prev_;};

    std::string getKey(){return key_;};
    std::string doubleToStringInCache();
    std::string getType(){return type_;};
    int getInt(){return integer_val_;};
    double getDouble(){return double_val_;};

    void setNext(Node *node);
    void setHashNext(Node *node);
  };

  Node *nodes_;
  Node *tail_;
  Node *hash_[CACHE_SIZE];
  int cur_size_;
  int max_size_;

  std::string result_;
  std::string type_int_;
  std::string type_double_;
  int hash_num_;
  void del();
  int hash_str(std::string key);
public:
  Cache();
  ~Cache();
  // int를 cache에 추가한다
  void add(std::string key, int value);
  // double을 cache에 추가한다
  void add(std::string key, double value);
  // key에 해당하는 value를 cache에서 가져온다
  // 타입과 key가 동일한 원소가 없다면 false를 반환한다.
  bool get(std::string key, int &value);
  // key에 해당하는 value를 cache에서 가져온다.
  // 타입과 key가 동일한 원소가 없다면 false를 반환한다.
  bool get(std::string key, double &value);

  // 디버그 및 채점 용: 연결 리스트를 문자열로 표현하여 반환한다
  // 다음과 같이 표현된 문자열을 반환한다
  // [key1: value1] -> [key2: value2] -> ... -> [keyN: valueN]
  std::string toString();
};

#endif