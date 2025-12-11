 // ReadDoc.cpp : implementation of the ReadDoc class


#include "pch.h"
#include "ReadDoc.h"
#include "Books.h"
#include "ChooseDlg.h"
#include "ClipLine.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "IniFileEx.h"
#include "MessageBox.h"
#include "NotePad.h"
#include "PerKeys.h"
#include "Persons.h"
#include "Resource.h"
#include "Read.h"
#include "ReadView.h"
#include "ResourceExtra.h"
#include "SearchDlg.h"
#include "MyToolBar.h"


const int StoreVer = 4;


static TCchar* FileSection = _T("A Book DB");
static TCchar* ABkDBKey    = _T("Path");
static TCchar* DbgABkDBKey = _T("DbgPath");


// ReadDoc

IMPLEMENT_DYNCREATE(ReadDoc, CDoc)

BEGIN_MESSAGE_MAP(ReadDoc, CDoc)


  ON_COMMAND(ID_CreateDB,      &onCreateDB)
  ON_COMMAND(ID_OpenDatabase,  &onOpenDatabase)
  ON_COMMAND(ID_AddRecord,     &onAddRecord)
  ON_COMMAND(ID_Search,        &onSearch)
  ON_COMMAND(ID_ModifyRcd,     &onModifyRcd)

  ON_COMMAND(ID_DisplayMenu,   &onSortTitle)
  ON_COMMAND(ID_SortTitle,     &onSortTitle)
  ON_COMMAND(ID_SortDate,      &onSortDate)
  ON_COMMAND(ID_SortAuthor,    &onSortAuthor)
  ON_COMMAND(ID_DebugDsp,      &onDebugDsp)

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

ReadDoc::ReadDoc() noexcept {setVersion(StoreVer);}


ReadDoc::~ReadDoc() { }


void ReadDoc::initialLoad() {

#ifdef _DEBUG
  iniFile.read(FileSection, DbgABkDBKey, path);
#else
  iniFile.read(FileSection, ABkDBKey, path);
#endif

  if (!path.isEmpty() || getDBpath()) loadDBfile(path);
  }


void ReadDoc::onCreateDB() {

  if (createFileDlg(pathDlgDsc, path) && !path.isEmpty()) {saveData();   loadDBfile(path);}

  display();
  }


void ReadDoc::onOpenDatabase() {if (getDBpath()) {loadDBfile(path);}   display();}


void ReadDoc::loadDBfile(TCchar* path) {
String s;

  dataSource = StoreSrc;   notePad.clear();   persons.clear();   books.clear();   dirty = false;

  if (!OnOpenDocument(path)) return;

  dbPath = path;   dirty |= verifyRefs();   persons.sort();   displayData();
  }


bool ReadDoc::getDBpath() {
bool rslt;
#ifdef _DEBUG

  pathDlgDsc.name = ::getPath(theApp.m_pszHelpFilePath);

  rslt = setOpenPath(pathDlgDsc);   iniFile.write(FileSection, DbgABkDBKey, path);

#else

  rslt = setOpenPath(pathDlgDsc);   iniFile.write(FileSection, ABkDBKey, path);

#endif
  return rslt;
  }


void ReadDoc::onAddRecord() {dirty |= books.addRecord();   displayData();}


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


void ReadDoc::onModifyRcd() {
ChooseDlg dlg;

  switch (dlg.DoModal()) {
    case IDCANCEL:        break;
    case ID_EditBookRcd : if (dspModify(_T("Edit Book Rcd"), dlg.book))
                                                                      dirty |= dlg.book->editRcd();
                          break;
    case ID_EditAuthRcd : if (dspModify(_T("Edit Author Rcd"), dlg.person))
                                                                    dirty |= dlg.person->editRcd();
                          break;
    case ID_EditPrtgRcd : if (dspModify(_T("Edit Protag Rcd"), dlg.person))
                                                                    dirty |= dlg.person->editRcd();
                          break;

    case ID_DelBookRcd  : if (dspModify(_T("Delete Book Rcd"), dlg.book))
                                                                dirty |= books.delRecord(dlg.book);
                          break;
    case ID_DelAuthRcd  : if (dspModify(_T("Delete Author Rcd"), dlg.person))
                                                            dirty |= persons.delRecord(dlg.person);

                          break;
    case ID_DelPrtgRcd  : if (dspModify(_T("Delete Protag Rcd"), dlg.person))
                                                            dirty |= persons.delRecord(dlg.person);
    default             : break;
    }

  displayCur();
  }


