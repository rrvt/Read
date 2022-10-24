// Record Base Class


#pragma once


enum RecordTyp {AuthorRcdTyp = 101, BookRcdTyp, NoRcdTypes};
typedef int RecordID;


class Record {
public:

RecordID id;

  Record() : id(0) {}
 ~Record() {id = 0;}

  void    add(int& nextIndex) {id = nextIndex++;}
  void    rcdCopy(Record& r) {id = r.id;}
  };

