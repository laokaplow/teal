#include "parse.h"
#include "utilities.h"
#include "memory.h"
#include "text.h"

#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <file_to_interpret>\n";
    return -1;
  }

  cout << Text::File::open(argv[1])->contents << endl;
  // auto res = parse(File::open(argv[1]));
  // if (auto err = match<Parse::Result::Ok>(res)) {
  //   try {
  //     eval_list(res.value, default_env());
  //     return 0;
  //   } catch(...) {
  //     cout << "Runtime Error.\n";
  //   }
  // } else if (auto err = match<Parse::Result::Error>(res)) {
  //   cout << "Parse Error\n";
  // } else {
  //   cout << "Uknown Error\n";
  //   return -1;
  // }
  return 0;
}
