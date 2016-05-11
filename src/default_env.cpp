#include "utilities.h"
#include "types.h"

#include <string>
using namespace std;



void define(Ref<List::Node> env, Ref<Atom> name, Ref<Value> value) {
  if (auto frame = match<List>(env->first)) {
    frame = cons(mk_pair(name, value), frame);
  } else {
    error("problem with environment - top frame is not a list");
  }
}
void define(Ref<List::Node> env, string name, Ref<Value> value) {
  return define(env, make<Atom>(name), value);
}


Ref<List::Node> default_env() {
  auto env = make<List::Node>(make<List::Empty>());

  define(env, "define", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    auto name = match<Atom>(list_after(0, args));
    auto expr = match<Value>(list_after(1, args));

    if (!(name && expr)) {
      error("Malformed call to define.");
    }

    define(env, name, eval(expr, env));

    return nil();
  }));
  //
  // SPECIAL_FORM("set!", {
  //   auto name = match<Atom>(pop(quoted_args));
  //   auto val = match<Value>(pop(quoted_args));
  //   if (!(name && val) {
  //     error("Malformed call to set!.");
  //   }
  //
  //   // find pair where key = name
  //   // update val
  //   error("Not impliemented error: set!.");
  //
  //   return nil()
  // });
  //
  // SPECIAL_FORM("set!", {
  //   auto name = match<Atom>(pop(quoted_args));
  //   auto val = match<Value>(pop(quoted_args));
  //   if (!(name && val) {
  //     error("Malformed call to set!.");
  //   }
  //   // find pair where key = name
  //   // update val
  // });
  //
  // SPECIAL_FORM("if", {
  //   auto condition = match<Bool>(eval(pop(args), env);
  //   auto then_clause = pop(args);
  //   auto else_clause = pop(args);
  //
  //   return eval((condition.value? then_clause: else_clause), env;
  // });
  //
  // SPECIAL_FORM("lambda", {
  //   auto parameters = pop(args);
  //   auto body = pop(args);
  //
  //   return make<Procedure::compound>(parameters, body, env);
  // });
  //
  // SPECIAL_FORM("for-each", {
  //   // do each in turn and return the last
  // });
  //
  // // see ref prog for othere special forms
  //
  // // deifne built-in functions....
  //
  // FUNC("begin", {
  //   // do each in turn and return the last
  // });
  //


  return env;
}
