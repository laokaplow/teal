#include "utilities.h"
#include "types.h"

#include "debug.h"

#include <stdexcept>

using namespace std;

void error(string msg, Ref<Value> obj) {
  throw runtime_error(msg);
}

Ref<Value> apply(Ref<Value> proc, Ref<List> args) {
  if (auto p = match<PrimitiveProcedure>(proc)) {
    return p->apply(args);
  } else if (auto p = match<List::Node>(proc)) {
    auto lexical_env = match<List::Node>(list_after(0, p));
    auto params = match<List>(list_after(1, p));
    auto body = match<List>(list_after(2, p));

    if (!(lexical_env && params && body)) {
      error("malformed functor");
    }

    // pair args
    // extend-environment
    // eval in new env

  } else {
    error("Unable to apply args - wrong type.");
  }

  return nil();
}

Ref<Value> eval(Ref<Value> exp, Ref<List::Node> env) {
  DEBUG("Evaluating: " << exp->show());
  // terminal values
  if (exp->is_self_evaluating()) {
    DEBUG("is self evaluating");
    return exp;
  }

  // variable
  if (auto name = match<Atom>(exp)) {
      DEBUG("is atom");
      auto value = list_after(1, env_lookup(env, name));
      return value;
  }

  //  s-expr
  if (auto sexpr = match<List::Node>(exp)) {
    auto head = eval(sexpr->first, env);
    auto rest = sexpr->rest;

    if (auto form = match<SpecialForm>(head)) {
        DEBUG("is special form");
      // may not evaluate all args, and/or may modify the env
      return form->eval(rest, env);
    }

    DEBUG("is function");
    return apply(head, eval_each(rest, env));
  }

  error("Unknown expression type - EVAL");
  return nil();
}

Ref<List> eval_each(Ref<List> exps, Ref<List::Node> env) {
  if (auto none = match<List::Empty>(exps)) {
    return nil();
  }
  if (auto some = match<List::Node>(exps)) {
    auto first = eval(some->first, env);
    return cons(first, eval_each(some->rest, env));
  }
  return nil();
}

namespace alist {
  Ref<List> lookup(string key, Ref<List> pairs) {
    return lookup(make<Atom>(key), pairs);
  }

  Ref<List> lookup(Ref<Value> key, Ref<List> pairs) {
    // DEBUG("looking for " << key->show() << " in " << pairs->show());
    while (auto l = match<List::Node>(pairs)) {
      if (auto kv = match<List::Node>(l->first)) {
        if (eq(key, kv->first)) {
          return kv;
        }
      } else {
        error("Malformed A-List, pair was not List::Node");
      }
      pairs = l->rest;
    }
    // if not found
    return nil();
  }
}

Ref<List> env_lookup(Ref<List::Node> env, Ref<Atom> name) {
  if (auto frame = match<List::Node>(env->first)) {
    // DEBUG("looking for " << name->show() << " in " << frame->show());
    if (auto kv = match<List::Node>(alist::lookup(name, frame))) {
      return kv;
    }
  } else {
    if (auto more = match<List::Node>(env->rest)) {
      return env_lookup(more, name);
    }
  }
  // can't find value in empty env
  error(string("Lookup error, atom `") + name->show() +"` does not name a value in the env.");
  return nil();
};

void env_set(Ref<List::Node> env, Ref<Atom> name, Ref<Value> new_val) {
  if (auto kv = match<List::Node>(env_lookup(env, name))) {
    kv->rest = cons(new_val, nil());
  }
};

void define(Ref<List::Node> env, Ref<Atom> name, Ref<Value> value) {
  if (auto frame = match<List>(env->first)) {
    env->first = cons(mk_pair(name, value), frame);
  } else {
    error("problem with environment - top frame is not a list");
  }
}

void define(Ref<List::Node> env, string name, Ref<Value> value) {
  return define(env, make<Atom>(name), value);
}


Ref<List> nil() {
  return make<List::Empty>();
}

Ref<List> cons(Ref<Value> a, Ref<List> b) {
  return  make<List::Node>(a, b);
}

Ref<List> mk_pair(Ref<Value> a, Ref<Value> b) {
  return cons(a, cons(b, nil()));
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

Ref<Value> list_after(int index, Ref<List> l) {
  if (auto xs = match<List::Node>(l)) {
     if (index == 0) {
      return xs->first;
    } else {
      return list_after(index-1, xs->rest);
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
  // DEBUG("comparing equality of " << a->show() << " and " << b->show());
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
