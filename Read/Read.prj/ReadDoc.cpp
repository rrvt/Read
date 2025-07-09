 // ReadDoc.cpp : implementation of the ReadDoc class


#include "pch.h"
#include "ReadDoc.h"
#include "Books.h"
#include "ClipLine.h"
#include "DeleteDlg.h"
#include "DeleteMsgDlg.h"
#include "EditDlg.h"
#include "EditOrDelRefDlg.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "IniFileEx.h"
#include "MessageBox.h"
#include "NotePad.h"
#include "Resource.h"
#include "Read.h"
#include "ReadView.h"
#include "ResourceExtra.h"
#include "SearchDlg.h"
#include "MyToolBar.h"


static TCchar* FileSection = _T("A Book DB");
static TCchar* ABkDBKey    = _T("Path");


// ReadDoc

IMPLEMENT_DYNCREATE(ReadDoc, CDoc)

BEGIN_MESSAGE_MAP(ReadDoc, CDoc)

  ON_COMMAND(ID_OpenDatabase,  &onOpenDatabase)
  ON_COMMAND(ID_AddRecord,     &onAddRecord)
  ON_COMMAND(ID_Search,        &onSearch)
  ON_COMMAND(ID_Edit,          &onEdit)
  ON_COMMAND(ID_Delete,        &onDelete)

  ON_COMMAND(ID_DisplayAll,    &onDisplayAll)
  ON_COMMAND(ID_AuthorMenu,    &onDisplayAll)
  ON_COMMAND(ID_AuthorAB,      &onDisplay_AB)
  ON_COMMAND(ID_AuthorCD,      &onDisplay_CD)
  ON_COMMAND(ID_AuthorEF,      &onDisplay_EF)
  ON_COMMAND(ID_AuthorGH,      &onDisplay_GH)
  ON_COMMAND(ID_AuthorIJ,      &onDisplay_IJ)
  ON_COMMAND(ID_AuthorKL,      &onDisplay_KL)
  ON_COMMAND(ID_AuthorMN,      &onDisplay_MN)
  ON_COMMAND(ID_AuthorOP,      &onDisplay_OP)
  ON_COMMAND(ID_AuthorQR,      &onDisplay_QR)
  ON_COMMAND(ID_AuthorST,      &onDisplay_ST)
  ON_COMMAND(ID_AuthorUV,      &onDisplay_UV)
  ON_COMMAND(ID_AuthorWX,      &onDisplay_WX)
  ON_COMMAND(ID_AuthorYZ,      &onDisplay_YZ)

  ON_COMMAND(ID_FixIt,         &onFixIt)

  ON_COMMAND(ID_File_Save,     &OnFileSave)
  ON_COMMAND(ID_EDIT_COPY,     &onEditCopy)

END_MESSAGE_MAP()


// ReadDoc construction/destruction

ReadDoc::ReadDoc() noexcept : dataSource(NotePadSrc), dirty(false) {
  pathDlgDsc(_T("Book Database"), _T("ABookDB.bdb"), _T("bdb"), _T("*.bdb"));
  }


ReadDoc::~ReadDoc() { }


void ReadDoc::initialLoad() {

  iniFile.read(FileSection, ABkDBKey, path);

  if (path.isEmpty()) {setOpenPath(pathDlgDsc);   iniFile.write(FileSection, ABkDBKey, path);}

  loadDBfile(path);
  }



void ReadDoc::onOpenDatabase() {
  if (setOpenPath(pathDlgDsc)) {    loadDBfile(path);}
  }


void ReadDoc::loadDBfile(TCchar* path) {
String s;

  dataSource = StoreSrc;   notePad.clear();   persons.clear();   books.clear();

  OnOpenDocument(path);   dbPath = path;   dirty = false;

  updateBookKeys();   persons.sort();

  s = _T("Read -- A List of Books Read at ") + dbPath;

  theApp.setTitle(s);    onDisplayAll();
  }


void ReadDoc::updateBookKeys() {
PersIter iter(persons);
Person*  person;
Book*    book;

  for (person = iter(); person; person = iter++) {
    person->updateRefs();
    }
  for (book = books.startLoop(); book; book = books.nextNode()) {
    book->verifyRefs();
    }
  }


void ReadDoc::onAddRecord() {
ABookDlg aBookDlg;

  if (aBookDlg.DoModal() == IDOK) {
    Book* book = books.find(aBookDlg.title);

    updateEntries(Author,  aBookDlg, book);
    updateEntries(Author2, aBookDlg, book);
    updateEntries(Protag,  aBookDlg, book);
    updateEntries(Protag2, aBookDlg, book);
    }
onDisplayAll();
  }


