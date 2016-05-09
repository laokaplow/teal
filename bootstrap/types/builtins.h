#ifndef TYPES_BUILTINS_H
#define TYPES_BUILTINS_H

#include "runtime/value.h"

#include <string>

struct Atom : public Value {
  string value;
};

struct Integer : public Value {
  int value;
};

struct Byte : public Value {
  char value;
}

struct Float : public Value {
  double value;
}

struct String : public Value {
  std::string value;
}

template <typename T>
struct List : public Value {
  vector<Ref<Value>> contents;

  void append(Ref<T> item) {
    contents.push_back(item);
  }
};

// (::+<Integer,Integer> 5 7)




#endif
