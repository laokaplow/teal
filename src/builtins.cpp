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
  define(env, "nil", nil());

  // functions
  define(env, "display", make<PrimitiveProcedure>([](Ref<List> args) {
    while (auto xs = match<List::Node>(args)) {
      cout << xs->first->show();
      args = xs->rest;
    }
    cout.flush();
    return nil();
  }));

  define(env, "empty?", make<PrimitiveProcedure>([](Ref<List> args) {
    if (auto vxs = match<Vector>(list_after(0, args))) {
      return make<Bool>(0 == vxs->contents.size());
    }

    return make<Bool>(!!match<List::Empty>(list_after(0, args)));
  }));

  define(env, "atom?", make<PrimitiveProcedure>([](Ref<List> args) {
    return make<Bool>(!!match<Atom>(list_after(0, args)));
  }));

  define(env, "=", make<PrimitiveProcedure>([](Ref<List> args) {
    auto a = list_after(0, args);
    auto b = list_after(1, args);

    return make<Bool>(eq(a, b));
  }));

  define(env, "read-lines", make<PrimitiveProcedure>([](Ref<List> args) {
    string buf;
    vector<Ref<Value>> lines;
    while (cin) {
      getline(cin, buf, '\n');
      if (!cin && buf.size() == 0) continue;
      vector<Ref<Value>> cs;
      for (char &c: buf) {
        cs.push_back(make<Character>(c));
      }
      lines.push_back(List::fromVec(cs));
    }
    auto res = List::fromVec(lines);
    // DEBUG(res->show());
    return res;
  }));

  define(env, "exit", make<PrimitiveProcedure>([](Ref<List> args) {
    int code = 0;
    if (auto user_code = match<Integer>(list_after(0, args))) {
      code = user_code->value;
    }
    exit(code);

    return nil(); // control flow never reaches here
  }));

  define(env, "len", make<PrimitiveProcedure>([](Ref<List> args) {
    if (auto vxs = match<Vector>(list_after(0, args))) {
      return make<Integer>(vxs->contents.size());
    }

    auto num = len(match<List>(list_after(0, args)));
    if (num == -1) {
      error("invalid argument to len - must be list or string or vector");
    }
    return make<Integer>(num);
  }));

  define(env, "first", make<PrimitiveProcedure>([](Ref<List> args) {
    if (auto xs = match<List::Node>(list_after(0, args))) {
      return xs->first;
    } else {
      error("list::first argument error: must be non-empty list");
      Ref<Value> r = make<Integer>(-42);
      return r; // control will never reach here
    }
  }));

  define(env, "rest", make<PrimitiveProcedure>([](Ref<List> args) {
    if (auto xs = match<List::Node>(list_after(0, args))) {
      return xs->rest;
    } else {
      error("list::rest argument error: must be non-empty list");
      Ref<List> r = nil();
      return r; // control will never reach here
    }
  }));

  define(env, "ascii", make<PrimitiveProcedure>([](Ref<List> args) {
    if (auto code = match<Integer>(list_after(0, args))) {
      return make<Character>(char(code->value));
    }

    error("ascii fn called on non-int arg");
    return make<Character>('\0');
  }));

  define(env, "char", make<PrimitiveProcedure>([](Ref<List> args) {
    auto s = match<String>(list_after(0, args));
    char c = 'T';

    if (s && s->value.size()) {
      c = s->value[0];
    } else {
      error("char must bbe called with non-empty string");
    }

    return make<Character>(c);
  }));

  define(env, "for-each", make<PrimitiveProcedure>([](Ref<List> args) {
    auto xs = match<List>(list_after(0, args));
    auto proc = match<Lambda>(list_after(1, args));
    vector<Ref<Value>> vals;

    if (auto vxs = match<Vector>(list_after(0, args))) {
      if (proc) {
        for (Ref<Value> item: vxs->contents) {
          auto argpack = cons(item, nil());
          auto res = proc->apply(argpack);
          vals.push_back(res);
        }
        return List::fromVec(vals);
      }
    }

    if (!(xs && proc)) {
      error("malformed for-each block");
    }

    while (auto x = match<List::Node>(xs)){
      auto arg = cons(x->first, nil());
      auto val = proc->apply(arg);
      vals.push_back(val);
      xs = x->rest;
    }

    return List::fromVec(vals);
  }));

  define(env, "zip", make<PrimitiveProcedure>([](Ref<List> args) {
    auto a = match<List>(list_after(0, args));
    auto b = match<List>(list_after(1, args));

    if (!(a && b)) {
      error("args to zip must be lists");
    }

    return zip(a, b);
  }));

  define(env, "list", make<PrimitiveProcedure>([](Ref<List> args) {
    return args;
  }));

  define(env, "-", make<PrimitiveProcedure>([](Ref<List> args) {
    auto a = match<Integer>(list_after(0, args));
    auto b = match<Integer>(list_after(1, args));

    if (!(a && b)) {
      error("subtraction `-` is only defined on ints.");
    }

    return make<Integer>(a->value - b->value);
  }));

  define(env, "+", make<PrimitiveProcedure>([](Ref<List> args) {
    auto a = match<Integer>(list_after(0, args));
    auto b = match<Integer>(list_after(1, args));

    if (!(a && b)) {
      error("addition `+` is only defined on ints.");
    }

    return make<Integer>(a->value + b->value);
  }));

  define(env, "<", make<PrimitiveProcedure>([](Ref<List> args) {
    auto a = match<Integer>(list_after(0, args));
    auto b = match<Integer>(list_after(1, args));

    if (!(a && b)) {
      error("lesser `<` is only defined on ints.");
    }

    return make<Bool>(a->value < b->value);
  }));

  define(env, "<=", make<PrimitiveProcedure>([](Ref<List> args) {
    auto a = match<Integer>(list_after(0, args));
    auto b = match<Integer>(list_after(1, args));

    if (!(a && b)) {
      error("lesser-or-equal `<=` is only defined on ints.");
    }

    return make<Bool>(a->value <= b->value);
  }));

  define(env, "nth", make<PrimitiveProcedure>([](Ref<List> args) {
    auto xs = match<List>(list_after(0, args));
    auto i = match<Integer>(list_after(1, args));

    if (!i) {
      error("nth is only defined for lists and vectors");
    }

    if (auto vxs = match<Vector>(list_after(0, args))) {
      int d = i->value;
      if ((d < 0) || (d >= (int)vxs->contents.size())) {
        error("index out of range for vector i=" + i->show());
      }

      return vxs->contents[i->value];
    }

    if (!(xs && i)) {
      error("nth is only defined for integer indexes on lists");
    }

    if (-1 == len(xs)) {
      error("index out of range: " + to_string(i->value) + " in " + xs->show());
    }

    return list_after(i->value, xs);
  }));

  define(env, "range", make<PrimitiveProcedure>([](Ref<List> args) {
    auto low  = match<Integer>(list_after(0, args));
    auto high = match<Integer>(list_after(1, args));

    if (!(low && high) || (low->value > high->value)) {
      error("range takes two ints, the first may not be larger then the second");
    }

    vector<Ref<Value>> xs;
    for (int i = low->value; i < high->value; i++) {
      xs.push_back(make<Integer>(i));
    }

    return List::fromVec(xs);
  }));

  define(env, "begin", make<PrimitiveProcedure>([](Ref<List> args) {
    // args are already evaluated in order
    // return last value
    return list_after(len(args)-1, args);
  }));

  define(env, "get-member", make<PrimitiveProcedure>([](Ref<List> args) {
    // DEBUG(args->show())
    auto xs = match<List::Node>(list_after(0, args));
    auto name = match<Atom>(list_after(1, args));

    if (!(xs && name)) {
      error("get-member argument error");
    }

    if (auto kv = match<List::Node>(alist::lookup(name, xs))) {
      return list_after(1, kv);
    } else {
      error("key `" + name->value + "` not found int " + xs->show());
      return list_after(0, args); // control will never get here
    }
  }));

  define(env, "set-member!", make<PrimitiveProcedure>([](Ref<List> args) {
    auto xs = match<List::Node>(list_after(0, args));
    auto name = match<Atom>(list_after(1, args));
    auto val = match<Value>(list_after(2, args));

    if (!(xs && name && val)) {
      error("set-member! argument error");
    }

    if (auto kv = match<List::Node>(alist::lookup(name, xs))) {
       kv->rest = cons(val, nil());
    } else {
      error("key `" + name->value + "` not found int " + xs->show());
    }

    return nil();
  }));


  define(env, "and", make<PrimitiveProcedure>([](Ref<List> args) {
    // DEBUG(args->show())

    if (auto none = match<List::Empty>(args)) {
      error("and takes at least one argument");
    }

    while (auto bs = match<List::Node>(args)) {
      if (auto cond = match<Bool>(bs->first)) {
        if (!cond->value) return make<Bool>(false);
      } else {
        error("all arguments to `and` must be booleans");
      }
      args = bs->rest;
    }

    return make<Bool>(true);
  }));

  define(env, "not", make<PrimitiveProcedure>([](Ref<List> args) {
    if (auto b = match<Bool>(list_after(0, args))) {
       return make<Bool>(!b->value);
    } else {
      error("argument to `not` must be boolean");
    }

    return make<Bool>(false);
  }));

  define(env, "cons", make<PrimitiveProcedure>([](Ref<List> args) {
    auto car = match<Value>(list_after(0, args));
    auto cdr = match<Value>(list_after(1, args));

    if (!(car && cdr)) {
      error("cons takes two arguments");
    }

    return cons(car, cons(cdr, nil()));
  }));

  define(env, "vectorize", make<PrimitiveProcedure>([](Ref<List> args) {
    auto ls = list_after(0, args);
    auto items = match<List>(ls);

    if (!items) {
      DEBUG(ls->show());
      error("error - can only vectorize lists");
    }

    return make<Vector>(items);
  }));

  define(env, "vector-append!", make<PrimitiveProcedure>([](Ref<List> args) {
    auto vxs = match<Vector>(list_after(0, args));
    auto item = match<Value>(list_after(1, args));

    if (!(vxs && item)) {
      error("malformed call to vector-append!");
    }

    vxs->append(item);

    return nil();
  }));

  define(env, "vector-pop!", make<PrimitiveProcedure>([](Ref<List> args) {
    auto xs = list_after(0, args);
    Ref<Value> retval = nil();
    if (auto vxs = match<Vector>(xs)) {
      if (vxs->contents.size()) {
        retval = vxs->contents.back();
        vxs->contents.pop_back();
        return retval;
      } {
        error("Vector-pop! must be called with non-empty vector");
      }
    }
    DEBUG(xs->show());
    error("vector-pop! must be called with a vector");
    return retval;
  }));

  return env;
}
