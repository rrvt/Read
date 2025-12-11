// List of Persons


#include "pch.h"
#include "Persons.h"
#include "ABookIO.h"
#include "Books.h"
#include "NotePad.h"
#include "PersonDlg.h"
#include "qsort.h"
#include "RegExpr.h"
#include "Resource.h"
#include "Utilities.h"


void Persons::store(Archive& ar) {
PersIter iter(*this);
Person*  r;

  ar.wrtBin(nData());

  for (r = iter(); r; r = iter++) r->store(ar);
  }


void Person::store(Archive& ar) {
ABookIO aBkIO(ar);

  aBkIO.write(id);
  aBkIO.write(Byte(perTyp));
  aBkIO.write(fName);
  aBkIO.write(lName);

  bookKeys.store(ar);
  }


void Persons::load(Archive& ar) {
int     n;
int     i;
Person* r;
int     x;

  clear();   ar.readBin(n);    if (!n) return;

  for (i = 0; i < n; i++) {

    r = data.allocate();   x = r->load(dbVer, ar);   data = r;

    if (x > nextIndex) nextIndex = x;
    }

  nextIndex++;
  }


int Person::load(int dbVer, Archive& ar) {
ABookIO aBkIO(ar);
Byte    byte;

  aBkIO.read(id);
  aBkIO.read(byte); perTyp = (PerTyp) (byte >= 5 ? byte - 4 : byte);
  aBkIO.read(fName);
  aBkIO.read(lName);

  setKey();

  bookKeys.setVersion(dbVer);   bookKeys.load(ar);   return id;
  }


Person* Persons::add(PerTyp perTyp, TCchar* fName, TCchar* lName) {
Person* person;

  person = find(perTyp, fName, lName);   if (person) return person;

  person = data.allocate();
  person->set(nextIndex);   person->add(perTyp, fName, lName);   data = person;   return person;
  }


Person*  Person::add(PerTyp pTyp, TCchar* firstName, TCchar* lastName) {
  perTyp = pTyp;
  fName  = firstName;
  lName  = lastName;
  setKey();   return this;
  }


bool Person::editRcd() {
PersonDlg dlg;
bool      dirty = false;

  dlg.firstName = fName;
  dlg.lastName  = lName;

  if (dlg.DoModal() == IDOK) {
    dirty = fName != dlg.firstName;
    dirty = lName != dlg.lastName;

    fName = dlg.firstName;
    lName = dlg.lastName;
    }

  return dirty;
  }


bool Persons::delRecord(Person* person) {             // PerDel
PersIter iter(*this);
Person*  per;

  for (per = iter(); per; per = iter++) if (per == person) {

    per->removeRefs(person->id);

    iter.remove();   return true;
    }

  return false;
  }


Person* Persons::find(PerTyp perTyp, TCchar* fName, TCchar* lName) {
PersIter iter(*this);
Person*  r;

  for (r = iter(); r; r = iter++)
                       if (r->perTyp == perTyp && r->fName == fName && r->lName == lName) return r;
  return 0;
  }


Person* Persons::find(int id) {
PersIter iter(*this);
Person*  r;

  if (!id) return 0;

  for (r = iter(); r; r = iter++) if (r->id == id) return r;

  return 0;
  }


void Persons::sort() {
PersIter iter(*this);
Person*  r;

  if (nData()) qsort(&data[0], &data[nData()-1]);

  for (r = iter(); r; r = iter++) r->bookKeys.sort();
  }


bool Persons::mergeDuplicates() {
PersIter iter(*this);
Person*  perI;
PersIter iterJ(*this);
Person*  perJ;
bool     dirty = false;

  for (perI = iter(); perI; perI = iter++) {

    for (iterJ = iter, perJ = iterJ++; perJ; perJ = iterJ++) {

      if (perI->perTyp == perJ->perTyp &&
          perI->fName  == perJ->fName  &&
          perI->lName  == perJ->lName)
                             {perI->bookKeys += perJ->bookKeys;   iterJ.remove();   dirty |= true;}
      }
    }

  return dirty;
  }


void Persons::display(DspOptions opt) {
  switch (opt) {
    case NilDsp   :
    case TitleDsp :
    case DateDsp  :
    case AuthDsp  :
    case DebugDsp : dbgDsp(); break;
    }
  }


void Persons::display() {
PersIter iter(*this);
Person*  r;

  notePad << _T("Persons") << nCrlf << nCrlf;
  notePad << nSetRTab(3) << nSetTab(5);

  for (r = iter(); r; r = iter++) r->display();
  }


bool Persons::display(int id) {
PersIter iter(*this);
Person*  r;

  if (!id) return false;

  for (r = iter(); r; r = iter++) {

    if (r->id == id) {notePad << nTab; r->dspName(); return true;}
    }

  return false;
  }


void Persons::display(PerTyp typ, Cstring& tgt) {
String   t = _T(".*") +  tgt + _T(".*");
RegExpr  re(t);
PersIter iter(*this);
Person*  r;
bool     lineBrk = false;

  for (r = iter(); r; r = iter++) {

    if (r->perTyp != typ) continue;

    if (re.match(r->getFLname())) {if (lineBrk) notePad << nCrlf;  r->display();  lineBrk = true;}
    }
  }


void Person::display() {
int tabs[] = {1, 9, 0};
  setTabs(-3, tabs);

  notePad << nTab << id << nTab;  dspName();  notePad << nCrlf;

  bookKeys.display(id);
  }


