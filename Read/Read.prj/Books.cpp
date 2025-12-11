// Books Description


#include "pch.h"
#include "Books.h"
#include "BookDlg.h"
#include "ABookIO.h"
#include "NotePad.h"
#include "qsort.h"
#include "Persons.h"
#include "RegExpr.h"
#include "Utilities.h"

//#include "MessageBox.h"       // Debugging


void Books::load(Archive& ar) {
int   n;
int   i;
Book* bk;
int   x;

  clear();   ar.readBin(n);    if (!n) return;

  for (i = 0; i < n; i++) {
    bk = data.allocate();

    x = bk->load(dbVer, ar);   if (x > nextIndex) nextIndex = x;

    bk->setKey(TtlBkSort);   data = bk;
    }

  nextIndex++;   sortMode = TtlBkSort;
  }


int Book::load(int dbVer, Archive& ar) {
ABookIO aBkIO(ar);
int     pkId;
String  s;

  aBkIO.read(id);
  aBkIO.read(title);

  if (dbVer == 1) {
    aBkIO.read(pkId);  if (pkId) perKeys.add(pkId);
    aBkIO.read(pkId);  if (pkId) perKeys.add(pkId);
    aBkIO.read(pkId);  if (pkId) perKeys.add(pkId);
    aBkIO.read(pkId);  if (pkId) perKeys.add(pkId);
    }

  else if (dbVer >= 2) perKeys.load(aBkIO);

  aBkIO.read(location);

  switch (dbVer) {
    case 1  : aBkIO.read(s);   pubDate = getYYDate(s);   break;
    case 3  : aBkIO.read(s);   pubDate = s;              break;
    default : aBkIO.read(pubDate);                       break;
    }

  switch (dbVer) {
    case 1  : fixReadDate(); break;
    case 3  : aBkIO.read(s);   readDate = s;   fixReadDate();   break;
    default : aBkIO.read(readDate);                             break;
    }

  aBkIO.read(comment);   return id;
  }


void Book::fixReadDate() {
Date      today;
CTimeSpan span;

  if (!readDate.isMinDate()) return;

  today.getToday();
  span = today - pubDate;

  readDate = pubDate;   readDate += span.GetTotalSeconds() / 2;
  }


void Books::store(Archive& ar) {
BksIter iter(*this);
Book*   bk;

  ar.wrtBin(nData());

  for (bk = iter(); bk; bk = iter++) bk->store(ar);
  }


void Book::store(Archive& ar) {
ABookIO aBkIO(ar);

  aBkIO.write(id);
  aBkIO.write(title);

  perKeys.store(aBkIO);

  aBkIO.write(location);
  aBkIO.write(pubDate);
  aBkIO.write(readDate);
  aBkIO.write(comment);
  }


bool Books::addRecord() {
BookDlg dlg;
Book*   bk;

  if (dlg.DoModal() != IDOK || dlg.title.isEmpty()) return false;

  bk = data.allocate();   bk->set(nextIndex);   bk->get(dlg);   bk->setKey(sortMode);   data = bk;

  return true;
  }


bool Book::editRcd() {                  // Bk Edit
BookDlg dlg;
bool    dirty = false;
Date    dt;
String  s;
String  t;

  dlg.title = title;
  dlg.author1  = perKeys.getAuthor(1);
  dlg.author2  = perKeys.getAuthor(2);
  dlg.protag1  = perKeys.getProtag(1);
  dlg.protag2  = perKeys.getProtag(2);
  dlg.location = location;
  dlg.comment  = comment;
  dlg.pubDate  = pubDate;
  dlg.readDate = readDate;

  if (dlg.DoModal() != IDOK || dlg.title.isEmpty()) return dirty;

  dirty |= title != dlg.title;
  dirty |= !isPresent(dlg.author1);
  dirty |= !isPresent(dlg.author2);
  dirty |= !isPresent(dlg.protag1);
  dirty |= !isPresent(dlg.protag2);
  dirty |= location != dlg.location;
  dirty |= comment  != dlg.comment;
  dt = dlg.pubDate;    s = dt.format(_T("%Y"));         t = pubDate.format(_T("%Y"));
  dirty |= s != t;
  dt = dlg.readDate;   s = dt.format(_T("%m/%d/%y"));   t = readDate.format(_T("%m/%d/%y"));
  dirty |= s != t;

  get(dlg);   return dirty;
  }


