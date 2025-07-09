// List of Persons


#include "pch.h"
#include "Persons.h"
#include "ABookIO.h"
#include "Books.h"
#include "NotePad.h"
#include "qsort.h"

#include "Read.h"




void Persons::clear() {
int i;

  for (i = 0; i < noRcds(); i++) rcds[i].clear();
  rcds.clear(); nextIndex = 1;
  }



Person* Persons::add(PerTyp perTyp, TCchar* fName, TCchar* lName) {
Person& r = rcds[noRcds()];

  r.perTyp = perTyp; r.fName = fName; r.lName = lName; r.add(nextIndex);  return &r;
  }




Person* Persons::find(PerTyp perTyp, TCchar* fName, TCchar* lName) {
int i;

  for (i = 0; i < noRcds(); i++) {
    Person& r = rcds[i];

    if (r.perTyp == perTyp && r.fName == fName && r.lName == lName) return &r;
    }
  return 0;
  }


Person*  Persons::find(PerTyp perTyp, int id) {
int i;

  if (!id) return 0;

  for (i = 0; i < noRcds(); i++) {
    Person& r = rcds[i];

    if (r.perTyp == perTyp && r.id == id) return &r;
    }

  return 0;
  }


void Persons::sort() {
PersIter iter(*this);
Person*  author;

  if (noRcds()) qsort(&rcds[0], &rcds[noRcds()-1]);

  for (author = iter(); author; author = iter++) author->bookKeys.sort();
  }


void Persons::display() {
int i;

  notePad << _T("Persons") << nCrlf << nCrlf;
  notePad << nSetRTab(3) << nSetTab(5);

  for (i = 0; i < noRcds(); i++) {Person& r = rcds[i];   if (!r.bookKeys.isEmpty()) r.display();}
  }



bool Persons::display(int id) {
int i;

  if (!id) return false;

  for (i = 0; i < noRcds(); i++) {
    Person& r = rcds[i];

    if (r.bookKeys.isEmpty()) continue;

    if (r.id == id) {notePad << nTab; r.dspName(); return true;}
    }

  return false;
  }



void Persons::display(PerTyp typ, CString tgt) {
int  i;
bool lineBrk = false;

  for (i = 0; i < noRcds(); i++) {
    Person& p = rcds[i];

    if (p.perTyp != typ || p.bookKeys.isEmpty()) continue;

    if (lineBrk) notePad << nCrlf;   lineBrk = true;

    if (p.lName == tgt) {p.display();}
    }
  }


void Person::display() {

  notePad << nClrTabs << nSetRTab(3) << nSetTab(5) << nSetTab(12);

  notePad << nTab << id << nTab;  dspName();  notePad << nCrlf;

  notePad << nClrTabs << nSetTab(7);

  bookKeys.display(id);
  }


void Person::dspName() {

  switch (perTyp) {
    case AuthorTyp:
    case Author :
    case Author2: notePad << _T("Author:     "); break;
    case ProtagTyp:
    case Protag :
    case Protag2: notePad << _T("Protagonist:"); break;
    }
  notePad << nTab << fName << _T(" ") << lName;
  }



void Persons::expunge() {
int i;

  for (i = 0; i < noRcds(); i++) {
    Person& p = rcds[i];

    if (p.bookKeys.isEmpty()) {rcds.del(i); i--;}
    }
  }


void Persons::load(Archive& ar) {
int n;
int i;
int x;

  clear();   ar.readBin(n);    if (!n) return;

  for (i = 0; i < n; i++) {
    x = rcds[noRcds()].load(ar);
    if (x > nextIndex) nextIndex = x;
    }

  nextIndex++;
  }


void Persons::store(Archive& ar) {
int     i;

  ar.wrtBin(noRcds());

  for (i = 0; i < noRcds(); i++) {
    if (!rcds[i].bookKeys.isEmpty()) rcds[i].store(ar);
    }
  }



void Persons::del(Person& person) {
int i;
int n = noRcds();
int personID = person.id;

  person.removeRefs();

  for (i = 0; i < n; i++) if (rcds[i].id == personID) {rcds.del(i); return;}
  }



void Persons::removeBookRef(int personID, int bookID) {
int i;
int n = noRcds();

  if (!personID) return;

  for (i = 0; i < n; i++) {
    Person& person = rcds[i];
    if (person.id == personID) {person.removeBookRef(bookID); return;}
    }
  }


int Person::load(Archive& ar) {
ABookIO aBkIO(ar);

  aBkIO.read(id);
  aBkIO.read(perTyp);
  aBkIO.read(fName);
  aBkIO.read(lName);

  bookKeys.load(ar);   return id;
  }


