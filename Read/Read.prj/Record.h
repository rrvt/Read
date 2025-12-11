// Record Base Class


#pragma once


enum RecordTyp  {AuthorRcdTyp = 101, BookRcdTyp, NoRcdTypes};

enum DspOptions {NilDsp, TitleDsp, DateDsp, AuthDsp, PersonDsp, AllPerDsp, DebugDsp};


class Record {
public:

String key;
int    id{};

  Record() {}
 ~Record() {id = 0;}

  void    set(int& nextIndex) {id = nextIndex++;}
  void    copy(Record& r) {id = r.id;}
  };

