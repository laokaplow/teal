#ifndef VALUE_H_
#define VALUE_H_


struct Value {
  virtual ~Value() = 0;
  P<Value> clone();
  string show();
  bool compare();
  P<Type> type();
};

#endif
