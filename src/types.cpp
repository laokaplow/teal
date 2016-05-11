#include "types.h"
#include <algorithm>

using namespace std;

Value::~Value() {}

Ref<List> List::fromVec(std::vector<Ref<Value>> v) {
  Ref<List> list = make<List::Empty>();
  reverse(v.begin(), v.end());
  for (auto i : v) {
    list = make<List::Node>(i, list);
  }
  return list;
}