bool Book::isPresent(Person* per) {

  if (!per) return true;

  return perKeys.isPresent(per->id);
  }


void Book::get(BookDlg& dlg) {

  title    = dlg.title;     title.trim();

  perKeys.clear();
  setKeys(dlg.author1, AuthorTyp);
  setKeys(dlg.author2, AuthorTyp);
  setKeys(dlg.protag1, ProtagTyp);
  setKeys(dlg.protag2, ProtagTyp);

  location = dlg.location;  location.trim();

  pubDate  = dlg.pubDate;
  readDate = dlg.readDate;

  comment  = dlg.comment;   comment.trim();
  }


void Book::setKeys(Person* per, PerTyp typ) {       // Bk Edit

  if (!per) return;

  per->bookKeys.addRef(this);   perKeys.add(per->id);
  }


bool Book::updateEntry(PerTypPos perTypPos, BookDlg& dlg) {
  return true;
  }


bool Books::delRecord(Book* book) {                 // BkDel
BksIter iter(*this);
Book*   bk;

  for (bk = iter(); bk; bk = iter++) if (bk == book) {

    book->removeRefs(book->id);

    iter.remove();   return true;
    }

  return false;
  }


void Book::removeRefs(int bookID) {perKeys.removeRefs(bookID);}       //BkDel


Book* Books::find(int bookID) {
BksIter iter(*this);
Book*   bk;

  for (bk = iter(); bk; bk = iter++) if (bk->id == bookID) return bk;

  return 0;
  }


void  Books::display(Cstring& tgt) {
String  t = _T(".*") +  tgt + _T(".*");
RegExpr re(t);
BksIter iter(*this);
Book*   bk;
String  s;
bool    lineBrk = false;

  for (bk = iter(); bk; bk = iter++) {

    if (re.match(bk->title)) {

      if (lineBrk) notePad << nCrlf;   lineBrk = true;

      bk->display(0, false);
      }
    }
  }


void Books::loadCbx(CComboBox& cbx) {
BksIter iter(*this);
Book*   bk;
int     x;

  sort(TtlBkSort);

  for (bk = iter(); bk; bk = iter++) {x = cbx.AddString(bk->title);   cbx.SetItemDataPtr(x, bk);}
  }


void Books::sort(BookSortMode mode) {
BksIter iter(*this);
Book*   bk;

  if (sortMode == mode) return;

  for (bk = iter(); bk; bk = iter++) bk->setKey(mode);

  qsort(&data[0], &data[nData()-1]);    sortMode = mode;
  }


void Book::setKey(BookSortMode mode) {

  switch (mode) {
    case TtlBkSort  : key.clear();                break;
    case DtBkSort   : key = getSortDate();        break;
    case AuthBkSort : key = perKeys.findAuth();   break;
    }

  key += titleKey(title);
  }


String Book::getSortDate() {return readDate.format(_T("%Y%m%d"));}


void Books::display(DspOptions opt) {
  switch (opt) {
    case NilDsp   : break;
    case TitleDsp : sort(TtlBkSort);    display(); break;
    case DateDsp  : sort(DtBkSort);     displayRev();   break;
    case AuthDsp  : sort(AuthBkSort);   display(); break;
    case DebugDsp : dbgDsp(); break;
    }
  }


void Books::display() {
BksIter iter(*this);
Book*   bk;
bool    blkLn = false;

  for (bk = iter(); bk; bk = iter++)
                           {if (blkLn) notePad << nCrlf;   bk->display(-3, dbVer);   blkLn = true;}
  }


