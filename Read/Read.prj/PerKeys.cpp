// Person Keys -- ID/Pointers to Persons, Authors and Protaganists


#include "pch.h"
#include "PerKeys.h"
#include "ABookIO.h"
#include "MessageBox.h"
#include "Utilities.h"


bool PerKey::operator== (PerKey& pk) {return idP()->key ==  pk.idP()->key;}
auto PerKey::operator<=>(PerKey& pk) {return idP()->key <=> pk.idP()->key;}


bool PerKeys::isPresent(int id) {
PerKysIter iter(*this);
PerKey*    pk;

  for (pk = iter(); pk; pk = iter++) if (pk->idP.id == id) return true;

  return false;
  }


PerKey*  PerKeys::find(int id) {
PerKysIter iter(*this);
PerKey*    pk;

  for (pk = iter(); pk; pk = iter++) if (pk->idP.id == id) return pk;

  return 0;
  }


String PerKeys::findAuth() {
PerKysIter iter(*this);
PerKey*    pk;
Person*    per;

  for (pk = iter(); pk; pk = iter++) {
    per = (*pk)();   if (!per) continue;

    if (per->perTyp == AuthorTyp) return per->getLFname();
    }

  return String(_T(""));
  }


Person* PerKeys::getPerson(PerTyp typ, int pos) {
PerKysIter iter(*this);
PerKey*    pk;
Person*    per;
int        i;

  for (i = 0, pk = iter(); pk; pk = iter++) {
    per = (*pk)();   if (per->perTyp == typ && ++i == pos) return per;
    }

  return 0;
  }


bool PerKeys::verifyRefs() {
PerKysIter iter(*this);
PerKey*    pk;
bool       dirty = false;

  for (pk = iter(); pk; pk = iter++)
                                   if (!persons.find(pk->idP.id)) {iter.remove();   dirty |= true;}
  return dirty;
  }





void PerKeys::removeRefs(int bookID) {            //BkDel
PerKysIter iter(*this);
PerKey*    pk;
Person*    per;
String     s;

  for (pk = iter(); pk; pk = iter++) {
    per = pk->idP();

    if (per && per->removeRef(bookID)) {
      s.format(_T("No book references, delete %s:  %s"), per->getPerType(), per->getFLname().str());
      if (msgYesNoBox(s) == IDYES) persons.del(per->id);
      }

    iter.remove();
    }
  }


void PerKeys::removeRef(int personID) {             // PerDel
PerKysIter iter(*this);
PerKey*    pk;

  for (pk = iter(); pk; pk = iter++) if (pk->idP.id == personID) {iter.remove();   return;}
  }


void PerKeys::load(ABookIO& aBkIO) {
int i;
int n;

  clear();  aBkIO.read(n);

  for (i = 0; i < n; i++) {PerKey perKey;   aBkIO.read(perKey.idP.id);   data += perKey;}
  }


void PerKeys::store(ABookIO& aBkIO) {
PerKysIter iter(*this);
PerKey*    pk;

  aBkIO.write(nData());

  for (pk = iter(); pk; pk = iter++) aBkIO.write(pk->idP.id);
  }


void PerKeys::display(int initTab) {
int        tabs[] = {20, 20, 20, 0};

PerKysIter iter(*this);
PerKey*    pk;
Person*    per;
String     s;

  setTabs(initTab, tabs);

  for (pk = iter(); pk; pk = iter++) {
    per = (*pk)();   if (!per) continue;

    s = per->getFLname();   if (s.isEmpty()) continue;

    notePad << nTab;

    switch (per->perTyp) {
      case AuthorTyp: notePad << _T("A:  "); break;
      case ProtagTyp: notePad << _T("P:  "); break;
      }

    notePad << s;
    }
  notePad << nCrlf;
  }


void PerKeys::dbgDsp() {
PerKysIter iter(*this);
PerKey*    pk;

  for (pk = iter(); pk; pk = iter++) notePad << nTab << pk->idP.id;
  }




//////-----------------
#if 0
void PerKeys::removeBookRefs(Book* book) {
PerKysIter iter(*this);
PerKey*    pk;

  for (pk = iter(); pk; pk = iter++) (*pk)()->removeBookRef(book);
  }
#endif
#if 1
#else
  notePad << nClrTabs << nSetTab(4) << nSetTab(24) << nSetTab(44) << nSetTab(64);
#endif

