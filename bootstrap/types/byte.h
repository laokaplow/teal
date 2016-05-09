#ifndef TYPES_CHAR_H_
#define TYPES_CHAR_H_

bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

bool is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_special_char(char c) {
  static const set<char> special = {
    '+', '-', '/', '*', '=', '&', '|', '>', '<', '%', '^', '_'
    '!', '?', ':', '.', '@', '#', '$', '~', '\`', ','
  };
  return is_letter(c) || is_digit(c) || special.count(c);
}

bool is_whitespace(char c) {
  static const set<char> whitespace = {' ', '\t', '\n'};
  return whitespace.count(c);
}

bool is_comment(char c) { return c == ';'; }


#endif
