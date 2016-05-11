#include "parse.h"
#include "utilities.h"
#include "memory.h"
#include "text.h"

#include <fstream>
#include <iostream>

using namespace std;

Ref<Value> interpret_file(string filename) {
  auto env = default_env();
  auto res = Parse::parse(Text::File::open(string(argv[1])));
  if (auto ok = match<Parse::Result::Ok>(res)) {
      eval_list(ok->value, default_env());
      return 0;
    } catch(...) {
      cout << "Runtime Error.\n";
    }
  } else if (auto err = match<Parse::Result::Error>(res)) {
    throw "Parse Error\n";
  }
  return env;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <file_to_interpret>\n";
    return -1;
  }

  try {
    interpret_file(argv[1]);
    return 0;
  } catch(exception &e) {
    cout << e.what();
    return -1;
  }

  return 0;
}
