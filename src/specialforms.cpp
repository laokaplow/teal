#include "builtins.h"
#include "utilities.h"
#include "debug.h"

#include <vector>

using namespace std;

void attach_special_forms(Ref<List::Node> env) {

  define(env, "quote", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    if (auto xs = match<List::Node>(args)) {
      if (auto more = match<List::Empty>(xs->rest)) {
        return xs->first;
      }
    }
    error("quote takes exactly one argument");
    Ref<Value> fake_val;
    return fake_val; // control never reaches here
  }));

  define(env, "env", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    return env;
  }));

  define(env, "define", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    auto name = match<Atom>(list_after(0, args));
    auto expr = match<Value>(list_after(1, args));

    if (!(name && expr)) {
      error("Malformed call to define.");
    }

    auto init =  eval(expr, env);

    // DEBUG(init->show());

    define(env, name, init);

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

  define(env, "lambda", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    // DEBUG("wwwwwwwwwwwweeeeeeeeeeeeeeee");
    auto params  = match<List>(list_after(0, args));
    auto body = match<Value>(list_after(1, args));

    if (!(params && body)) {
      error("Malformed lambda expression");
    }

    return make<Lambda>(env, params, body);

    // auto proc = cons(env, cons(params, cons(body, nil())));
    // DEBUG("Creating proc = " << proc->show());
    // DEBUG("env = " << env->show());
    // DEBUG("params = " << params->show());
    // DEBUG("body = " << body->show());
    // return proc;

  }));

  define(env, "dict", make<SpecialForm>([](Ref<List> args, Ref<List::Node> env) {
    auto kv_pairs = match<List>(list_after(0, args));
    // DEBUG(kv_pairs->show());
    vector<Ref<Value>> ps;
    while (auto head = match<List::Node>(kv_pairs)) {
      auto kv = match<List::Node>(head->first);
      // DEBUG(kv->show())
      // eval k, v seperately
      auto k = match<Atom>(eval(list_after(0, kv), env));
      auto v = match<Value>(eval(list_after(1, kv), env));

      if (!(kv && k && v)) {
        cout << k << " " << v;
        error("malformed dictionary entry: " + head->first->show());
      }
      auto entry = mk_pair(k, v);
      // DEBUG(entry->show());
      ps.push_back(entry);
      kv_pairs = head->rest;
    }

    auto computed = List::fromVec(ps);
    // DEBUG(computed->show())
    return computed;
  }));

}
