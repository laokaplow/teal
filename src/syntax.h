#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "memory.h"
#include "text.h"
#include "types.h"

struct Syntax : Value {
  Text::Location where;
  Ref<Value> data;

  Syntax(Text::Location where, Ref<Value> data)
    : where(where), data(data)
  {}

  std::string show() const {
    // return std::string("<Syntax: ") + data->show() + ">"; };
    return data->show(); };
};

#endif
