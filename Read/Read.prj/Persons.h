// List of Persons

#pragma once
#include "Archive.h"
#include "BookKeys.h"
#include "ExpandableP.h"
#include "IterT.h"
#include "Record.h"



enum PerTypPos {NilPos, AuthorPos, Author2Pos, ProtagPos, Protag2Pos};
enum PerTyp    {NilTyp, AuthorTyp, ProtagTyp};


class Person : public Record {
public:

PerTyp   perTyp{NilTyp};
String   fName;
String   lName;
BookKeys bookKeys;

  Person() { }
 ~Person() {clear();}

  void clear();

  Person(Person& a) {copy(a);}

  Person& operator= (Person& a) {copy(a); return *this;}

  //  *p == *q
  //  *p >  *q
  //  *p <= *q

  bool    operator== (Person& a)  {return key ==  a.key;}
  auto    operator<=>(Person& a)  {return key <=> a.key;}

  int      load(int dbVer, Archive& ar);
  void     store(Archive& ar);

  Person*  add(PerTyp pTyp, TCchar* firstName, TCchar* lastName);
  bool     editRcd();

  void     sort();

  void     removeRefs()             {bookKeys.removeRefs(id);}
  void     removeRefs(int personID) {return bookKeys.removeRefs(personID);} // PerDel

  bool     removeRef(int bookID) {return bookKeys.removeRef(bookID);}       //BkDel
                                                                 // returns true if bookKeys empty

  bool     removeDuplicates() {return bookKeys.removeDuplicates();}
  bool     verifyRefs()       {return bookKeys.verifyRefs(id);}

  void     display();
  void     dspName();

  String   getFLname();
  String   getLFname();
  TCchar*  getPerType();

  void     dbgDsp();

private:

  void     setKey() {key = perTyp;   key += lName + fName;}

  void     copy(Person& a);
  };


class Persons;
typedef DatumPtrT<Person, String> PersonP;
typedef IterT<Persons, Person>    PersIter;


class Persons {
int                                     dbVer{};
int                                     nextIndex;
ExpandableP<Person, String, PersonP, 2> data;

public:

  Persons() : nextIndex(1) {}

  void     setVersion(int ver) {dbVer = ver;}

  void     clear() {data.clear();   nextIndex = 1;}

  void     load(Archive& ar);
  void     store(Archive& ar);

  Person*  add( PerTyp perTyp, TCchar* fName, TCchar* lName);
  Person*  find(PerTyp perTyp, TCchar* fName, TCchar* lName);
  Person*  find(int id);

  void     sort();

  bool     delRecord(Person* person);               // PerDel

  void     del(int personID);

  bool     mergeDuplicates();                       // FixIt

  void     display(DspOptions opt);
  void     display();
  bool     display(int id);
  void     display(PerTyp typ, Cstring& tgt);
  void     dbgDsp();

  void     loadCbx(PerTyp typ, CComboBox& cbx);

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Person* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  int     nData()      {return data.end();}               // returns number of data items in array
                                                          // not necessarily private
  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename PersIter;
  };


extern Persons persons;





/////////-----------------
//  Person*  find(PerTyp perTyp, int id);
//  bool    operator>  (Person& a);

//  Person*  startLoop() {x = -1; return nextNode();}
//  Person*  nextNode()  {return ++x < noRcds() ? &rcds[x] : 0;}

//  void examinePersons();

//  void     updateRefs()              {bookKeys.updateRefs();}

//  int      number() {return noRcds();}
//  void     resolveIDs(Archive& ar);
//  void     del(Person& person);
//  Person*  update(PerTyp perTyp, int perID, TCchar* fName, TCchar* lName);
//  void     removeBookRef(int personID, Book* book);
//  void     expunge();


