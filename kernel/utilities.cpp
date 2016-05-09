
#include <runtime.cpp>
#include "types.h"
#include "procedures.h"
#include "special_forms.h"

Ref<Value> lookup_variable_value(Ref<Atom> name, Ref<List> env) {

Ref<Value> eval(Ref<Value> exp, Ref<List> env) {
  // terminal values
  if (exp.is_self_evaluating()) {
    return exp;
  }

  // variable
  if (auto atom = match<Atom>(exp)) {
    return lookup_variable_value(atom, env);
  }

  //  s-expr
  if (auto sexpr = match<List::Node>(exp)) {
    auto head = eval(sexpr->first, env);
    auto rest = sexpr->rest;

    if (auto form = match<SpecialForm>(head)) {
      // may not evaluate all args, and/or may modify the env
      return form.eval(rest, env);
    }

    if (auto proc = match<Procedure>(head)) {
      return proc.apply(list_of_values(rest, env));
    }

    return error("Invalid value type in head position of s-expr", exp);
  }

  return error("Unknown expression type - EVAL", exp);
}

Ref<List> try_find_in_a_list(Ref<Value> key, Ref<List> alist) {
  if (auto l = match<List::Node>(alist)) {
    if (auto pair = match<List::Node>(l->first)) {
      if (eq(key, pair->first)) {
        return pair->second;
      }
    } else {
      error("Malformed A-List, pair was not List::Node");
    }-
  }
  
  return make<List::Empty>();
}

Ref<Value> lookup_variable_value(Ref<Atom> name, Ref<List> env) {
  if (auto frames = match<List::Node>(env)) {
    if (auto res = match<List::Node>(try_find_in_a_list(name, frames->first))) {
        return res;
    }

    return lookup_variable_value(name, frames->rest);
    }
  }

  // can't find value in empty env
  return error("Variable not found in env.", name);
};
