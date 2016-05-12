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
  } else if (auto l = match<Lambda>(proc)) {
    return l->apply(args);
  } else if (auto p = match<List::Node>(proc)) {
    DEBUG("applying old school lambda");
    DEBUG(proc->show());
    error("this functionality has been temporarily disabled");
    return nil();

    // lambda
    auto lexical_env = match<List::Node>(list_after(0, p));
    auto params = match<List>(list_after(1, p));
    auto body = match<Value>(list_after(2, p));

    if (!(lexical_env && params && body)) {
      error("malformed functor");
    }

    // pair args
    auto arg_pack = zip(params, args);
    // extend-environment
    auto env = cons(arg_pack, lexical_env);
    DEBUG("special env = " << env->show());
    DEBUG("proc body = " << body->show())

    // eval in new env
    return eval(body, env);

  } else {

    error("Unable to apply args - uknown proc type." + proc->show());
  }

  return nil();
}

Ref<Value> eval(Ref<Value> exp, Ref<List::Node> env) {
  // DEBUG("Evaluating: " << exp->show());
  // terminal values
  if (exp->is_self_evaluating()) {
    // DEBUG("is self evaluating");
    return exp;
  }

  // variable
  if (auto name = match<Atom>(exp)) {
      // DEBUG("is atom");
      auto value = list_after(1, env_lookup(env, name));
      return value;
  }

  //  s-expr
  if (auto sexpr = match<List::Node>(exp)) {
    auto head = eval(sexpr->first, env);
    auto rest = sexpr->rest;

    if (auto form = match<SpecialForm>(head)) {
        // DEBUG("is special form");
      // may not evaluate all args, and/or may modify the env
      return form->eval(rest, env);
    }

    // DEBUG("is function");
    // DEBUG("head = " << head->show());
    // DEBUG("qargs = " << rest->show());
    // DEBUG("processing args into...");
    auto args = eval_each(rest, env);
    // DEBUG("args = " << args->show());
    // DEBUG("preparing to apply args to head..")
    return apply(head, args);
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
    while (auto head = match<List::Node>(pairs)) {
      if (auto kv = match<List::Node>(head->first)) {
        if (eq(key, kv->first)) {
          return kv;
        }
      } else {
        error("Malformed A-List, pair was not List::Node");
      }
      pairs = head->rest;
    }
    // if not found
    return nil();
  }
}

Ref<List> env_lookup(Ref<List::Node> env, Ref<Atom> name) {
  Ref<List> remaining = env;
  while (auto frame = match<List::Node>(remaining)) {
    // DEBUG("looking for " << name->show() << " in " << frame->show());
    if (auto pair_list = match<List>(frame->first)) {
      if (auto kv = match<List::Node>(alist::lookup(name, pair_list))) {
        return kv;
      } else {
        // should be empty list to signify not found
      }
    } else {
      error(" bad frame in env, should be list");
    }
    remaining = frame->rest;
  }
  // can't find value in empty env
  error(string("Lookup error, atom `") + name->show() +"` does not name a value in the env.");
  return nil();
};

void env_set(Ref<List::Node> env, Ref<Atom> name, Ref<Value> new_val) {
  if (auto kv = match<List::Node>(env_lookup(env, name))) {
    kv->rest = cons(new_val, nil());
  } else {
    // not found in env, return empty list
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

Ref<List::Node> env_add_frame(Ref<List::Node> env, Ref<List> frame) {
  return cons(frame, env);
}


Ref<List> nil() {
  return make<List::Empty>();
}

Ref<List::Node> cons(Ref<Value> a, Ref<List> b) {
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

bool vec_compare(Ref<Value> a, Ref<Value> b) {
  auto xs = match<Vector>(a);
  auto ys = match<Vector>(b);

  if (!(xs && ys)) {
    return false;
  }

  if (xs->contents.size() != ys->contents.size()) {
    return false;
  }

  for (int i = 0; i < xs->contents.size(); i++) {
    if (!eq(xs->contents[i], ys->contents[i])) {
      return false;
    }
  }

  return true;
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
   || vec_compare(a, b)
   || try_compare<Atom>(a, b)
   || try_compare<Bool>(a, b)
   || try_compare<Character>(a, b)
   || try_compare<Integer>(a, b)
   || try_compare<Float>(a, b)
   || try_compare<String>(a, b)
  ;
}

int len(Ref<Value> arg) {
  if (auto xs = match<List>(arg)) {
    int count = 0;
    while (auto head = match<List::Node>(xs)) {
      count += 1;
      xs = head->rest;
    }
    return count;
  } if (auto str = match<String>(arg)) {
    return str->value.size();
  } else {
    // error("Error: len called with non-list argument");
    return -1; // control will never reach here
  }
}
