#ifndef TYPES_H_
#define TYPES_H_

#include "memory.h"

#include <functional>
#include <string>

struct Value {
  virtual ~Value() = 0;
  virtual bool is_self_evaluating() const { return true; }
  // virtual std::string show() const = 0;
};

struct ExternallyEvaluated {
  virtual bool is_self_evaluating() { return false; }
};

struct Atom : public Value {
  std::string value;

  Atom(std::string value = "") : value(value) {}
};

struct Bool : public Value {
  bool value;
};

struct Character : public Value {
  char value;
};

struct Integer : public Value {
  int value;

  Integer(int value) : value(value) {}
};

struct Float : public Value {
  double value;

  Float(double value) : value(value) {}
};

struct String : public Value {
  std::string value;

  String(std::string value) : value(value) {}
};

struct List : public Value {
  struct Empty;
  struct Node;
};

struct List::Empty : public List {
};

struct List::Node : public List, public ExternallyEvaluated {
  Ref<Value> first;
  Ref<List> rest;

  Node(Ref<Value> first, Ref<List> rest = make<List::Empty>())
    : first(first), rest(rest)
  {}


};

struct PrimitiveProcedure : public Value {
  std::function<Ref<Value>(Ref<Value>)> fn;

  Ref<Value> apply(Ref<Value> args) {
    return fn(args);
  }
};

struct SpecialForm : public Value {
  std::function<Ref<Value>(Ref<Value>)> fn;

  Ref<Value> eval(Ref<List> args, Ref<Value> env) const {
    return fn(args);
  }
};

#endif
