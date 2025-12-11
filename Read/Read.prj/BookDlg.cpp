// BookDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "BookDlg.h"
#include "PersonDlg.h"
#include "Resource.h"



// BookDlg dialog

IMPLEMENT_DYNAMIC(BookDlg, CDialogEx)


BookDlg::BookDlg(CWnd* pParent) : CDialogEx(IDD_Book, pParent)/*, comment(_T("")),
                                  pubDate(COleDateTime::GetCurrentTime()) ,
                                  readDate(COleDateTime::GetCurrentTime())*/ { }


BookDlg::~BookDlg() { }


BOOL BookDlg::OnInitDialog() {
CTime minDate(1970,1,1,12,0,0);
CTime maxDate(3000,12,31,0, 0, 0);
Date  dt = pubDate;

Date  minDt{1970,1,1,12,0,0};
Date  today;

  CDialogEx::OnInitDialog();

  loadCbx(author1Ctl, AuthorTyp, author1);   author1 = 0;
  loadCbx(author2Ctl, AuthorTyp, author2);   author2 = 0;

  loadCbx(protag1Ctl, ProtagTyp, protag1);   protag1 = 0;
  loadCbx(protag2Ctl, ProtagTyp, protag2);   protag2 = 0;

  pubDateCtl.SetRange(&minDate, &maxDate);
  pubDateCtl.SetFormat(_T("yyyy"));                                   //MM/dd/
  if (dt <= minDt) {pubDate.SetDate(2000, 1, 1);   pubDateCtl.SetTime(pubDate);}

  readDateCtl.SetRange(&minDate, &maxDate);
  readDateCtl.SetFormat(_T("MM/dd/yyyy"));                            // HH:mm

  return TRUE;
  }


void BookDlg::loadCbx(CComboBox& cbx, PerTyp pTyp, Person* per) {
PersIter iter(persons);
Person*  person;
String   s;
int      x;
int      dflt = 0;
String   fName;

  cbx.ResetContent();

  for (person = iter(); person; person = iter++) {
    if (person->perTyp == pTyp) {
#if 1
      x = addCbx(cbx, person);
#else
      s = person->lName;   fName = person->fName;

      if (!fName.isEmpty()) {if (!s.isEmpty())s += _T(", ");   s += fName;}

      x = cbx.AddString(s);    if (x >= 0) cbx.SetItemDataPtr(x, person);
#endif

      if (per == person) dflt = x;
      }
    }

  if (dflt) cbx.SetCurSel(dflt);
  }


int BookDlg::addCbx( CComboBox& cbx, Person* per) {
String s;
int    x;

  if (!per) return -1;

#if 1
  s = per->getLFname();
#else
String fName;
  s = per->lName;   fName = per->fName;

  if (!fName.isEmpty()) {if (!s.isEmpty())s += _T(", ");   s += fName;}
#endif
  x = cbx.AddString(s);    if (x >= 0) cbx.SetItemDataPtr(x, per);

  return x;
  }


void BookDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(        pDX, IDC_Title,    title);
    DDX_Control(     pDX, IDC_Author1,  author1Ctl);
    DDX_Control(     pDX, IDC_Author2,  author2Ctl);
    DDX_Control(     pDX, IDC_Protag1,  protag1Ctl);
    DDX_Control(     pDX, IDC_Protag2,  protag2Ctl);
    DDX_Text(        pDX, IDC_Location, location);
    DDX_Text(        pDX, IDC_Comment,  comment);
    DDX_DateTimeCtrl(pDX, IDC_PubDate,  pubDate);
    DDX_Control(     pDX, IDC_PubDate,  pubDateCtl);

    DDX_DateTimeCtrl(pDX, IDC_ReadDate, readDate);
    DDX_Control(     pDX, IDC_ReadDate, readDateCtl);
}

//DDX_Control(     pDX, IDC_Date,    dateCtl);
//DDX_DateTimeCtrl(pDX, IDC_Date,    date);


BEGIN_MESSAGE_MAP(BookDlg, CDialogEx)
  ON_BN_CLICKED(          IDC_NewAuth1,   &onNewAuth1)
  ON_BN_CLICKED(          IDC_NewAuth2,   &onNewAuth2)
  ON_BN_CLICKED(          IDC_NewProtag1, &onNewProtag1)
  ON_BN_CLICKED(          IDC_NewProtag2, &onNewProtag2)
  ON_CBN_SELCHANGE(       IDC_Author1,    &onChangeAuthor1)
  ON_CBN_SELCHANGE(       IDC_Author2,    &onChangeAuthor2)
  ON_CBN_SELCHANGE(       IDC_Protag1,    &onChangeProtag1)
  ON_CBN_SELCHANGE(       IDC_Protag2,    &onChangeProtag2)
  ON_NOTIFY(NM_KILLFOCUS, IDC_PubDate,    &onLeavePubDate)
  ON_NOTIFY(NM_KILLFOCUS, IDC_ReadDate,   &onLeaveReadDate)