void Books::displayRev() {
BksIter iter(*this);
Book*   bk;
bool    blkLn = false;

  for (bk = iter(BksIter::Rev); bk; bk = iter--)
                           {if (blkLn) notePad << nCrlf;   bk->display(-3, dbVer);   blkLn = true;}
  }



void Book::display(int initTab, int dbVer) {
int  tabs[]   = {1, 9, 0};
int  initTab2 = abs(initTab) + 1;
int  tabs2[]  = {15, 20, 20, 0};
bool locPres  = !location.isEmpty();
bool pubPres  = !pubDate.isEmpty();
bool readPres = !readDate.isEmpty();

  setTabs(initTab, tabs);

  notePad << nTab << id << nTab << _T("Title:  ") << title << nCrlf;

  perKeys.display(initTab2);
  setTabs(abs(initTab2), tabs2);

  if (locPres || pubPres || readPres) {

    notePad << nTab;
                  if (pubPres)  notePad << _T("PubDate:  ")     << pubDate.format(_T("%Y"));
    notePad << nTab;
                  if (readPres) notePad << _T("Read Date:  ")   << readDate.format(_T("%m/%d/%y"));
                  if (locPres)  notePad << nTab << _T("Loc:  ") << location;
    notePad << nCrlf;
    }

  if (!comment.isEmpty())  notePad << nTab << _T("Comment:  ")   << comment  << nCrlf;
  }


void Books::dbgDsp() {
BksIter iter(*this);
Book*   bk;

  notePad << _T("Books in Sorted Order") << nCrlf << nCrlf;

  for (bk = iter(); bk; bk = iter++) bk->dbgDsp();
  }


void Book::dbgDsp() {
int        tab = 35;
int        i;

  notePad << nClrTabs << nSetRTab(3) << nSetTab(4);
  for (i = 0; i < 8; i++) notePad << nSetRTab(tab + i * 5);

  notePad << nTab << id;
  notePad << nTab << title;

  perKeys.dbgDsp();
  notePad << nCrlf;
  }


void Book::clear() {
  key.clear();        title.clear();       perKeys.clear(); location.clear();   pubDate.clear();
  comment.clear();    readDate.clear();
  }


void Book::copy(Book& b) {
  Record::copy(b);
  key        = b.key;
  title      = b.title;
  perKeys    = b.perKeys;
  location   = b.location;
  pubDate    = b.pubDate;
  comment    = b.comment;
  readDate   = b.readDate;
  }



//////////////-------------

//#include "qsort.h"
#if 0
void Books::clear() {
int i;

  for (i = 0; i < noRcds(); i++) rcds[i].clear();

  rcds.clear(); nextIndex = 1;
  }
#endif


#if 0
  r->title    = dlg.title;
  r->location = dlg.location;  r->location.trim();
  r->pubDate  = dlg.pubDate;   r->pubDate.trim();
  r->comment  = dlg.comment;   r->comment.trim();
  r->key      = r->title;
