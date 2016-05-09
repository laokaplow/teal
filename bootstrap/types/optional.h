#ifndef TYPES_OPTIONAL_H_
#define TYPES_OPTIONAL_H_

template <typename T>
struct Optional {
  struct Just; // "some"
  struct Nothing; // "none"

  virtual ~Optional() = 0;
};

template <typename T>
struct Optional<T>::Just {
  Ref<A> value;
};

template <typename T>
struct Optional<T>::Nothing {
};

#endif
