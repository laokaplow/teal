#ifndef MEMORY_H_
#define MEMORY_H_


template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename U>
Ref<T> match(Ref<U> r) {
  return dynamic_pointer_cast<T>(r);
}

#endif
