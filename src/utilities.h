#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "memory.h"
#include "types.h"

#include <iostream>

Ref<Value> eval(Ref<Value> expr, Ref<List> env);
Ref<Value> apply(Ref<Value> proc, Ref<List> args);

Ref<List> eval_each(Ref<List> exprs, Ref<List> env);
Ref<List> default_env();

////
/// List ops
//
Ref<List> nil();
Ref<List> cons(Ref<Value> a, Ref<List> b = nil());
Ref<List> pair(Ref<Value> a, Ref<Value> b);
Ref<List> zip(Ref<List> as, Ref<List> bs);
Ref<Value> list_after(Ref<List> l, int index);
Ref<List> reverse(Ref<List> l,Ref<List> reveresed = nil());

namespace alist {
  Ref<List> lookup(std::string, Ref<List> dict);
  Ref<List> lookup(Ref<Value>, Ref<List> dict);
}

bool eq(Ref<Value> a, Ref<Value> b);



#endif