END_MESSAGE_MAP()



void BookDlg::onChangeAuthor1() {
//int x = author1Ctl.GetCurSel();

//  if (x >= 0) author1 = (Person*) author1Ctl.GetItemDataPtr(x);
  author1 = (Person*) getDataPtr(author1Ctl);

  if (author1 == author2) {author2Ctl.SetCurSel(-1);   author2 = 0;}
  }


void BookDlg::onChangeAuthor2() {
//int x = author2Ctl.GetCurSel();

//  if (x >= 0) author2 = (Person*) author2Ctl.GetItemDataPtr(x);
  author2 = (Person*) getDataPtr(author2Ctl);

  if (author1 == author2) {author2Ctl.SetCurSel(-1);;   author2 = 0;}
  }



void BookDlg::onChangeProtag1() {
//int x = protag1Ctl.GetCurSel();

//  if (x >= 0) protag1 = (Person*) protag1Ctl.GetItemDataPtr(x);
  protag1 = (Person*) getDataPtr(protag1Ctl);

  if (protag1 == protag2) {protag2Ctl.SetCurSel(-1);   protag2 = 0;}
  }


void BookDlg::onChangeProtag2() {
//int x = protag2Ctl.GetCurSel();

//  if (x >= 0) protag2 = (Person*) protag2Ctl.GetItemDataPtr(x);

  protag2 = (Person*) getDataPtr(protag2Ctl);

  if (protag1 == protag2) {protag2Ctl.SetCurSel(-1);   protag2 = 0;}
  }


// BookDlg message handlers

void BookDlg::onNewAuth1() {
PersonDlg dlg;
int       x;

  if (dlg.DoModal() == IDOK) {
    author1 = persons.add(AuthorTyp, dlg.firstName, dlg.lastName);

    x = addCbx(author1Ctl, author1);   if (x >= 0) author1Ctl.SetCurSel(x);
    }
  }

void BookDlg::onNewAuth2() {
PersonDlg dlg;
int       x;

  if (dlg.DoModal() == IDOK) {
    author2 = persons.add(AuthorTyp, dlg.firstName, dlg.lastName);

    x = addCbx(author2Ctl, author2);   if (x >= 0) author2Ctl.SetCurSel(x);
    }
  }

void BookDlg::onNewProtag1() {
PersonDlg dlg;
int       x;

  if (dlg.DoModal() == IDOK) {
    protag1 = persons.add(ProtagTyp, dlg.firstName, dlg.lastName);

    x = addCbx(protag1Ctl, protag1);   if (x >= 0) protag1Ctl.SetCurSel(x);
    }
  }

void BookDlg::onNewProtag2() {
PersonDlg dlg;
int       x;

  if (dlg.DoModal() == IDOK) {
    protag2 = persons.add(ProtagTyp, dlg.firstName, dlg.lastName);

    x = addCbx(protag2Ctl, protag2);   if (x >= 0) protag2Ctl.SetCurSel(x);
    }
  }




void BookDlg::onLeavePubDate(NMHDR* pNMHDR, LRESULT* pResult)
                                                  {checkDate(pubDateCtl, pubDate);   *pResult = 0;}

void BookDlg::onLeaveReadDate(NMHDR* pNMHDR, LRESULT* pResult)
                                                {checkDate(readDateCtl, readDate);   *pResult = 0;}


void BookDlg::OnOK() {

  author1 = (Person*) getDataPtr(author1Ctl);
  author2 = (Person*) getDataPtr(author2Ctl);
  protag1 = (Person*) getDataPtr(protag1Ctl);
  protag2 = (Person*) getDataPtr(protag2Ctl);

  if (author1 == author2) {author2Ctl.SetCurSel(-1);   author2 = 0;}
  if (protag1 == protag2) {protag2Ctl.SetCurSel(-1);   protag2 = 0;}

  CDialogEx::OnOK();
  }


void* BookDlg::getDataPtr(CComboBox& cbx)
                            {int x = cbx.GetCurSel();   return x >= 0 ? cbx.GetItemDataPtr(x) : 0;}


bool BookDlg::checkDate(CDateTimeCtrl& ctl, COleDateTime& oleDate) {
Date dt;
Date today;

  ctl.GetTime(oleDate);    dt = oleDate;    today.getToday();

  if (dt <= today) return true;

  dt = today;   oleDate = dt;   ctl.SetTime(oleDate);   ctl.SetFocus();   return false;
  }