void Persons::dbgDsp() {
PersIter iter(*this);
Person*  per;
bool     isProtag = false;

  notePad << _T("People") << nCrlf << nCrlf;                // in Sorted Order

  for (per = iter(); per; per = iter++) {
    if (!isProtag && per->perTyp == ProtagTyp) {notePad << nCrlf;   isProtag = true;}

    per->dbgDsp();
    }
  }


void Person::dbgDsp() {
String s = perTyp == AuthorTyp ? _T("Author") : _T("Protag");

  notePad << nClrTabs << nSetRTab(3) << nSetTab(4) << nSetTab(13);
  notePad << nTab << id;
  notePad << nTab << s;
  notePad << nTab << fName << _T(' ') << lName;
  bookKeys.dbgDsp();   notePad << nCrlf;
  }


void Person::dspName() {

  switch (perTyp) {
    case AuthorTyp: notePad << _T("Author:  "); break;
    case ProtagTyp: notePad << _T("Protagonist:  "); break;
    }
  notePad << getFLname();
  }


void Persons::loadCbx(PerTyp typ, CComboBox& cbx) {
PersIter iter(*this);
Person*  per;
int      x;

  for (per = iter(); per; per = iter++)
       if (per->perTyp == typ) {x = cbx.AddString(per->getLFname());   cbx.SetItemDataPtr(x, per);}
  }


String Person::getFLname() {
String s;

  s = lName;   s.trim();   fName.trim();

  if (!fName.isEmpty()) {if (!s.isEmpty()) s = fName + _T(" ") + s;}

  return s;
  }


String Person::getLFname() {
String s;

  s = lName;   s.trim();   fName.trim();

  if (!fName.isEmpty()) {if (!s.isEmpty()) s += _T(", ");   s += fName;}

  return s;
  }


TCchar* Person::getPerType() {
  switch (perTyp) {
    case AuthorTyp: return _T("Author");
    case ProtagTyp: return _T("Protagonist");
    case NilTyp   :
    default       : return _T("NilTyp");
    }
  }



void Persons::del(int personID) {
PersIter iter(*this);
Person*  r;

  for (r = iter(); r; r = iter++) if (r->id == personID) {iter.remove(); return;}
  }


void Person::clear() {bookKeys.clear(); fName.clear(); lName.clear();}


void Person::copy(Person& a) {

  Record::copy(a);  perTyp = a.perTyp;   fName = a.fName;   lName = a.lName;

  bookKeys = a.bookKeys;
  }



//////--------------
#if 0
bool Person::operator== (Person& a)
                              {return key == a.key;}

bool Person::operator>  (Person& a) {
  return  perTyp >  a.perTyp || perTyp == a.perTyp &&
         (lName  >  a.lName  || (lName == a.lName  && fName >  a.fName));
  }
auto Person::operator<=> (Person& a) {return key <=> a.key;}
#endif
//  aBkIO.flush();
//#include "Read.h"
#if 0
void Persons::clear() {
int i;

  for (i = 0; i < noRcds(); i++) rcds[i].clear();
  rcds.clear(); nextIndex = 1;
  }
#endif
#if 0
Person*  Persons::find(PerTyp perTyp, int id) {
PersIter iter(*this);
Person*  r;

  if (!id) return 0;

  for (r = iter(); r; r = iter++) if (r->perTyp == perTyp && r->id == id) return r;

  return 0;
  }
#endif
#if 0
    if (x == IDC_EditPerson) {
      person->fName = fName; person->lName = lName;
      }
    else if (x == IDC_DeleteRef) {
      person->bookKeys.delRef(book->id);
      person = persons.add(perTyp, fName, lName);
      }
#endif
//#include "EditOrDelRefDlg.h"
#if 1
#else
  person->set(nextIndex);
  person->perTyp = perTyp;
  person->fName  = fName;
  person->lName  = lName;
  setKey();   data = person;   return person;
#endif
#if 1
#else
  notePad << nClrTabs << nSetRTab(3) << nSetTab(4) << nSetTab(12);
#endif
#if 0
Person* Persons::update(PerTyp perTyp, int perID, TCchar* fName, TCchar* lName) {
#if 1
  return 0;
#else
Person*         person;
EditOrDelRefDlg edlg;

  person = find(perID);
  if (person) {
    if (person->fName == fName && person->lName == lName) return person;

    edlg.person = person->fName + _T(" ") + person->lName;

    int x = edlg.DoModal();

    return person;
    }

  return add(perTyp, fName, lName);
#endif
  }
#endif
//    if (r->bookKeys.isEmpty()) continue;
#if 0
void Persons::resolveIDs(Archive& ar) {
PersIter iter(*this);
Person*  r;

  for (r = iter(); r; r = iter++) r->bookKeys.resolveIDs(ar);
  }
#endif
//void Persons::del(Person& person) {person.removeRefs();   del(person.id);}
#if 0
void Persons::removeBookRef(int personID, Book* book) {
PersIter iter(*this);
Person*  r;

  if (!personID) return;

  for (r = iter(); r; r = iter++) if (r->id == personID) {r->removeBookRef(book); return;}
  }
#endif
#if 0
void Persons::expunge() {
PersIter iter(*this);
Person*  r;

  for (r = iter(); r; r = iter++) if (r->bookKeys.isEmpty()) iter.remove();
  }
#endif