bool ReadDoc::dspModify(TCchar* prefix, Book* book) {
String s = prefix;
  s += _T(":  ");   s += book->title;  return msgYesNoBox(s) == IDYES;
  }


bool ReadDoc::dspModify(TCchar* prefix, Person* person) {
String s = prefix;
  s += _T(":  ");   s += person->getFLname();   return msgYesNoBox(s) == IDYES;
  }


void ReadDoc::displayData() {
String s = _T("A List of Books Read at ") + dbPath;   theApp.setTitle(s);

  onSortTitle();
  }


void ReadDoc::displayCur() {
  switch (dspOption) {
    case NilDsp   :
    case TitleDsp : onSortTitle();      break;
    case DateDsp  : onSortDate();       break;
    case AuthDsp  : onSortAuthor();     break;
    case PersonDsp: display(dspPrefix); break;
    case AllPerDsp: display(0);         break;
    case DebugDsp : onDebugDsp();       break;
    }
  }


void ReadDoc::onSortTitle()  {displayBk(TitleDsp);}
void ReadDoc::onSortDate()   {displayBk(DateDsp);}
void ReadDoc::onSortAuthor() {displayBk(AuthDsp);}
void ReadDoc::onDebugDsp()   {displayBk(DebugDsp);}

void ReadDoc::displayBk(DspOptions opt) {

  dspOption = opt;  notePad.clear();   displayDB();

  books.display(opt);

  if (opt == DebugDsp) {notePad << nCrlf;   persons.display(opt);}

  display();
  }


void ReadDoc::onDisplayAll() {display(0);}
void ReadDoc::onDisplay_AB() {dspPrefix = _T("AB"); display(dspPrefix);}
void ReadDoc::onDisplay_CD() {dspPrefix = _T("CD"); display(dspPrefix);}
void ReadDoc::onDisplay_EF() {dspPrefix = _T("EF"); display(dspPrefix);}
void ReadDoc::onDisplay_GH() {dspPrefix = _T("GH"); display(dspPrefix);}
void ReadDoc::onDisplay_IJ() {dspPrefix = _T("IJ"); display(dspPrefix);}
void ReadDoc::onDisplay_KL() {dspPrefix = _T("KL"); display(dspPrefix);}
void ReadDoc::onDisplay_MN() {dspPrefix = _T("MN"); display(dspPrefix);}
void ReadDoc::onDisplay_OP() {dspPrefix = _T("OP"); display(dspPrefix);}
void ReadDoc::onDisplay_QR() {dspPrefix = _T("QR"); display(dspPrefix);}
void ReadDoc::onDisplay_ST() {dspPrefix = _T("ST"); display(dspPrefix);}
void ReadDoc::onDisplay_UV() {dspPrefix = _T("UV"); display(dspPrefix);}
void ReadDoc::onDisplay_WX() {dspPrefix = _T("WX"); display(dspPrefix);}
void ReadDoc::onDisplay_YZ() {dspPrefix = _T("YZ"); display(dspPrefix);}


void ReadDoc::display(TCchar* prefix) {
RegExpr* re = 0;
PersIter iter(persons);
Person*  person;
bool     lineBrk = false;

  notePad.clear();   displayDB();    //persons.sort();

  dspOption = prefix ? PersonDsp : AllPerDsp;

  if (prefix) {dspPrefix = prefix;   if (!findFilePrefix(dspPrefix, re)) return;}

  for (person = iter(); person; person = iter++) {

    if (prefix) {String initial = person->lName.substr(0, 1);   if (!re->match(initial)) continue;}

    if (lineBrk) notePad << nCrlf;   lineBrk = true;

    person->display();
    }

  display(NotePadSrc);
  }


void ReadDoc::displayDB() {notePad << _T("Database: ") << dbPath << nCrlf << nCrlf;}


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


