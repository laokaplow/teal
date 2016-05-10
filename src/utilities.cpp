#include "utilities.h"
#include "types.h"

#include <stdexcept>

using namespace std;

void error(string msg, Ref<Value> obj = {}) {
  throw runtime_error(msg);
}


Ref<Value> lookup_variable_value(Ref<Atom> name, Ref<List> env);
Ref<List> list_of_values(Ref<List> exps, Ref<List> env);

Ref<Value> eval(Ref<Value> exp, Ref<List> env) {
  // terminal values
  if (exp->is_self_evaluating()) {
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
      return form->eval(rest, env);
    }

    return apply(head, list_of_values(rest, env));
  }

  error("Unknown expression type - EVAL");
  return nil();
}

Ref<Value> apply(Ref<Value> proc, Ref<List> args) {
  if (auto p = match<PrimitiveProcedure>(proc)) {
    return p->apply(args);
  } else if (auto p = match<List::Node>(proc)) {
    auto lexical_env = alist::lookup("lexical_env", p);
    auto parameters = alist::lookup("params", p);
    auto body = alist::lookup("body", p);

    // extend-environment

  } else {
    error("Unable to apply args - wrong type.");
  }

  return nil();
}
namespace alist {
  Ref<List> lookup(string key, Ref<List> pairs) {
    return lookup(make<Atom>(key), pairs);
  }

  Ref<List> lookup(Ref<Value> key, Ref<List> pairs) {
    if (auto l = match<List::Node>(pairs)) {
      if (auto p = match<List::Node>(l->first)) {
        if (eq(key, p->first)) {
          return p->rest;
        }
      } else {
        error("Malformed A-List, pair was not List::Node");
      }
    }
    return nil();
  }
}

Ref<Value> lookup_variable_value(Ref<Atom> name, Ref<List> env) {
  if (auto frames = match<List::Node>(env)) {
    if (auto table = match<List>(frames->first)) {
      if (auto res = match<List::Node>(alist::lookup(name, table))) {
          return res;
      }
    } else {
      error("env frame is not list");
    }

    return lookup_variable_value(name, frames->rest);
  }


  // can't find value in empty env
  error("Variable not found in env.");
  return nil();
};

Ref<List> list_of_values(Ref<List> exps, Ref<List> env) {
  if (auto none = match<List::Empty>(exps)) {
    return nil();
  }
  if (auto some = match<List::Node>(exps)) {
    return cons(eval(some->first, env), list_of_values(some->rest, env));
  }
  return nil();
}


Ref<List> nil() {
  return make<List::Empty>();
}

Ref<List> cons(Ref<Value> a, Ref<List> b) {
  return  make<List::Node>(a, b);
}

Ref<List> mk_pair(Ref<Value> a, Ref<Value> b) {
  return cons(a, cons(b));
}

Ref<List> zip(Ref<List> keys, Ref<List> vals) {
  auto ks = match<List::Node>(keys);
  auto vs = match<List::Node>(vals);

  if (ks && vs) {
    return cons(mk_pair(ks->first, vs->first), zip(ks->rest, vs->rest));
  } else if (!ks && !vs) {
    return nil();
  } else {
    error("zip on lists of different lengths");
  }

  return nil();
};

Ref<Value> list_after(Ref<List> l, int index) {
  if (auto xs = match<List::Node>(l)) {
     if (index == 0) {
      return xs->first;
    } else {
      return list_after(xs->rest, index-1);
    }
  }
  return nil();
}

bool list_compare(Ref<Value> a_, Ref<Value> b_) {
  {
    auto a = match<List::Empty>(a_);
    auto b = match<List::Empty>(b_);

    if (a && b) return true;
  }
  {
    auto a = match<List::Node>(a_);
    auto b = match<List::Node>(b_);

    if (a && b) {
      return eq(a->first, b->first) && list_compare(a->rest, b->rest);
    }
  }
  return false;
}

Ref<List> reverse(Ref<List> l, Ref<List> reversed) {
  if (auto node = match<List::Node>(l)) {
    if (auto end = match<List::Empty>(node->rest)) {
      return cons(node->first, reversed);
    } else if (auto remaining = match<List::Node>(node->rest))
      return reverse(remaining, cons(node->first, reversed));
    } else if (auto end = match<List::Empty>(l)) {
    return reversed;
  }

  // control flow should never reach here
  return nil();
}



template <typename T>
bool try_compare(Ref<Value> a_, Ref<Value> b_) {
  auto a = match<T>(a_);
  auto b = match<T>(b_);

  return a && b && (a->value == b->value);
}
bool eq(Ref<Value> a, Ref<Value> b) {
  return (a.get() == b.get()) // reference equality
   || list_compare(a, b)
   || try_compare<Atom>(a, b)
   || try_compare<Bool>(a, b)
   || try_compare<Character>(a, b)
   || try_compare<Integer>(a, b)
   || try_compare<Float>(a, b)
   || try_compare<String>(a, b)
  ;
}
