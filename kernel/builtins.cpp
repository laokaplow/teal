// special forms

#include "runtime.h"
#include "utilities.h"
#include "special_forms.h"
#include "types.h"

Ref<List> default_env() {
  Ref<List> env = make<List::Empty>();
  define(env, "", );

  // special forms
  define_special_forms(env);


  // procedures
  defproc()
}
