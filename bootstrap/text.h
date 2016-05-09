#ifndef TEXT_H_
#define TEXT_H_

#include <string>
#include <vector>
#include <ostream>

namespace Text {

  struct Position {
    const int offset = 0; // could be line + collumn in future

    Position step() const {
      return { offset + 1 };
    }

    bool operator==(const Position &other) const {
      return offset == other.offset;
    }
  };

  struct Location {
    const Position begin = 0; // index to start
    const Position end = 0; // one past index to end
    const std::string filename = "";

    Location(const Position begin, const Position end, const std::string filename)
      : begin(begin), end(end), filename(filename)
    {}

    bool operator==(const Location& other) const {
      return ((begin == other.begin)
        && (end == other.end)
        && (filename == other.filename)
      );
    }
  };


  struct File {
    const std::vector<char> contents;
    const std::string name;
    bool operator==(const File &other) {
      return (name == other.name) && (contents == other.contents);
    }
  };

  struct View {
    const File &file;
    const Position head;

    View(File &file, Position start = {})
      : file(file), head(head)
    {}

    int size() const {
      return file.contents.size() - head.offset;
    }

    bool is_empty() const {
      return size() == 0;
    }

    char peek() const {
      return file.contents[head.offest];
    }

    View tail() const {
      return View(file,  is_empty()? head : head.step());
    }

    Location to(Position dest) {
      return Location(head, dest, file.name);
    }

    operator Location() const {
      return Location(head, head, file.name);
    }

    bool operator==(const View &other) const {
      return (file == other.file) && (head == other.head);
    }
  };

};


#endif
