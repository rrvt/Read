// List of Persons

#pragma once
#include "Archive.h"
#include "Expandable.h"
#include "IterT.h"
#include "Record.h"



enum PerTypPos {NilTypPos, Author, Author2, Protag, Protag2, BaseTyp};
enum PerTyp    {AuthorTyp = BaseTyp, ProtagTyp};


class BookKey {
public:
String title;
int    bookID;

  BookKey() : bookID(0) {title.clear();}
 ~BookKey()             {clear();}

  void clear() {title.clear(); bookID = 0;}

  BookKey(BookKey& b) {copy(b);}
  BookKey& operator= (BookKey& b) {copy(b); return *this;}

 //  *p == *q
 //  *p >  *q
 //  *p <= *q

  bool    operator== (BookKey& b) {return title == b.title;}
  bool    operator>  (BookKey& b) {return title >  b.title ;}
  bool    operator<= (BookKey& b) {return title <  b.title;}

private:

  void copy(BookKey& b) {title = b.title; bookID = b.bookID;}
  };


class BookKeys;
typedef IterT<BookKeys, BookKey> BksIter;


class BookKeys {
Expandable<BookKey, 1> rcds;

public:

  BookKeys() {}
  BookKeys(BookKeys& bk) {copy(bk);}
 ~BookKeys() {clear();}

  BookKeys& operator= (BookKeys& bk) {copy(bk); return *this;}

  void clear();

  BookKey* find(  int bookID, String& title);
  void     addRef(int bookID, String& title);
  void     delRef(int bookID);
  bool     isEmpty() {return !noRcds();}

  void     sort();

  void     updateRefs();

  void     removeRefs(   int personID);
  void     removeBookRef(int bookID);

  bool     removeDuplicates();
  bool     verifyRefs(RecordID personID);

  void     load(Archive& ar);
  void     store(Archive& ar);

  void     display(int index);

private:

  int  noRcds() {return rcds.end();}
  void copy(BookKeys& bk) {rcds = bk.rcds;}

  // returns either a pointer to data (or datum) at index i in array or zero

  BookKey* datum(int i) {return 0 <= i && i < nData() ? &rcds[i] : 0;}       // or data[i].p

  int   nData()      {return rcds.end();}                 // returns number of data items in array

  friend typename BksIter;
  };


class Person : public Record {
public:

Byte     perTyp;
String   fName;
String   lName;
BookKeys bookKeys;

  Person() : perTyp(NilTypPos) {}
 ~Person() {clear();}

  void clear();

  Person(Person& a) {copy(a);}

  Person& operator= (Person& a) {copy(a); return *this;}

  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  bool    operator== (Person& a);
  bool    operator>  (Person& a);
  bool    operator<= (Person& a);

  int      load( Archive& ar);
  void     store(Archive& ar);

  void     sort();

  void     updateRefs()              {bookKeys.updateRefs();}

  void     removeRefs()              {bookKeys.removeRefs(id);}
  void     removeBookRef(int bookID) {bookKeys.removeBookRef(bookID);}

  bool     removeDuplicates()        {return bookKeys.removeDuplicates();}
  bool     verifyRefs()              {return bookKeys.verifyRefs(id);}

  void     display();
  void     dspName();

private:

  void copy(Person& a);
  };


class Persons;
typedef IterT<Persons, Person> PersIter;


class Persons {
//int                   x;
int                   nextIndex;
Expandable<Person, 1> rcds;

public:

  Persons() : nextIndex(1) {}

  void     clear();

  Person*  add( PerTyp perTyp, TCchar* fName, TCchar* lName);
  Person*  find(PerTyp perTyp, TCchar* fName, TCchar* lName);
  Person*  find(PerTyp perTyp, int id);

  int      number() {return noRcds();}

  void     sort();

  void     del(Person& person);
  void     removeBookRef(int personID, int bookID);

  void     expunge();

  void     load(Archive& ar);
  void     store(Archive& ar);

  void     display();
  bool     display(int id);
  void     display(PerTyp typ, CString tgt);

//  Person*  startLoop() {x = -1; return nextNode();}
//  Person*  nextNode()  {return ++x < noRcds() ? &rcds[x] : 0;}

//  void examinePersons();

private:

  int noRcds() {return rcds.end();}

  // returns either a pointer to data (or datum) at index i in array or zero

  Person* datum(int i) {return 0 <= i && i < nData() ? &rcds[i] : 0;}       // or data[i].p

  int   nData()      {return rcds.end();}                 // returns number of data items in array

  friend typename PersIter;
  };


extern Persons persons;
