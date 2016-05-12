#include "types.h"
#include "utilities.h"
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

Ref<Value> Lambda::magic(Ref<List> args) {
  DEBUG("\n\nIn lambda apply")
  DEBUG("\nargs = " << args->show());
  DEBUG("\nparams = " << params->show());
  auto frame = zip(params, args);
  DEBUG("\nFrame = " << frame->show());
  DEBUG("\nlexical env = " << env->show());
  auto app_env = env_add_frame(env, frame);
  DEBUG("applicaion env = " << app_env->show());
  DEBUG("\nbody = " << body->show());
  DEBUG("\n\n")

  auto t = list_after(1, env_lookup(app_env, make<Atom>("pi")));
  DEBUG(t->show())








  return make<String>("lambda return value");
}
