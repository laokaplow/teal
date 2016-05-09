#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <string>

struct Value {
  virtual ~Value() = 0;
  virtual bool is_self_evaluating() { return true; }
}

struct ExternallyEvaluated {
  virtual bool is_self_evaluating() { return false; }
}

struct Atom : public Value {
  string value;
};

struct Bool : public Value {
  bool value;
}

struct Character : public Value {
  char value;
}

struct Integer : public Value {
  int value;
};

struct Float : public Value {
  double value;
}

struct List : public Value {
  struct Empty;
  struct Node;
};

struct List::Empty : public List {
}

struct List::Node : public List, public ExternallyEvaluated {
  Ref<Value> first;
  Ref<List> rest;

  Node(Ref<Value> first, Ref<List> rest = make<List::Empty>())
    : first(first), rest(rest)
  {}
};

struct Procedure : public Value {
  struct Primitive;
  struct Compound;

  virtual Ref<Value> apply(Ref<Value> args, Ref<Value> env) = 0;
};

struct Procedure::Compound {
    Ref<Value> lexical_environment;
    Ref<Value> param_list;
    Ref<Value> body;

    Ref<Value> apply(Ref<Value> args, Ref<Value> env);
}

struct Procedure::Primitive {
  std::function<Ref<Value>, Ref<Value> > fn;

  Ref<Value> apply(Ref<Value> args, Ref<Value> env) {
    return fn(args);
  }
};

struct SpecialForm : public Value {
  std::function<Ref<Value>, Ref<Value> > fn;

  Ref<Value> eval(Ref<List> args) const {
    return fn(args);
  }
}

#endif
