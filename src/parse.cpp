#include "parse.h"

#include "syntax.h"
#include "types.h"
#include "utilities.h"

#include <set>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std;

namespace { // readers
  Ref<Parse::Result> readAtom(Text::View v);
  Ref<Parse::Result> readNumber(Text::View v);
  Ref<Parse::Result> readString(Text::View v);
  Ref<Parse::Result> readList(Text::View v);
  Ref<Parse::Result> readExprs(Text::View v);
}

namespace { // utilities
  char pop(Text::View &);

  bool is_hex(char);
  bool is_digit(char);
  bool is_letter(char);
  bool is_whitespace(char);
  bool is_atom_special_char(char);
  bool is_valid_atom_char(char);
}

// exports
Parse::Result::~Result() {}
Ref<Parse::Result> Parse::parse(Text::File &f) {
  auto res = readExprs(f);

  // make sure we have parsed the whole file
  if (auto ok = match<Parse::Result::Ok>(res)) {
    if (!ok->leftover.is_empty()) {
      return make<Parse::Result::Error>("Expected end of file.", ok->leftover);
    }
  }

  // otherwise just pass up the result, good or bad
  return res;
}

namespace { // readers

Ref<Parse::Result> readAtom(Text::View v) {
  auto start = v;
  string value;

  // verify first char
  if (!v.is_empty() && is_valid_atom_char(v.peek()) && !is_digit(v.peek())) {
    value += pop(v);
  } else {
    auto msg = "Atoms must start with letter or valid special character.";
    return make<Parse::Result::Error>(msg, v);
  }

  // consume valid chars
  while (!v.is_empty() && is_valid_atom_char(v.peek())) {
      value += pop(v);
  }

  return make<Parse::Result::Ok>(make<Atom>(value), start, v);
}

Ref<Parse::Result> readNumber(Text::View v) {
  auto start = v;
  string digits;

  if (!v.is_empty() && is_digit(v.peek())) {
    digits += pop(v);
  } else {
    return make<Parse::Result::Error>("Numbers must start with digit.", v);
  }

  // read remaining integer portion
  while (!v.is_empty() && is_digit(v.peek())) {
      digits += pop(v);
  }

  // maybe its a float
  if (!v.is_empty() && v.peek() == '.') {
    digits += pop(v);

    // decimal must be followed by at least one digit
    if (!v.is_empty() && is_digit(v.peek())) {
      digits += pop(v);
    } else {
      auto msg = "Floats must have digit after decimal.";
      return make<Parse::Result::Error>(msg, v);;
    }

    // followed by any zero or more digits
    while (!v.is_empty() && is_digit(v.peek())) {
      digits += pop(v);
    }

    return make<Parse::Result::Ok>(make<Float>(stod(digits)), start, v);
  }

  return make<Parse::Result::Ok>(make<Integer>(stoi(digits)), start, v);
}

Ref<Parse::Result> readString(Text::View v) {
  // starts with quote
  // middle is zero or more regular characters or escapes
  // ends with matching quote

  auto start = v;
  string contents;
  char quote;

  // starts with ' or "
  if (!v.is_empty() && ((v.peek() == '\'') || (v.peek() == '"'))) {
    quote = pop(v);
  } else {
    return make<Parse::Result::Error>("Strings must start with a quote.", v);
  }

  while(!v.is_empty()) {
    char c = pop(v);

    if (c == quote) { // end of string
      return make<Parse::Result::Ok>(make<String>(contents), start, v);
    }

    if (c == '\\') { // escape sequence
      if (v.is_empty()) {
        string msg = "Unexpected <eof> in string escape sequence";
        return make<Parse::Result::Error>(msg, v);
      }

      c = v.peek();
      switch (c) {
        // literal escapes
        case '\\': // falthroguh
        case '\'': // fallthroughg
        case '\"': //
          contents += pop(v);
          break;

        // symbolic escapes
        case 'n':
          pop(v);
          contents += "\n";
          break;
        case 't':
          pop(v);
          contents += "\t";
          break;

        case 'x': {// hexidecimal
          pop(v);
          string hex_digits;
          while (hex_digits.size() < 2) {
            if (!v.is_empty() && is_hex(v.peek())) {
              hex_digits += pop(v);
            } else {
              auto msg = "Hex escape sequence must have two hex digits";
              return make<Parse::Result::Error>(msg, v);
            }
          }
          contents += (char)stoi(hex_digits);
          break;
        }

        default: // invalid escape sequence
          string msg = "Invalid escape sequence in string literal";
          return make<Parse::Result::Error>(msg, v);
          break;
      }
    }
  }

  auto msg = "Unexpected <end of file> in string literal";
  return make<Parse::Result::Error>(msg, v);
}

Ref<Parse::Result> readList(Text::View v) {
  auto begin = v;
  char opener;
  Ref<Syntax> contents;

  // starts with '(' or '['
  if (!v.is_empty() && ((v.peek() == '(') || (v.peek() == '[') || (v.peek() == '{'))) {
    opener = pop(v);
  } else {
    return make<Parse::Result::Error>("List must start with ( or [ or {", v);
  }

  auto r = readExprs(v);
  if (auto error = match<Parse::Result::Error>(r)) {
    return error;
  } else if (auto ok = match<Parse::Result::Ok>(r)) {
    v = ok->leftover;
    contents = ok->value;
  } else {
    throw std::runtime_error("Parse::Result match error");
  }

  char closer = v.peek();
  if (!v.is_empty() && (((opener == '(') && (closer == ')'))
    || ((opener == '[') && (closer == ']'))
    || ((opener == '{') && (closer == '}')))
  ) {
    pop(v);
  } else {
    auto msg = stringstream("Unmatched braces: ");
    msg << begin.head.offset << ", " << v.head.offset  << ".";
    return make<Parse::Result::Error>(msg.str(), v);
  }

  return make<Parse::Result::Ok>(contents, begin, v);
}

Ref<Parse::Result> readExprs(Text::View v) {
  auto start = v;
  auto nodes = make<List>();

  while (!v.is_empty()) {
    // ignore whitespace
    if (is_whitespace(v.peek())) {
      pop(v);
      continue;
    }
    // ignore comments
    if (v.peek() == ';') { // line comment
      pop(v);
      // advance to the end of the line
      while (!v.is_empty() && (v.peek() != '\n')) {
        pop(v);
      }
      continue;
    }

    // try to read something
    bool progress = false;
    for (auto read : {readAtom, readNumber, readString, readList}) {
      auto r = read(v);
      if (auto ok = match<Parse::Result::Ok>(r)) {
        nodes = cons(ok->value, nodes);
        v = ok->leftover;
        progress = true;
        break;
      } else if (auto error = match<Parse::Result::Error>(r)) {
        if (v == error->leftover) { // didn't get anywhere
          continue;
        } else {
          return error;
        }
      } else {
        throw std::runtime_error("Parse::Result match error");
      }
    }

    if (!progress) {
      break;
    }
  }

  return make<Parse::Result::Ok>(reverse(nodes), start, v);
}

}

namespace { // utilities

char pop(Text::View& v) {
  char c = v.peek();
  auto n = v.tail();
  v.become(n);
  return c;
}


bool is_hex(char c) {
  return is_digit(c) || (c >= 'A' && c <= 'F');
}

bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

bool is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_whitespace(char c) {
  static const set<char> whitespace = {' ', '\t', '\n'};
  return whitespace.count(c);
}

bool is_atom_special_char(char c) {
  static const set<char> special = {
    '+', '-', '/', '*', '=', '&', '|', '>', '<', '%', '^', '_',
    '!', '?', ':', '.', '@', '#', '$', '~', '`', ','
  };
  return special.count(c);
}

bool is_valid_atom_char(char c) {
  return is_digit(c) || is_letter(c) || is_atom_special_char(c);
}

};