#endif
#if 0
  switch (perTypPos) {
    case AuthorPos  : fName = dlg.author;  lName = dlg.authorLast;  perTyp = AuthorTyp;
                      person = book ? persons.find(AuthorTyp, book->authorID) : 0; break;
    case Author2Pos : fName = dlg.author2; lName = dlg.author2Last; perTyp = AuthorTyp;
                      person = book ? persons.find(AuthorTyp, book->author2ID) : 0; break;
    case ProtagPos  : fName = dlg.protag;  lName = dlg.protagLast;  perTyp = ProtagTyp;
                      person = book ? persons.find(ProtagTyp, book->protagID) : 0; break;
    case Protag2Pos : fName = dlg.protag2; lName = dlg.protag2Last; perTyp = ProtagTyp;
                      person = book ? persons.find(ProtagTyp, book->protag2ID) : 0; break;
    }
  fName.trim(); lName.trim();

  if (fName.isEmpty() && lName.isEmpty()) return;

  if (!person) {
    person = persons.find(perTyp, fName, lName);
    if (!person) person = persons.add(perTyp, fName, lName);
    }

  else if (person->fName != fName || person->lName != lName) {
    EditOrDelRefDlg edlg;

    edlg.person = fName + _T(" ") + lName;

    int x = edlg.DoModal();

    if (x == IDC_EditPerson) {
      person->fName = fName; person->lName = lName;
      }
    else if (x == IDC_DeleteRef) {
      person->bookKeys.delRef(book->id);
      person = persons.add(perTyp, fName, lName);
      }
    }

  if (!book) {
    book = &books.add(dlg.title);
    book->location = dlg.location;  book->location.trim();
    book->pubDate  = dlg.pubDate;   book->pubDate.trim();
    book->comment  = dlg.comment;   book->comment.trim();
    }

  person->bookKeys.addRef(id, title);

  switch (perTypPos) {
    case Author : book->authorID  = person->id;   break;
    case Author2: book->author2ID = person->id;   break;
    case Protag : book->protagID  = person->id;   break;
    case Protag2: book->protag2ID = person->id;   break;
    }
#endif
#if 0
    x = rcds[noRcds()].load(ar);
#endif

//  aBkIO.write(authorIdP);
//  aBkIO.write(author2IdP);
//  aBkIO.write(protagIdP);
//  aBkIO.write(protag2IdP);
#if 0
//authorIdP  = b.authorIdP;
  author2IdP = b.author2IdP;
  protagIdP  = b.protagIdP;
  protag2IdP = b.protag2IdP;
#endif
#if 0
  aBkIO.read(authorIdP.id);  if (authorIdP.id)  perKeys.add(authorIdP.id);
  aBkIO.read(author2IdP.id); if (author2IdP.id) perKeys.add(author2IdP.id);
  aBkIO.read(protagIdP.id);  if (protagIdP.id)  perKeys.add(protagIdP.id);
  aBkIO.read(protag2IdP.id); if (protag2IdP.id) perKeys.add(protag2IdP.id);
#endif
#if 0
//  if (personID != authorIdP  && persons.display(authorIdP))  notePad << nCrlf;
  if (personID != author2IdP && persons.display(author2IdP)) notePad << nCrlf;
  if (personID != protagIdP  && persons.display(protagIdP))  notePad << nCrlf;
  if (personID != protag2IdP && persons.display(protag2IdP)) notePad << nCrlf;
#endif
//  if (!subOrd) notePad << nClrTabs << nSetRTab(3) << nSetTab(4) << nSetTab(12);
#if 0
#if 1
#else
  notePad << nClrTabs << nSetTab(4) << nSetTab(24) << nSetTab(44) << nSetTab(65);
#endif
void Book::display(int initTab, int dbVer) {

  notePad << nTab << id << nTab << _T("Title:  ") << title << nCrlf;

  perKeys.display();

  notePad << nClrTabs << nSetTab(4) << nSetTab(24) << nSetTab(44) << nSetTab(65);

  if (!location.isEmpty()) notePad << nTab << _T("Loc:  ")       << location;
  if (!pubDate.isEmpty())  notePad << nTab << _T("PubDate:  ")   << pubDate;
  if (!readDate.isEmpty()) notePad << nTab << _T("Read Date:  ") << readDate << nCrlf;
  if (!comment.isEmpty())  notePad << nTab << _T("Comment:  ")   << comment  << nCrlf;
  }
#endif
#if 0
//  notePad << nTab << authorIdP;
  notePad << nTab << author2IdP;
  notePad << nTab << protagIdP;
  notePad << nTab << protag2IdP;
#endif
#if 0
  //  persons.removeBookRef(authorIdP,  this);
  persons.removeBookRef(author2IdP, this);
  persons.removeBookRef(protagIdP,  this);
  persons.removeBookRef(protag2IdP, this);
