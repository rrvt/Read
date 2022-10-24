// Book Descriptions


#pragma once
#include "Archive.h"
#include "Expandable.h"
#include "Persons.h"
#include "Record.h"



class Book : public Record {
public:

String title;
int    authorID;
int    author2ID;
int    protagID;
int    protag2ID;
String location;
String pubDate;
String comment;

  Book() : protagID(0), protag2ID(0), authorID(0), author2ID(0), pubDate() {}
 ~Book() {clear();}

  void clear();

  Book(Book& b) {copy(b);}
  Book& operator= (Book& b) {copy(b); return *this;}

  int  load(Archive& ar);
  void store(Archive& ar);

  void display(int personID, bool subOrd = true);

  void verifyRefs();
  bool verifyRef(PerTyp perTyp, int personID);

  void removeRef(int personID);
  void removePersonRefs();

private:

  void copy(Book& b);
  };


class Books {
int                 x;
int                 nextIndex;
Expandable<Book, 1> rcds;

public:

  Books() : nextIndex(1) {}

  void clear();

  Book& add( TCchar* title);
  Book* find(TCchar* title);
  Book* find(int bookID);

  int   number() {return noRcds();}

  void  display(CString tgt);

  void load(Archive& ar);
  void store(Archive& ar);

  void del(Book& book);
  void removeRef(int bookID, int personID);

  Book* startLoop() {x = -1; return nextNode();}
  Book* nextNode()  {return ++x < noRcds() ? &rcds[x] : 0;}

private:

  int noRcds() {return rcds.end();}
  };


extern Books books;