void ReadDoc::updateEntries(PerTypPos perTypPos, ABookDlg& dlg, Book*& book) {
String  fName;
String  lName;
Person* person = 0;
PerTyp  perTyp;

  switch (perTypPos) {
    case Author : fName = dlg.author;  lName = dlg.authorLast;  perTyp = AuthorTyp;
                  person = book ? persons.find(AuthorTyp, book->authorID) : 0; break;
    case Author2: fName = dlg.author2; lName = dlg.author2Last; perTyp = AuthorTyp;
                  person = book ? persons.find(AuthorTyp, book->author2ID) : 0; break;
    case Protag : fName = dlg.protag;  lName = dlg.protagLast;  perTyp = ProtagTyp;
                  person = book ? persons.find(ProtagTyp, book->protagID) : 0; break;
    case Protag2: fName = dlg.protag2; lName = dlg.protag2Last; perTyp = ProtagTyp;
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

  person->bookKeys.addRef(book->id, book->title);

  switch (perTypPos) {
    case Author : book->authorID  = person->id; break;
    case Author2: book->author2ID = person->id; break;
    case Protag : book->protagID  = person->id; break;
    case Protag2: book->protag2ID = person->id; break;
    }

  dirty = true;
  }


void ReadDoc::onSearch() {
SearchDlg dlg;

  if (dlg.DoModal() == IDOK) {

    notePad.clear();

    switch (dlg.srchType) {
      case SearchDlg::Author: persons.display(AuthorTyp, dlg.target); break;
      case SearchDlg::Protag: persons.display(ProtagTyp, dlg.target); break;
      case SearchDlg::Title : books.display(dlg.target);              break;
      }
    }

  invalidate();
  }


void ReadDoc::onEdit()
                     {EditDlg dlg;   if (dlg.DoModal() == IDOK) {editEntry(dlg.person, dlg.book);}}


void ReadDoc::editEntry(Person* per, Book* book) {
ABookDlg  dlg;
Person*   person;
int       perID;
PerTypPos perTyp = NilTypPos;

  if (!book) return;

  perID = per ? per->id : 0;

  dlg.title       = book->title;
  person = persons.find(AuthorTyp, book->authorID);
  if (person) {
    dlg.author      = person->fName;
    dlg.authorLast  = person->lName;
    if (person->id == perID) perTyp = Author;
    }

  person = persons.find(AuthorTyp, book->author2ID);
  if (person) {
    dlg.author2     = person->fName;
    dlg.author2Last = person->lName;
    if (person->id == perID) perTyp = Author2;
    }

  person = persons.find(ProtagTyp, book->protagID);
  if (person) {
    dlg.protag      = person->fName;
    dlg.protagLast  = person->lName;
    if (person->id == perID) perTyp = Protag;
    }

  person = persons.find(ProtagTyp, book->protag2ID);
  if (person) {
    dlg.protag2     = person->fName;
    dlg.protag2Last = person->lName;
    if (person->id == perID) perTyp = Protag2;
    }
  dlg.location    = book->location;
  dlg.pubDate     = book->pubDate;
  dlg.comment     = book->comment;

  if (dlg.DoModal() == IDOK) {
    book->title    = dlg.title;
    book->location = dlg.location;
    book->pubDate  = dlg.pubDate;
    book->comment  = dlg.comment;

    switch (perTyp) {
      case Author : per->fName = dlg.author;  per->lName = dlg.authorLast;  break;
      case Author2: per->fName = dlg.author2; per->lName = dlg.author2Last; break;
      case Protag : per->fName = dlg.protag;  per->lName = dlg.protagLast;  break;
      case Protag2: per->fName = dlg.protag2; per->lName = dlg.protag2Last; break;
      }

    updateEntries(Author,  dlg, book);
    updateEntries(Author2, dlg, book);
    updateEntries(Protag,  dlg, book);
    updateEntries(Protag2, dlg, book);

    notePad.clear();   invalidate();
    }
  }


void ReadDoc::onDelete() {
DeleteDlg delDlg;
Person* person;
Book*   book;

  if (delDlg.DoModal() == IDOK) {

    person = delDlg.person;
    book   = delDlg.book;

    notePad.clear();

    switch (delDlg.targetTyp) {
      case DeleteDlg::Author:
      case DeleteDlg::Protag: if (person) delPerson(person); break;
      case DeleteDlg::Title : if (book)   delBook(book);     break;
      }

    invalidate();
    }
  }


static TCchar* pTyp[] = {_T("Author"), _T("Protagonist")};


void ReadDoc::delPerson(Person* person) {
DeleteMsgDlg dlg;
String       s;

  if (!person) return;

  s = _T("Delete ");
  s += pTyp[person->perTyp- BaseTyp];
  s += _T(": ") + person->fName + _T(" ") + person->lName + _T("?");

  dlg.message = s;
  if (dlg.DoModal() == IDOK) {
    persons.del(*person);   dirty = true;
//  person->display();
    }
  }


void ReadDoc::delBook(Book* book) {
DeleteMsgDlg dlg;
String       s;

  if (!book) return;

  s = _T("Delete ") + book->title;

  dlg.message = s;
  if (dlg.DoModal() == IDOK) {
    books.del(*book);   dirty = true;
    }
  }


void ReadDoc::onDisplayAll() {display(0);}
void ReadDoc::onDisplay_AB() {display(_T("AB"));}
void ReadDoc::onDisplay_CD() {display(_T("CD"));}
void ReadDoc::onDisplay_EF() {display(_T("EF"));}
void ReadDoc::onDisplay_GH() {display(_T("GH"));}
void ReadDoc::onDisplay_IJ() {display(_T("IJ"));}
void ReadDoc::onDisplay_KL() {display(_T("KL"));}
void ReadDoc::onDisplay_MN() {display(_T("MN"));}
void ReadDoc::onDisplay_OP() {display(_T("OP"));}
void ReadDoc::onDisplay_QR() {display(_T("QR"));}
void ReadDoc::onDisplay_ST() {display(_T("ST"));}
void ReadDoc::onDisplay_UV() {display(_T("UV"));}
void ReadDoc::onDisplay_WX() {display(_T("WX"));}
void ReadDoc::onDisplay_YZ() {display(_T("YZ"));}


void ReadDoc::display(TCchar* prefix) {
String   s;
RegExpr* re = 0;
PersIter iter(persons);
Person*  person;
bool     lineBrk = false;

  notePad.clear();   persons.sort();

  if (prefix) {
    s = prefix;

    if (!findFilePrefix(s, re)) return;
    }

  for (person = iter(); person; person = iter++) {

    if (person->bookKeys.isEmpty()) continue;

    if (prefix) {String initial = person->lName.substr(0, 1);   if (!re->match(initial)) continue;}

    if (lineBrk) notePad << nCrlf;   lineBrk = true;

    person->display();
    }

  display(NotePadSrc);
  }


static RegExpr prefixPat[] = {_T("[ABab]"),
                              _T("[CDcd]"),
                              _T("[EFef]"),
                              _T("[GHgh]"),
                              _T("[IJij]"),
                              _T("[KLkl]"),
                              _T("[MNmn]"),
                              _T("[OPop]"),
                              _T("[QRqr]"),
                              _T("[STst]"),
                              _T("[UVuv]"),
                              _T("[WXwx]"),
                              _T("[YZyz]")
                              };


bool ReadDoc::findFilePrefix(String& s, RegExpr*& re) {
String t;
int    i;

  if (s.empty()) return 0;
  t = s.substr(0, 1);

  for (i = 0; i < noElements(prefixPat); i++)
                                      if (prefixPat[i].match(t)) {re = &prefixPat[i]; return true;}
  return false;
  }


BOOL ReadDoc::OnNewDocument() {return CDocument::OnNewDocument();}


void ReadDoc::onEditCopy() {clipLine.load();}


void ReadDoc::display(DataSource ds) {dataSource = ds; invalidate();}


void ReadDoc::onFixIt() {
PersIter iter(persons);
Person*  person;

  for (person = iter(); person; person = iter++) {
    if (person->removeDuplicates()) dirty = true;
    if (person->verifyRefs())       dirty = true;
    }

  onDisplayAll();
  }


void ReadDoc::OnFileSave() {

  if (!dirty) return;

  persons.sort();   pathDlgDsc.name = dbPath;

  if (setSaveAsPath(pathDlgDsc)) {dataSource = StoreSrc; OnSaveDocument(path);}
  }


// UglyDoc serialization

void ReadDoc::serialize(Archive& ar) {

  if (ar.isStoring())
    switch(dataSource) {
      case NotePadSrc : ar << notePad; return;
      case StoreSrc   : writeDB(ar);         return;
      default         :                      return;
      }

  else
    switch(dataSource) {
      case StoreSrc : readDB(ar); return;
      default       : return;
      }
  }


static const int dbVersion = 1;

void ReadDoc::writeDB(Archive& ar) {
String ver = _T("Version: "); ver += dbVersion;  ver += _T('\n');
int    rcdTyp;

  ar.write(ver);

  for (
    rcdTyp = AuthorRcdTyp; rcdTyp < NoRcdTypes; rcdTyp++) {

    ar.wrtBin(rcdTyp);

    switch(rcdTyp) {
      case AuthorRcdTyp : persons.store(ar); break;
      case BookRcdTyp   : books.store(ar);   break;
      }
    }
  return;

}

void ReadDoc::readDB(Archive& ar) {
String line;    ar.read(line);
int    pos    = line.find(_T(' ')) + 1;
int    endPos = line.find(_T('\n'));
String v      = line.substr(pos, endPos - pos);
uint   x;
int    dbVer  = v.stoi(x);

  switch (dbVer) {
    case  1 : readDB_1(ar); break;
    case  2 : readDB_2(ar); break;
    default : break;
    }
  }


void ReadDoc::readDB_1(Archive& ar) {
int rcdTyp;
int rTyp = 0;

  for (rcdTyp = AuthorRcdTyp; rcdTyp < NoRcdTypes; rcdTyp++) {

    ar.readBin(rTyp);

    switch(rTyp) {
      case AuthorRcdTyp : persons.load(ar); break;
      case BookRcdTyp   : books.load(ar);   break;
      }
    }
  }


void ReadDoc::readDB_2(Archive& ar) {
  }


// ReadDoc diagnostics

#ifdef _DEBUG
void ReadDoc::AssertValid() const          {CDocument::AssertValid();}
void ReadDoc::Dump(CDumpContext& dc) const {CDocument::Dump(dc);}
#endif //_DEBUG

