#ifndef RUNTIME_H_
#define RUNTIME_H_

#include <memory>
#include <stdexcept>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> match(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename U>
Ref<T> match(Ref<U> r) {
  return std::dynamic_pointer_cast<T>(r);
}

#define RAISE(msg) { throw std::logic_error(msg); }


#endif
