#ifndef VALUE_H_
#define VALUE_H_

// The interface for a runtime value
struct Value {
  virtual ~Value() = 0;
  // P<Value> clone();
  // string show();
  // bool compare();
  // P<Type> type();
};


#endif