void Person::store(Archive& ar) {
ABookIO aBkIO(ar);

  aBkIO.write(id);
  aBkIO.write(perTyp);
  aBkIO.write(fName);
  aBkIO.write(lName);

//  aBkIO.flush();

  bookKeys.store(ar);
  }


void Person::clear() {bookKeys.clear(); fName.clear(); lName.clear();}


void Person::copy(Person& a) {

  rcdCopy(a);  perTyp = a.perTyp;   fName = a.fName;   lName = a.lName;

  bookKeys = a.bookKeys;
  }


bool Person::operator== (Person& a)
                              {return perTyp == a.perTyp && fName == a.fName &&  lName == a.lName;}


bool Person::operator>  (Person& a) {
  return  perTyp >  a.perTyp || perTyp == a.perTyp &&
         (lName  >  a.lName  || (lName == a.lName  && fName >  a.fName));
  }


bool Person::operator<= (Person& a) {
  return perTyp <  a.perTyp || perTyp == a.perTyp &&
        (lName  <  a.lName  || (lName == a.lName  && fName <= a.fName));
  }


void BookKeys::clear() {
int i;

  for (i = 0; i < noRcds(); i++) rcds[i].clear();

  rcds.clear();
  }


void BookKeys::addRef(int bookID, String& title) {
int i;

  for (i = 0; i < noRcds(); i++) if (rcds[i].bookID == bookID) {rcds[i].title = title; return;}

  BookKey& bookKey = rcds[noRcds()];

  bookKey.bookID = bookID;   bookKey.title = title;
  }


void BookKeys::delRef(int bookID) {
int i;

  for (i = 0; i < noRcds(); i++) {
    BookKey& bookKey = rcds[i];
    if (bookKey.bookID == bookID) {bookKey.title.clear();   rcds.del(i); break;}
    }
  }


BookKey* BookKeys::find(  int bookID, String& title) {
int i;

  for (i = 0; i < noRcds(); i++) {
    BookKey& bookKey = rcds[i];
    if (bookKey.bookID == bookID && bookKey.title == title) return &bookKey;
    }

  return 0;
  }


void BookKeys::sort() {
  if (noRcds()) qsort(&rcds[0], &rcds[noRcds()-1]);
  }


void BookKeys::updateRefs() {
int i;

  for (i = 0; i < noRcds(); i++) {
    BookKey& bookKey = rcds[i];

    Book* book = books.find(bookKey.bookID);

    if (book) bookKey.title = book->title;
    else     {rcds.del(i); i--;}
    }
  }



void BookKeys::removeRefs(int personID) {
int i;
int n = noRcds();

  for (i = 0; i < n; i++) books.removeRef(rcds[i].bookID, personID);
  }


void BookKeys::removeBookRef(int bookID) {
int i;

  for (i = 0; i < noRcds(); i++)
            {BookKey& bookKey = rcds[i];   if (bookKey.bookID == bookID) {rcds.del(i); i--;}}
  }


bool BookKeys::removeDuplicates() {
int  i;
int  j;
bool dirty = false;

  for (i = 0; i < noRcds()-1; i++) {
    BookKey& bookKey = rcds[i];

    for (j = i+1; j < noRcds(); j++) {
      BookKey& b = rcds[j];

      if (bookKey.bookID == b.bookID)
        {rcds.del(j); j--; dirty = true;}
      }
    }

  return dirty;
  }


bool BookKeys::verifyRefs(RecordID personID) {
int  i;
bool dirty = false;

  for (i = 0; i < noRcds(); i++) {

    Book* book = books.find(rcds[i].bookID);

    if (book) {
      if (book->authorID  == personID) continue;
      if (book->author2ID == personID) continue;
      if (book->protagID  == personID) continue;
      if (book->protag2ID == personID) continue;
      }

    rcds.del(i); i--; dirty = true;
    }

  return dirty;
  }



void BookKeys::display(int id) {
int   i;
Book* book;
bool  lineBrk = false;

  for (i = 0; i < noRcds(); i++) {
    BookKey& bookKey = rcds[i];

    book = books.find(bookKey.bookID);    if (!book) continue;

    if (lineBrk) notePad << nCrlf;   lineBrk = true;

    book->display(id);
    }
  }


void  BookKeys::load(Archive& ar) {
ABookIO aBkIO(ar);
int     i;
int     n;

  rcds.clear();  aBkIO.read(n);

  for (i = 0; i < n; i++) aBkIO.read(rcds[i].bookID);
  }


void BookKeys::store(Archive& ar) {
ABookIO aBkIO(ar);
int     i;

  aBkIO.write(noRcds());

  for (i = 0; i < noRcds(); i++) aBkIO.write(rcds[i].bookID);
  }