void ReadDoc::onEditCopy() {clipLine.load();}


void ReadDoc::display(DataSource ds) {dataSource = ds; invalidate();}


void ReadDoc::onFixIt() {
PersIter iter(persons);
//Person*  person;

  dirty |= persons.mergeDuplicates();
#if 0
  for (person = iter(); person; person = iter++) {
    if (person->removeDuplicates()) dirty = true;
    if (person->verifyRefs())       dirty = true;
    }
#endif
  notePad.clear();   persons.dbgDsp();   display();
//  onDisplayAll();
  }


bool ReadDoc::verifyRefs() {                        // LoadDB
BksIter iter(books);
Book*   bk;
bool    dirty = false;

  for (bk = iter(); bk; bk = iter++) dirty |= bk->verifyRefs();

  return dirty;
  }


void ReadDoc::OnFileSave() {

  persons.sort();   pathDlgDsc.name = dbPath;

  if (setSaveAsPath(pathDlgDsc)) {backupFile(10); dataSource = StoreSrc; OnSaveDocument(path);}
  }


void ReadDoc::saveData() {
DataSource stk = dataSource;

  if (!dirty) return;

  if (!dbPath.isEmpty()) {backupFile(10);   dataSource = StoreSrc;    OnSaveDocument(dbPath);}

  dataSource = stk;
  }


