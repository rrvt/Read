// Book Descriptions


#pragma once
#include "Archive.h"
#include "Date.h"
#include "ExpandableP.h"
#include "IterT.h"
#include "PerKeys.h"
#include "Persons.h"
#include "Record.h"

//class ABookDlg;
class BookDlg;


enum BookSortMode {NilBkSort, TtlBkSort, DtBkSort, AuthBkSort};


class Book : public Record {
public:

String  title;
PerKeys perKeys{};
String  location;
String  comment;
Date    pubDate;
Date    readDate;

        Book() {}
        Book(Book& b) {copy(b);}
       ~Book() {clear();}

  void  clear();

  Book& operator= (Book& b) {copy(b); return *this;}

  int   load(int dbVer, Archive& ar);
  void  store(Archive& ar);

  bool  verifyRefs() {return perKeys.verifyRefs();}

  bool  editRcd();
  void  get(BookDlg& dlg);
  bool  delRcd();

  void  setKey(BookSortMode mode);

  void  setKeys(Person* per, PerTyp typ);          //enum PerTyp    {NilTyp, AuthorTyp, ProtagTyp};
  bool  updateEntry(PerTypPos perTypPos, BookDlg& dlg);

  void  display(int initTab, int dbVer);

  void  dbgDsp();

  void  removeRefs(int bookID);                                     // BkDel
  void  removeRef(int personID) {perKeys.removeRef(personID);}      // PerDel

  bool  operator== ( Book& bk) {return key ==  bk.key;}
  auto  operator<=> (Book& bk) {return key <=> bk.key;}

private:

  bool   isPresent(Person* per);

  String getSortDate();
  void   fixReadDate();

  void copy(Book& b);
  };


class Books;
typedef DatumPtrT<Book, String> BookP;
typedef IterT<Books, Book>      BksIter;


class Books {
int                                 dbVer{};
int                                 nextIndex{1};
ExpandableP<Book, String, BookP, 2> data;
BookSortMode                        sortMode{NilBkSort};

public:

  Books() {}

  void setVersion(int ver) {dbVer = ver;}

  void clear() {data.clear();   nextIndex = 1;}

  void load(Archive& ar);
  void store(Archive& ar);

  bool addRecord();
  bool delRecord(Book* bk);

  Book* find(int bookID);

  void display(DspOptions opt);
  void display(Cstring& tgt);

  void loadCbx(CComboBox& cbx);
  void sort(BookSortMode mode);

private:

  void display();
  void displayRev();
  void dbgDsp();

  // returns either a pointer to data (or datum) at index i in array or zero

  Book*  datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  int    nData()      {return data.end();}                // returns number of data items in array
                                                          // not necessarily private
  void   removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename BksIter;
  };


extern Books books;



/////----------------

//PerIdP  authorIdP{};
//PerIdP  author2IdP{};
//PerIdP  protagIdP{};
//PerIdP  protag2IdP{};

//  void del(Book& book);
//  void removeRef(int bookID, int personID);
//  String get(COleDateTime& oDT, TCchar* fmt);
//  bool  verifyRef(PerTyp perTyp, int personID);
#if 0
  Book* find(TCchar* title);
#endif

