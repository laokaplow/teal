#ifndef BUILTINS_H_
#define BUILTINS_H_

#include "memory.h"
#include "types.h"

Ref<List::Node> default_env();

void attach_special_forms(Ref<List::Node> env);
void attach_builtin_fns(Ref<List::Node> env);

#endif
