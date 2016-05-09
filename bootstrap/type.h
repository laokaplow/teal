#ifndef TYPE_H_
#define TYPE_H_


struct Type {
  TYPE();
  virtual ~TYPE() = 0;

  virtual P<Type> clone();
  virtual void show();
  virtual int read(const string &text, int &length_read);

};

#endif
