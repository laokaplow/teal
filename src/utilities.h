#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "memory.h"
#include "types.h"

#include <iostream>

void error(std::string msg, Ref<Value> obj = {});

Ref<Value> apply(Ref<Value> proc, Ref<List> args);

Ref<Value> eval(Ref<Value> expr, Ref<List::Node> env);
Ref<List> eval_each(Ref<List> exprs, Ref<List::Node> env);

/// env ops
Ref<List::Node> default_env();
Ref<List> env_lookup(Ref<List::Node> env, Ref<Atom> name);
void env_set(Ref<List::Node> env, Ref<Atom> name, Ref<Value> new_val);
void define(Ref<List::Node> env, Ref<Atom> name, Ref<Value> value);
void define(Ref<List::Node> env, std::string name, Ref<Value> value);
Ref<List::Node> env_add_frame(Ref<List::Node> env, Ref<List> frame);

////
/// List ops
//
Ref<List> nil();
Ref<List::Node> cons(Ref<Value> a, Ref<List> b = nil());
Ref<List> mk_pair(Ref<Value> a, Ref<Value> b);
Ref<List> zip(Ref<List> as, Ref<List> bs);
Ref<Value> list_after(int index, Ref<List> l);
Ref<List> reverse(Ref<List> l,Ref<List> reveresed = nil());


namespace alist {
  Ref<List> lookup(std::string, Ref<List> dict);
  Ref<List> lookup(Ref<Value>, Ref<List> dict);
}

bool eq(Ref<Value> a, Ref<Value> b);



#endif
