// special forms

#include "runtime.h"
#include "utilities.h"
#include "special_forms.h"
#include "types.h"

Ref<List> default_env() {
  Ref<List> env = make<List::Empty>();
  define(env, "", );

  // special forms
  define_special_form(env, "quote", [](Ref<List> args, Ref<List> env) {

  });
  define_special_form(env, "set!", [](Ref<List> args, Ref<List> env) {

  });
  define_special_form(env, "if", [](Ref<List> args, Ref<List> env) {

  });
  define_special_form(env, "lambda", [](Ref<List> args, Ref<List> env) {

  });
  define_special_form(env, "begin") [](Ref<List> args, Ref<List> env) {

  });
  define_special_form(env, "let", [](Ref<List> args, Ref<List> env) {

  });


  // procedures
  defproc()
}
