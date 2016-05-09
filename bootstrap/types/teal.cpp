#include <iostream>
#include <string>
#include <stdexcept>

Ref<value> readline() {
  if (!cin) {
    throw runtime_error("Cannot read line. STDIN is closed.");
  }
  string s;
  cin.getline(cin, s);
  return make<String>(s);
}

int main() {
  while ()

  return 0;
}
