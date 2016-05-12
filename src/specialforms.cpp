#include "builtins.h"
#include "utilities.h"

void attach_special_forms(Ref<List::Node> env) {

  define(env, "quote", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    return args;
  }));

  define(env, "define", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    auto name = match<Atom>(list_after(0, args));
    auto expr = match<Value>(list_after(1, args));

    if (!(name && expr)) {
      error("Malformed call to define.");
    }

    define(env, name, eval(expr, env));

    return nil();
  }));

  define(env, "set!", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    auto name = match<Atom>(list_after(0, args));
    auto val = match<Value>(list_after(1, args));

    if (!(name && val)) {
      error("Malformed call to set!.");
    }

    env_set(env, name, eval(val, env));

    return nil();
  }));

  define(env, "if", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    auto condition  =  match<Bool>(eval(list_after(0, args), env));
    auto then_clause = match<Value>(list_after(1, args));
    auto else_clause = match<Value>(list_after(2, args));

    if (!(condition && then_clause && else_clause)) {
      error("Malformed if block");
    }

    return eval((condition->value? then_clause: else_clause), env);
  }));

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
}
