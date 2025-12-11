// Book Keys


#pragma once
#include "BkIdP.h"
#include "Expandable.h"
#include "IterT.h"
#include "Record.h"

class Archive;
class Book;


class BookKey {
public:
BkIdP idP{};

  BookKey() { }
  BookKey(BookKey& b) {copy(b);}
 ~BookKey()           {clear();}

  void clear() {idP.clear();}

  BookKey& operator= (BookKey& b) {copy(b); return *this;}

  Book*    operator() () {return idP();}

 //  *p == *q
 //  *p >  *q
 //  *p <= *q

  bool operator== (BookKey& b);
  auto operator<=>(BookKey& b);

  operator BookKey*() {return this;}              // Converts this to Datum*   const

private:

  void copy(BookKey& b) {idP = b.idP;}
  };


class BookKeys;
typedef IterT<BookKeys, BookKey>   BkKysIter;


class BookKeys {
int                    dbVer{};

Expandable<BookKey, 2> data;

public:

  BookKeys() {}
  BookKeys(BookKeys& bk) {copy(bk);}
 ~BookKeys() {clear();}

  void setVersion(int ver) {dbVer = ver;}

  void clear() {data.clear();}

  BookKeys& operator=  (BookKeys& bk) {copy(bk); return *this;}
  BookKeys& operator+= (BookKeys& bk);

  bool     isEmpty() {return !nData();}

  void     addRef(Book* book);

  BookKey* find(int id);

  void     sort();

  void     removeRefs(int personID);            // PerDel -- remove all references
  bool     removeRef( int bookID);              // BkDel  -- remove one reference
                                                // return true if empty
  bool     removeDuplicates();

  bool     verifyRefs(int personID);
  void     load( Archive& ar);
  void     store(Archive& ar);

  void     display(int index);
  void     dbgDsp();

private:

  void copy(BookKeys& bk) {data = bk.data;}

  // returns either a pointer to data (or datum) at index i in array or zero

  BookKey* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  int      nData()      {return data.end();}              // returns number of data items in array
                                                          // not necessarily private
  void     removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename BkKysIter;
  };



//////-----------------
#if 0
class BkIdP {
Book* book{};

public:
int   id{};

  void clear() {id = 0; book = 0;}

  operator int ()     {return id;}

  Book* operator() ();    // {return book ? book : book = books.find(id);}
  };
#endif
#if 0
int   id{};
Book* book{};
#endif
#if 0
String title;
int    bookID;   : bookID(0) {title.clear();}   {title.clear(); bookID = 0;}
#endif

//  bool    operator (BookKey& b) {return book ? book->title <  b.book->title : false;}
//{title = b.title; bookID = b.bookID;}
//  BookKey* find(  int bookID, String& title);

//  void     updateRefs();
//  void     resolveIDs();
//  void     resolveIDs(Archive& ar);
//  void     delRef(Book* book);

