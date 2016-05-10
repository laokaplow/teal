#ifndef MEMORY_H_
#define MEMORY_H_

#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> make(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, typename U>
Ref<T> match(Ref<U> r) {
  return std::dynamic_pointer_cast<T>(r);
}



#endif
