// EditDlg.cpp : implementation file


#include "pch.h"
#include "EditDlg.h"
#include "Books.h"
#include "IniFileEx.h"
#include "Persons.h"
#include "resource.h"
#include "afxdialogex.h"


static TCchar* Section  = _T("Edit");
static TCchar* StateKey = _T("State");
static TCchar* IDkey    = _T("ID");


// EditDlg dialog

IMPLEMENT_DYNAMIC(EditDlg, CDialogEx)


EditDlg::EditDlg(CWnd* pParent) : CDialogEx(IDD_Edit, pParent),
                                  targetTyp(Author), targetID(_T("")),     target(_T("")),
                                  other(_T("")),     otherState(NilOther), person(0),
                                  book(0) { }


EditDlg::~EditDlg() { }


BOOL EditDlg::OnInitDialog() {

  targetTyp = (State) iniFile.readInt(Section, StateKey, 0);
  iniFile.readString(Section, IDkey,  targetID);

  CDialogEx::OnInitDialog();   load(targetID, false);   return TRUE;
  }


void EditDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);

  DDX_Radio(   pDX, IDC_RADIO1, (int&)targetTyp);
  DDX_Control( pDX, IDC_RADIO1, targetTypCtrl);
  DDX_Text(    pDX, IDC_TgtID,  targetID);
  DDX_Control( pDX, IDC_TgtID,  tgtIDCtrl);
  DDX_Control( pDX, IDC_Target, targetCtrl);
  DDX_CBString(pDX, IDC_Target, target);
  DDX_CBString(pDX, IDC_Other,  other);
  DDX_Control( pDX, IDC_Other,  otherCtrl);
  }


BEGIN_MESSAGE_MAP(EditDlg, CDialogEx)
  ON_BN_CLICKED(   IDC_RADIO1, &EditDlg::OnBnClickedRadio1)
  ON_BN_CLICKED(   IDC_RADIO2, &EditDlg::OnBnClickedRadio2)
  ON_BN_CLICKED(   IDC_RADIO3, &EditDlg::OnBnClickedRadio3)
  ON_EN_KILLFOCUS( IDC_TgtID,  &EditDlg::onLeaveID)
  ON_EN_KILLFOCUS( IDC_Target, &EditDlg::onLeaveTarget)
  ON_CBN_SELCHANGE(IDC_Other,  &EditDlg::OnSelchangeOther)
END_MESSAGE_MAP()


void EditDlg::OnBnClickedRadio1() {
  targetTyp = Author;
  tgtIDCtrl.SetWindowText(_T(""));  targetCtrl.SetWindowText(_T(""));
  otherCtrl.ResetContent();
  }


void EditDlg::OnBnClickedRadio2() {
  targetTyp = Protag;
  tgtIDCtrl.SetWindowText(_T(""));  targetCtrl.SetWindowText(_T(""));
  otherCtrl.ResetContent();
  }


void EditDlg::OnBnClickedRadio3() {
  targetTyp = Title;
  tgtIDCtrl.SetWindowText(_T(""));  targetCtrl.SetWindowText(_T(""));
  otherCtrl.ResetContent();
  }


void EditDlg::onLeaveID() {
int   n;
Tchar buf[12];

  n = tgtIDCtrl.GetLine(0, buf, noElements(buf));

  String s(buf, n);   load(s, true);
  }


void EditDlg::onLeaveTarget() {
CString cs;
String  s;

  targetCtrl.GetWindowText(cs);   s = _T(".*") + cs + _T(".*");   RegExpr re(s);

  otherState = TargetOther;

  switch (targetTyp) {
    case Author : perToTgt(AuthorTyp, re); break;
    case Protag : perToTgt(ProtagTyp, re); break;
    case Title  : bookToTgt(re);           break;
    }
  }


void EditDlg::perToTgt(PerTyp perTyp, RegExpr& re) {
PersIter iter(persons);
Person*  person;

  otherCtrl.ResetContent();

  for (person = iter(); person; person = iter++) {
    if (person->perTyp != perTyp) continue;

    String s = person->fName + _T(" ") + person->lName;

    if (re.match(s)) {load(otherCtrl, person->id, s);}
    }

  otherCtrl.SetCurSel(0);

  if (otherCtrl.GetCount() == 1) OnSelchangeOther();
  }


void EditDlg::bookToTgt(RegExpr& re) {
Book*   book;

  otherCtrl.ResetContent();

  for (book = books.startLoop(); book; book = books.nextNode()) {

    if (re.match(book->title)) {load(otherCtrl, book->id, book->title);}
    }

  otherCtrl.SetCurSel(0);
  }


void EditDlg::OnSelchangeOther() {
int      i;
CString cs;
String   t;
String   id;
String   s;

  if (otherState == TargetOther) {
    i = otherCtrl.GetCurSel();   otherCtrl.GetLBText(i, cs);   t = cs;

    id = t.substr(0, 4).trim();   tgtIDCtrl.SetWindowText(id);  load(id, true);
    }
  }


void EditDlg::load(String idStg, bool startEdit) {
uint     id;
uint     x;
String   name;

  id = idStg.stoi(x);   person = 0;  book = 0;

  switch (targetTyp) {
    case Author : person = persons.find(AuthorTyp, id); break;
    case Protag : person = persons.find(ProtagTyp, id); break;
    case Title  : book   = books.find(id); break;
    }

  otherCtrl.ResetContent();

  if (person) {
    name = person->fName + _T(" ") + person->lName;   targetCtrl.SetWindowText(name);

    BookKeys& bookKeys = person->bookKeys;
    BksIter   iter(bookKeys);
    BookKey*  bookKey;

    for (bookKey = iter(); bookKey; bookKey = iter++) {

      load(otherCtrl, bookKey->bookID, bookKey->title);
      }

    otherCtrl.SetCurSel(0);

    if (startEdit && otherState == TargetOther && otherCtrl.GetCount() == 1) OnOK();
    }

  else if (book) {targetCtrl.SetWindowText(book->title); if (startEdit) OnOK();}

  otherState = OtherOther;
  }


void EditDlg::load(CComboBox& cbx, int id, TCchar* s)
                                {String t;   t.format(_T("%4i  %s"), id, s);    cbx.AddString(t);}


void EditDlg::OnOK() {
uint x;

  CDialogEx::OnOK();

  iniFile.writeInt(   Section, StateKey, targetTyp);
  iniFile.writeString(Section, IDkey,    targetID);

  String s       = other;
  String otherID = s.substr(0, 4).trim();
  int    id      = otherID.stoi(x);

  switch (targetTyp) {
    case Author :
    case Protag : book = books.find(id); break;
    case Title  : break;
    }
  }

