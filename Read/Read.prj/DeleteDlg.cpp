// DeleteDlg dialog


#include "stdafx.h"
#include "DeleteDlg.h"
#include "Books.h"
#include "Resource.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(DeleteDlg, CDialogEx)


DeleteDlg::DeleteDlg(CWnd* pParent ) : CDialogEx(IDD_Delete, pParent),
                                tgtID(_T("")), targetTyp(Author), target(_T("")), person(0), book(0) {}


DeleteDlg::~DeleteDlg() { }


void DeleteDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Radio(  pDX, IDC_RADIO1, (int&) targetTyp);
  DDX_Text(   pDX, IDC_TgtID,         tgtID);
  DDX_Text(   pDX, IDC_Target,        target);
  DDX_Control(pDX, IDC_TgtID,         tgtIDCtrl);
  DDX_Control(pDX, IDC_Target,        targetCtrl);
  }


BEGIN_MESSAGE_MAP(DeleteDlg, CDialogEx)
  ON_BN_CLICKED(   IDC_RADIO1, &DeleteDlg::OnBnClickedRadio1)
  ON_BN_CLICKED(   IDC_RADIO2, &DeleteDlg::OnBnClickedRadio2)
  ON_BN_CLICKED(   IDC_RADIO3, &DeleteDlg::OnBnClickedRadio3)
  ON_EN_KILLFOCUS( IDC_TgtID,  &DeleteDlg::OnLeaveTgtID)
  ON_CBN_KILLFOCUS(IDC_Target, &DeleteDlg::OnLeaveTarget)
  ON_CBN_SELCHANGE(IDC_Target, &DeleteDlg::OnSelchangeOther)
END_MESSAGE_MAP()


void DeleteDlg::OnBnClickedRadio1() {
  targetTyp = Author;
  tgtIDCtrl.SetWindowText(_T(""));  targetCtrl.SetWindowText(_T(""));
  targetCtrl.ResetContent();
  }


void DeleteDlg::OnBnClickedRadio2() {
  targetTyp = Protag;
  tgtIDCtrl.SetWindowText(_T(""));  targetCtrl.SetWindowText(_T(""));
  targetCtrl.ResetContent();
  }


void DeleteDlg::OnBnClickedRadio3() {
  targetTyp = Title;
  tgtIDCtrl.SetWindowText(_T(""));  targetCtrl.SetWindowText(_T(""));
  targetCtrl.ResetContent();
  }


void DeleteDlg::OnLeaveTgtID() {
int      n;
Tchar    buf[12];

  person = 0; book = 0;

  n = tgtIDCtrl.GetLine(0, buf, noElements(buf));   if (!n) return;
  String s(buf, n);   load(s);
  }


void DeleteDlg::OnLeaveTarget() {
CString cs;
String  s;

  if (targetCtrl.GetCount()) return;

  person = 0; book = 0;

  targetCtrl.GetWindowText(cs);   if (cs.IsEmpty()) return;

  s = _T(".*") + cs + _T(".*");   RegExpr re(s);

  switch (targetTyp) {
    case Author : perToTgt(AuthorTyp, re); break;
    case Protag : perToTgt(ProtagTyp, re); break;
    case Title  : bookToTgt(re);           break;
    }

  if (targetCtrl.GetCount() == 1) OnSelchangeOther();
  }


void DeleteDlg::perToTgt(PerTyp perTyp, RegExpr& re) {
PersIter iter(persons);
Person*  person;

  targetCtrl.ResetContent();

  for (person = iter(); person; person = iter++) {
    if (person->perTyp != perTyp) continue;

    String s = person->fName + _T(" ") + person->lName;

    if (re.match(s)) {load(targetCtrl, person->id, s);}
    }

  targetCtrl.SetCurSel(0);
  }



void DeleteDlg::bookToTgt(RegExpr& re) {
Book*   book;

  targetCtrl.ResetContent();

  for (book = books.startLoop(); book; book = books.nextNode()) {

    if (re.match(book->title)) {load(targetCtrl, book->id, book->title);}
    }

  targetCtrl.SetCurSel(0);
  }


void DeleteDlg::OnSelchangeOther() {
int      i;
CString cs;
String   t;
String   id;
String   s;

  if (!targetCtrl.GetCount()) return;

  i = targetCtrl.GetCurSel();   targetCtrl.GetLBText(i, cs);   t = cs;

  id = t.substr(0, 4).trim();   tgtIDCtrl.SetWindowText(id);  load(id);
  }


void DeleteDlg::load(String idStg) {
int      id;
uint     x;
String   name;

  id = idStg.stoi(x);

  switch (targetTyp) {
    case Author : person = persons.find(AuthorTyp, id); break;
    case Protag : person = persons.find(ProtagTyp, id); break;
    case Title  : book   = books.find(id); break;
    }

  if (person) {
    name = person->fName + _T(" ") + person->lName;   targetCtrl.SetWindowText(name);  OnOK();
    }
  else if (book) {targetCtrl.SetWindowText(book->title); OnOK();}
  }


void DeleteDlg::load(CComboBox& cbx, int id, TCchar* s)
                                      {String t;   t.format(_T("%4i  %s"), id, s);    cbx.AddString(t);}

