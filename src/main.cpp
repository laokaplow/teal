#include "parse.h"
#include "utilities.h"
#include "memory.h"
#include "text.h"

#include <fstream>
#include <iostream>

using namespace std;

Ref<Value> interpret_file(string filename) {
  auto env = default_env();

  auto file = Text::File::open(filename);
  cout << filename << "\n" << file->contents << "\n";

  auto res = Parse::parse(file);
  if (auto ok = match<Parse::Result::Ok>(res)) {
    cout << "parse ok!\n";
    cout << ok->value->show() << "\n";
    try {
      cout << "before eval\n";
      eval_each(match<List>(ok->value), default_env());
      cout << "after eval\n";
      return 0;
    } catch (std::runtime_error &e) {
      cout << "Runtime Error: " << e.what() << "\n";
    }
  } else if (auto err = match<Parse::Result::Error>(res)) {
    cout << "Parse Error: " << err->msg << "\n";
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
