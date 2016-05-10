#ifndef PARSE_H_
#define PARSE_H_

#include "text.h"
#include "syntax.h"
#include "types.h"
#include "memory.h"

#include <string>

namespace Parse {

  struct Result {
    class Ok;
    class Error;

    Text::View leftover;

    Result(Text::View leftover) : leftover(leftover) {}

    virtual ~Result() = 0;
  };

  struct Result::Ok : public Result {
    Ref<Syntax> value;

    Ok(Ref<Value> value, const Text::View begin, const Text::View end)
      : Result(end), value(make<Syntax>(begin.to(end), value))
    {}
  };

  struct Result::Error : public Result {
    const std::string msg;

    Error(const std::string msg, const Text::View leftover)
      : Result(leftover), msg(msg)
    {}
  };

  Ref<Result> parse(Text::File &f);
}

#endif
