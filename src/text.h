#ifndef TEXT_H_
#define TEXT_H_

#include "types.h"

#include <string>
#include <vector>
#include <ostream>
#include <fstream>

namespace Text {

  struct Position {
    int offset = 0; // could be line + collumn in future

    Position step() const {
      return { offset + 1 };
    }

    bool operator==(const Position &other) const {
      return offset == other.offset;
    }
  };

  struct Location {
    const Position begin = {0}; // index to start
    const Position end = {0}; // one past index to end
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
    std::string contents;
    std::string name;
    bool operator==(const File &other) const {
      return (name == other.name) && (contents == other.contents);
    }

    File(std::string contents, std::string name)
      : contents(contents), name(name)
    {}

    static Ref<File> open(std::string filename) {
      auto ifs = std::ifstream(filename);
      return make<File>(
        std::string(
          (std::istreambuf_iterator<char>(ifs)),
          std::istreambuf_iterator<char>()
        ),
        filename
      );
    }
  };

  struct View {
    File &file;
    Position head;

    View(File &file, Position head = {})
      : file(file), head(head)
    {}

    View &operator=(View &other) {
      become(other);

      return *this;
    }

    void become(View &other) {
      head = other.head;
      file = other.file;
    }

    int size() const {
      return file.contents.size() - head.offset;
    }

    bool is_empty() const {
      return size() == 0;
    }

    char peek() const {
      return file.contents[head.offset];
    }

    View tail() const {
      return View(file,  is_empty()? head : head.step());
    }

    Location to(const View& end) const {
      auto dest = end.head;
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
