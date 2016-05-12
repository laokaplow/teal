#include "builtins.h"
#include "utilities.h"
#include "types.h"
#include "debug.h"

#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

Ref<List::Node> default_env() {
  auto env = make<List::Node>(make<List::Empty>());

  attach_special_forms(env);
  attach_builtin_fns(env);

  // constants
  define(env, "pi", make<Float>(3.1415926));

  // functions
  define(env, "display", make<PrimitiveProcedure>([](Ref<List> args) {
    while (auto xs = match<List::Node>(args)) {
      cout << xs->first->show();
      args = xs->rest;
    }

    return nil();
  }));
  //
  // define(env, "empty?", make<PrimitiveProcedure>([](Ref<List> args) {
  //   return make<Bool>(!!match<List::Empty>(list_after(0, args)));
  //
  // }));
  //
  // define(env, "atom?", make<PrimitiveProcedure>([](Ref<List> args) {
  //   return make<Bool>(!!match<Atom>(list_after(0, args)));
  // }));
  //
  // define(env, "=", make<PrimitiveProcedure>([](Ref<List> args) {
  //   auto a = list_after(0, args);
  //   auto b = list_after(1, args);
  //
  //   return make<Bool>(eq(a, b));
  // }));
  //
  // define(env, "read-lines", make<PrimitiveProcedure>([](Ref<List> args) {
  //   string buf;
  //   vector<Ref<Value>> lines;
  //   while (cin) {
  //     getline(cin, buf, '\n');
  //     if (!cin && buf.size() == 0) continue;
  //     vector<Ref<Value>> cs;
  //     for (char &c: buf) {
  //       cs.push_back(make<Character>(c));
  //     }
  //     lines.push_back(List::fromVec(cs));
  //   }
  //   auto res = List::fromVec(lines);
  //   DEBUG(res->show());
  //   return res;
  // }));
  //
  // define(env, "exit", make<PrimitiveProcedure>([](Ref<List> args) {
  //   int code = 0;
  //   if (auto user_code = match<Integer>(list_after(0, args))) {
  //     code = user_code->value;
  //   }
  //   exit(code);
  //
  //   return nil(); // control flow never reaches here
  // }));
  //
  // define(env, "len", make<PrimitiveProcedure>([](Ref<List> args) {
  //   if (auto xs = match<List>(list_after(0, args))) {
  //     int count = 0;
  //     while (auto head = match<List::Node>(xs)) {
  //       count += 1;
  //       xs = head->rest;
  //     }
  //     return make<Integer>(count);
  //   } else {
  //     error("Error: len called with non-list argument");
  //     return make<Integer>(-42); // control will never reach here
  //   }
  // }));
  //
  // define(env, "first", make<PrimitiveProcedure>([](Ref<List> args) {
  //   if (auto xs = match<List::Node>(list_after(0, args))) {
  //     return xs->first;
  //   } else {
  //     error("list::first argument error: must be non-empty list");
  //     Ref<Value> r = make<Integer>(-42);
  //     return r; // control will never reach here
  //   }
  // }));
  //
  // define(env, "rest", make<PrimitiveProcedure>([](Ref<List> args) {
  //   if (auto xs = match<List::Node>(list_after(0, args))) {
  //     return xs->rest;
  //   } else {
  //     error("list::rest argument error: must be non-empty list");
  //     Ref<List> r = nil();
  //     return r; // control will never reach here
  //   }
  // }));
  //
  // define(env, "ascii", make<PrimitiveProcedure>([](Ref<List> args) {
  //   if (auto code = match<Integer>(list_after(0, args))) {
  //     return make<Character>(char(code->value));
  //   }
  //
  //   error("ascii fn called on non-int arg");
  //   return make<Character>('\0');
  // }));
  //
  // define(env, "char", make<PrimitiveProcedure>([](Ref<List> args) {
  //   auto s = match<String>(list_after(0, args));
  //   char c = 'T';
  //
  //   if (s && s->value.size()) {
  //     c = s->value[0];
  //   } else {
  //     error("char must bbe called with non-empty string");
  //   }
  //
  //   return make<Character>(c);
  // }));
  //
  // define(env, "for-each", make<PrimitiveProcedure>([](Ref<List> args) {
  //   auto xs = match<List>(list_after(0, args));
  //   auto proc = match<Value>(list_after(1, args));
  //   vector<Ref<Value>> vals;
  //
  //   while (auto x = match<List::Node>(xs)){
  //     auto arg = cons(x->first, nil());
  //     vals.push_back(eval(cons(proc, arg), cons(nil())));
  //     xs = x->rest;
  //   }
  //
  //   return List::fromVec(vals);
  // }));
  //
  // define(env, "zip", make<PrimitiveProcedure>([](Ref<List> args) {
  //   auto a = match<List>(list_after(0, args));
  //   auto b = match<List>(list_after(1, args));
  //
  //   if (!(a && b)) {
  //     error("args to zip must be lists");
  //   }
  //
  //   return zip(a, b);
  // }));
  
  define(env, "list", make<PrimitiveProcedure>([](Ref<List> args) {
    return args;
  }));

  // define(env, "dict", make<PrimitiveProcedure>([](Ref<List> args) {
  //   auto xs = match<List>(list_after(0, args));
  //   auto fn = match<Value>(list_after(1, args));
  //   vector<Ref<Value>> vals;
  //
  //   while (auto x = match<List::Node>(xs)){
  //     auto arg = cons(x->first, nil());
  //     vals.push_back(apply(fn, arg));
  //     xs = x->rest;
  //   }
  //
  //   return List::fromVec(vals);
  // }));



  return env;
}
