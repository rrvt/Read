// Person Keys -- ID/Pointers to Persons, Authors and Protaganists


#pragma once
#include "PerIdP.h"
#include "Expandable.h"
#include "IterT.h"
#include "Persons.h"

class ABookIO;
class Book;


class PerKey {
public:
PerIdP idP{};

  PerKey() { }
  PerKey(PerKey& pk) {copy(pk);}
 ~PerKey() { }

  void clear() {idP.clear();}

  void set(int id) {idP.set(id);}             // Bk Edit

  PerKey& operator= (PerKey& pk) {copy(pk);   return *this;}

  Person* operator() () {return idP();}

  bool operator== (PerKey& pk);                  // {return idP()->key ==  pk.idP()->key;}
  auto operator<=>(PerKey& pk);                  // {return idP()->key <=> pk.idP()->key;}

  operator PerKey*() {return this;}              // Converts this to Datum*   const

private:

  void copy(PerKey& b) {idP = b.idP;}
  };


class PerKeys;
typedef IterT<PerKeys, PerKey> PerKysIter;


class PerKeys {

int                   dbVer{};

Expandable<PerKey, 2> data;

public:

  PerKeys() { }
  PerKeys(PerKeys& pks) {copy(pks);}
 ~PerKeys() {clear();}

 void     clear() {data.clear();}

  void     setVersion(int ver) {dbVer = ver;}

  PerKeys& operator= (PerKeys& pks) {copy(pks);   return *this;}

  bool     isEmpty() {return !nData();}

  void     load( ABookIO& aBkIO);
  void     store(ABookIO& aBkIO);

  void     add(int id) {data.nextData().set(id);}       // Bk Edit
  bool     isPresent(int id);

  Person*  getAuthor(int pos) {return getPerson(AuthorTyp, pos);}
  Person*  getProtag(int pos) {return getPerson(ProtagTyp, pos);}

  void     removeRefs(int bookID);
  void     removeRef(int personID);                 // PerDel


  bool     verifyRefs();

  PerKey*  find(int id);
  String   findAuth();

  void     display(int initTab);
  void     dbgDsp();

private:

  Person* getPerson(PerTyp typ, int pos);

  void copy(PerKeys& pks) {data = pks.data;}

  // returns either a pointer to data (or datum) at index i in array or zero

  PerKey* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  int     nData()      {return data.end();}              // returns number of data items in array
                                                          // not necessarily private
  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename PerKysIter;
  };

