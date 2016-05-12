#ifndef TYPES_H_
#define TYPES_H_

#include "memory.h"
#include "debug.h"

#include <functional>
#include <string>
#include <vector>

struct Value {
  virtual ~Value() = 0;
  virtual bool is_self_evaluating() const { return true; }
  virtual std::string show() const = 0;
};

struct Atom : public Value {
  std::string value;

  Atom(std::string value = "") : value(value) {}
  std::string show() const {
    return value;
  }

  bool is_self_evaluating() const override { return false; }
};

struct Bool : public Value {
  bool value;

  Bool(bool value) : value(value) {}

  std::string show() const {
    return std::to_string(value);
  }
};

struct Character : public Value {
  char value;

  Character(char value) : value(value) {}

  std::string show() const {
    return std::string() + value;
  }
};

struct Integer : public Value {
  int value;

  Integer(int value) : value(value) {}

  std::string show() const {
    return std::to_string(value);
  }
};

struct Float : public Value {
  double value;

  Float(double value) : value(value) {}
  std::string show() const {
    return std::to_string(value);
  }
};

struct String : public Value {
  std::string value;

  String(std::string value) : value(value) {}
  std::string show() const {
    return std::string("\"") + value + "\"";
  }
};

struct List : public Value {
  struct Empty;
  struct Node;

  static Ref<List> fromVec(std::vector<Ref<Value>> v);
};

struct List::Empty : public List {
  std::string show() const { return "()"; };
};

struct List::Node : public List{
  Ref<Value> first;
  Ref<List> rest;

  Node(Ref<Value> first, Ref<List> rest = make<List::Empty>())
    : first(first), rest(rest)
  {}

  std::string show() const {
    std::string buffer = "(";
    buffer += first->show();
    auto rem = rest;
    while (auto more = match<List::Node>(rem)) {
      buffer += " ";
      buffer += more->first->show();
      rem = more->rest;
    }
    buffer += ")";

    return buffer;
  };
  virtual bool is_self_evaluating() const override { return false; }
};

struct PrimitiveProcedure : public Value {
  std::function<Ref<Value>(Ref<List>)> fn;

  PrimitiveProcedure(std::function<Ref<Value>(Ref<List>)> fn)
    : fn(fn)
  {}

  Ref<Value> apply(Ref<List> args) {
    return fn(args);
  }

  std::string show() const { return "<PrimitiveProcedure>"; };
};

struct SpecialForm : public Value {
  std::function<Ref<Value>(Ref<List>, Ref<List::Node>)> fn;

  SpecialForm(std::function<Ref<Value>(Ref<List>, Ref<List::Node>)> fn)
    : fn(fn)
  {}

  Ref<Value> eval(Ref<List> unevaluated_args, Ref<List::Node> env) {
    return fn(unevaluated_args, env);
  }

  std::string show() const { return "<Special Form>"; };
};

struct Lambda : public Value {
  Ref<List::Node> env;
  Ref<List> params;
  Ref<Value> body;

  Lambda(Ref<List::Node> env, Ref<List> params, Ref<Value> body)
    : env(env), params(params), body(body)
  {}

  Ref<Value> apply(Ref<List> args);

  std::string show() const { return "<Lambda>"; };
};

#endif