void ReadDoc::dspDirty() {
String d = dirty ? _T("true") : _T("false");
String s = _T("Dirty is ") + d;

  messageBox(s);
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



void ReadDoc::writeDB(Archive& ar) {
String ver = _T("Version: "); ver += StoreVer;  ver += _T('\n');
int    rcdTyp;

  setVersion(StoreVer);   ar.write(ver);

  for (rcdTyp = AuthorRcdTyp; rcdTyp < NoRcdTypes; rcdTyp++) {

    ar.wrtBin(rcdTyp);

    switch(rcdTyp) {
      case AuthorRcdTyp : persons.store(ar); break;
      case BookRcdTyp   : books.store(ar);   break;
      }
    }
  return;
  }


void ReadDoc::readDB(Archive& ar) {
String line;    if (!ar.read(line) || line.isEmpty()) return;
int    pos    = line.find(_T(' ')) + 1;
int    endPos = line.find(_T('\n'));
String v      = line.substr(pos, endPos - pos);
uint   x;
int    rcdTyp;
int    rTyp = 0;

  dbVer  = v.stoi(x);   setVersion(dbVer);    dirty = dbVer != StoreVer;

  for (rcdTyp = AuthorRcdTyp; rcdTyp < NoRcdTypes; rcdTyp++) {

    if (!ar.readBin(rTyp)) return;

    switch(rTyp) {
      case AuthorRcdTyp : persons.load(ar);   break;
      case BookRcdTyp   : books.load(ar);     break;
      }
    }
  }


void ReadDoc::setVersion(int ver) {books.setVersion(ver);   persons.setVersion(ver);}



// ReadDoc diagnostics

#ifdef _DEBUG
void ReadDoc::AssertValid() const          {CDocument::AssertValid();}
void ReadDoc::Dump(CDumpContext& dc) const {CDocument::Dump(dc);}
#endif //_DEBUG



#if 0
///////----------------

Book*    book;
  for (book = books.startLoop(); book; book = books.nextNode()) {
    book->verifyRefs();
    }
#endif
//  persons.resolveIDs(ar);   display();
#if 0
PersIter iterP(persons);
Person*  person;

  for (person = iterP(); person; person = iterP++) person->updateRefs();
#endif

#if 1
#else
ABookDlg aBookDlg;
Book*    book;

  if (aBookDlg.DoModal() == IDOK) {

    if (aBookDlg.title.IsEmpty()) return;

    book = books.find(aBookDlg.title);   if (!book) {book = books.add(aBookDlg);}
#if 1
#else
    updateEntries(Author,  aBookDlg, book);
    updateEntries(Author2, aBookDlg, book);
    updateEntries(Protag,  aBookDlg, book);
    updateEntries(Protag2, aBookDlg, book);
#endif
    }
#endif
//#include "DeleteDlg.h"
//#include "EditDlg.h"
//#include "EditOrDelRefDlg.h"
#if 0
void ReadDoc::updateEntries(PerTypPos perTypPos, ABookDlg& dlg, Book*& book) {
String  fName;
String  lName;
Person* person = 0;
PerTyp  perTyp;

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

  person->bookKeys.addRef(book->id, book->title);

  switch (perTypPos) {
    case Author : book->authorID  = person->id; break;
    case Author2: book->author2ID = person->id; break;
    case Protag : book->protagID  = person->id; break;
    case Protag2: book->protag2ID = person->id; break;
    }

  dirty = true;
  }
#endif
//#include "DeleteMsgDlg.h"
#if 0
void ReadDoc::delPerson(Person* person) {
#if 1
#else
DeleteMsgDlg dlg;
String       s;

  if (!person) return;

  s = _T("Delete ");
  s += pTyp[person->perTyp];
  s += _T(": ") + person->fName + _T(" ") + person->lName + _T("?");

  dlg.message = s;

  if (dlg.DoModal() == IDOK) {persons.del(*person);   dirty = true;}
#endif
  }


void ReadDoc::delBook(Book* book) {
#if 1
#else
DeleteMsgDlg dlg;
String       s;

  if (!book) return;

  s = _T("Delete ") + book->title;

  dlg.message = s;
  if (dlg.DoModal() == IDOK) {
    books.del(*book);   dirty = true;
    }
#endif
  }
#endif
//ON_COMMAND(ID_Delete,        &onDelete)


#if 0
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
#endif
#if 0
                     {EditDlg dlg;   if (dlg.DoModal() == IDOK) {editEntry(dlg.person, dlg.book);}}


void ReadDoc::editEntry(Person* per, Book* book) {
BookDlg    dlg;
PerKysIter iter(book->perKeys);
PerKey*    pk;
Person*    person;
int        perID;
PerTypPos  perTyp = NilPos;

  if (!book) return;

  perID = per ? per->id : 0;

  dlg.title       = book->title;

  pk = iter();   person = (*pk)();              //  person = persons.find(book->authorIdP);
  if (person) {
    dlg.author      = person->fName;
    dlg.authorLast  = person->lName;
    if (person->id == perID) perTyp = AuthorPos;
    }

  pk = iter++;   person = (*pk)();              //  person = persons.find(book->author2IdP);
  if (person) {
    dlg.author2     = person->fName;
    dlg.author2Last = person->lName;
    if (person->id == perID) perTyp = Author2Pos;
    }

  pk = iter++;   person = (*pk)();              //  person = persons.find(book->protagIdP);
  if (person) {
    dlg.protag      = person->fName;
    dlg.protagLast  = person->lName;
    if (person->id == perID) perTyp = ProtagPos;
    }

  pk = iter++;   person = (*pk)();              //  person = persons.find(book->protag2IdP);
  if (person) {
    dlg.protag2     = person->fName;
    dlg.protag2Last = person->lName;
    if (person->id == perID) perTyp = Protag2Pos;
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
      case AuthorPos  : per->fName = dlg.author;  per->lName = dlg.authorLast;  break;
      case Author2Pos : per->fName = dlg.author2; per->lName = dlg.author2Last; break;
      case ProtagPos  : per->fName = dlg.protag;  per->lName = dlg.protagLast;  break;
      case Protag2Pos : per->fName = dlg.protag2; per->lName = dlg.protag2Last; break;
      }

    book->updateEntry(AuthorPos,  dlg);
    book->updateEntry(Author2Pos, dlg);
    book->updateEntry(ProtagPos,  dlg);
    book->updateEntry(Protag2Pos, dlg);

    notePad.clear();   invalidate();
    }
  }
#endif


//static TCchar* pTyp[] = {_T(""), _T("Author"), _T("Protagonist")};
#if 1
#else
  dspOption = DebugDsp;   notePad.clear();

  notePad << _T("Database: ") << dbPath << nCrlf << nCrlf;

  books.display(dspOption, dbVer);   notePad << nCrlf;   persons.display(dspOption, dbVer);

  display();
#endif

