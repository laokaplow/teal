#include "builtins.h"
#include "utilities.h"
#include "types.h"

#include <iostream>
#include <string>
using namespace std;

Ref<List::Node> default_env() {
  auto env = make<List::Node>(make<List::Empty>());

  attach_special_forms(env);
  attach_builtin_fns(env);

  // constants
  define(env, "pi", make<Float>(3.1415826));

  // functions
  define(env, "display", make<PrimitiveProcedure>([](Ref<List> args) {
    while (auto xs = match<List::Node>(args)) {
      cout << xs->first->show();
      args = xs->rest;
    }

    return nil();
  }));

  define(env, "empty?", make<PrimitiveProcedure>([](Ref<List> args) {
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

  return env;
}