#endif
#if 0
//  if (personID == authorIdP)  {authorIdP.clear();  return;}
  if (personID == author2IdP) {author2IdP.clear(); return;}
  if (personID == protagIdP)  {protagIdP.clear();  return;}
  if (personID == protag2IdP) {protag2IdP.clear(); return;}
#endif
#if 0
//  if (!verifyRef(AuthorTyp, authorIdP )) {authorIdP.clear();}
  if (!verifyRef(AuthorTyp, author2IdP)) {author2IdP.clear();}
  if (!verifyRef(ProtagTyp, protagIdP )) {protagIdP.clear();}
  if (!verifyRef(ProtagTyp, protag2IdP)) {protag2IdP.clear();}
#endif
#if 0
PerTyp  perTyp;
int     perID;
String  fName;
String  lName;
Person* person = 0;

  switch (perTypPos) {
//    case AuthorPos  : perTyp = AuthorTyp;   perID = authorIdP;
//                      fName = dlg.author;  lName = dlg.authorLast;   break;

    case Author2Pos : perTyp = AuthorTyp;   perID = author2IdP;
                      fName = dlg.author2; lName = dlg.author2Last;   break;

    case ProtagPos  : perTyp = ProtagTyp;   perID = protagIdP;
                      fName = dlg.protag;  lName = dlg.protagLast;    break;

    case Protag2Pos : perTyp = ProtagTyp;   perID = protag2IdP;
                      fName = dlg.protag2; lName = dlg.protag2Last;   break;
    }

  person = persons.update(perTyp, perID, fName, lName);

  person->bookKeys.addRef(this);

  switch (perTypPos) {
//  case AuthorPos  : authorIdP.clear();    authorIdP.id  = person->id;   break;
    case Author2Pos : author2IdP.clear();   author2IdP.id = person->id;   break;
    case ProtagPos  : protagIdP.clear();    protagIdP.id  = person->id;   break;
    case Protag2Pos : protag2IdP.clear();   protag2IdP.id = person->id;   break;
    }
#endif
#if 0
Book* Books::add(ABookDlg& dlg) {
Book* r = data.allocate();

  r->set(nextIndex);
  r->add(dlg);
  data = r;  return r;
  }


void Book::add(ABookDlg& dlg) {
Date dt;

  title    = dlg.title;     title.trim();
  location = dlg.location;  location.trim();
//  dt = dlg.pubDate;

  pubDate  = dlg.pubDate;             // dt.format(_T("%Y"));   pubDate.trim();
  comment  = dlg.comment;   comment.trim();
  key      = title;
  }
#endif


#if 0
void Books::del(Book& book) {
int     bookID = book.id;
BksIter iter(*this);
Book*   bk;

  book.removePersonRefs();

  for (bk = iter(); bk; bk = iter++) if (bk->id == bookID) {iter.remove(); return;}
  }
#endif
#if 0
void Books::removeRef(int bookID, int personID) {
BksIter iter(*this);
Book*   bk;

  for (bk = iter(); bk; bk = iter++) if (bk->id == bookID) {bk->removeRef(personID); return;}
  }
#endif
//void Book::removeRef(int personID) {perKeys.removeRef(personID);}


//void Book::removePersonRefs() {perKeys.removeBookRefs(this);}
#if 0
String Book::get(COleDateTime& oDT, TCchar* fmt) {
Date dt = oDT;
String s = dt.format(fmt);   return s.trim();
  }
#endif
#if 0
bool Book::verifyRef(PerTyp perTyp, int personID)
                                                {return !personID || persons.find(personID) != 0;}
#endif
#if 0
Book* Books::Books::find(TCchar* title) {
BksIter iter(*this);
Book*   bk;

  for (bk = iter(); bk; bk = iter++) if (bk->title == title) return bk;

  return 0;
  }
#endif
//enum PerTyp    {NilTyp, AuthorTyp, ProtagTyp};


