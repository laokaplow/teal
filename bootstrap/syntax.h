#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "text.h"

struct Syntax {
    Text::Location where;
    Ref<Value> data;

  Syntax(Text::Location where, Ref<Value> data)
    : where(where), data(data)
  {}
};

#endif
