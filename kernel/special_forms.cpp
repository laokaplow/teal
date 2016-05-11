
void define_special_form(env, name, impl) {
  add name, impl to top frame of env
  consider doing some error checking
}

void define_special_forms(Ref<List> env) {
  define_special_form(env, "quote", [](Ref<List> args, Ref<List> env) {
    // return arguments unevaluated
    return args;
  });

  define_special_form(env, "set!", [=](Ref<List> args, Ref<List> env) {
    auto name = pop(args);
    auto val = pop(args);

    // push pair onto top frame of env
    ...

    return make<List::Empty>();
  });

  define_special_form(env, "if", [](Ref<List> args, Ref<List> env) {
    auto condition = match<Bool>(eval(pop(args), env);
    auto then_clause = pop(args);
    auto else_clause = pop(args);

    return eval((condition.value? then_clause: else_clause), env;
  });

  define_special_form(env, "lambda", [](Ref<List> args, Ref<List> env) {
    auto parameters = pop(args);
    auto body = pop(args);

    return make<Procedure::compound>(parameters, body, env);
  });

  define_special_form(env, "begin") [](Ref<List> args, Ref<List> env) {
    // do each in turn and return the last
  });

  // (define let)
  define_special_form(env, "let", [](Ref<List> args, Ref<List> env) {
    // rewrite into lambda, then call with args
  });
}
