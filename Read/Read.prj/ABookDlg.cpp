// ABookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ABookDlg.h"
#include "Resource.h"


// ABookDlg dialog

IMPLEMENT_DYNAMIC(ABookDlg, CDialogEx)

ABookDlg::ABookDlg(CWnd* pParent) : CDialogEx(IDD_Abook, pParent), title(_T("")), author(_T("")),
            pubDate(_T("")), authorID(_T("")), author2ID(_T("")), protagID(_T("")), protag2ID(_T("")) { }

ABookDlg::~ABookDlg() { }


void ABookDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);

  DDX_Text(   pDX, IDC_Title,      title);

  DDX_Text(   pDX, IDC_AuthorID,   authorID);
  DDX_Text(   pDX, IDC_FirstName,  author);
  DDX_Text(   pDX, IDC_LastName,   authorLast);

  DDX_Text(   pDX, IDC_Aurthor2ID, author2ID);
  DDX_Text(   pDX, IDC_FirstName2, author2);
  DDX_Text(   pDX, IDC_LastName2,  author2Last);

  DDX_Text(   pDX, IDC_ProtagID,   protagID);
  DDX_Text(   pDX, IDC_PrtgFirstNm,      protag);
  DDX_Text(   pDX, IDC_PrtgLastNm,      protagLast);

  DDX_Text(   pDX, IDC_Protag2ID,  protag2ID);
  DDX_Text(   pDX, IDC_Prtg2FirstNm,      protag2);
  DDX_Text(   pDX, IDC_Prtg2LastNm,      protag2Last);

  DDX_Text(   pDX, IDC_Location,      location);
  DDX_Text(   pDX, IDC_PubDate,     pubDate);
  DDX_Text(   pDX, IDC_Comment,     comment);

  DDX_Control(pDX, IDC_AuthorID,     authorIDctrl);
  DDX_Control(pDX, IDC_FirstName,    authFNctrl);
  DDX_Control(pDX, IDC_LastName,     authLNctrl);
  DDX_Control(pDX, IDC_Aurthor2ID,   author2IDctrl);
  DDX_Control(pDX, IDC_FirstName2,   auth2FNctrl);
  DDX_Control(pDX, IDC_LastName2,    auth2LNctrl);
  DDX_Control(pDX, IDC_ProtagID,     protagIDctrl);
  DDX_Control(pDX, IDC_PrtgFirstNm,  prtgFNctrl);
  DDX_Control(pDX, IDC_PrtgLastNm,   prtgLNctrl);
  DDX_Control(pDX, IDC_Protag2ID,    protag2IDctrl);
  DDX_Control(pDX, IDC_Prtg2FirstNm, prtg2FNctrl);
  DDX_Control(pDX, IDC_Prtg2LastNm,  prtg2LNctrl);
  }


BEGIN_MESSAGE_MAP(ABookDlg, CDialogEx)
  ON_EN_KILLFOCUS(IDC_AuthorID,   &ABookDlg::OnLeaveAuthorID)
  ON_EN_KILLFOCUS(IDC_Aurthor2ID, &ABookDlg::OnLeaveAurthor2ID)
  ON_EN_KILLFOCUS(IDC_ProtagID,   &ABookDlg::OnLeaveProtagID)
  ON_EN_KILLFOCUS(IDC_Protag2ID,  &ABookDlg::OnLeaveProtag2ID)
END_MESSAGE_MAP()



void ABookDlg::OnLeaveAuthorID() {  load(AuthorTyp, authorIDctrl,  authFNctrl,  authLNctrl);}
void ABookDlg::OnLeaveAurthor2ID() {load(AuthorTyp, author2IDctrl, auth2FNctrl, auth2LNctrl);}
void ABookDlg::OnLeaveProtagID() {  load(ProtagTyp, protagIDctrl,  prtgFNctrl,  prtgLNctrl);}
void ABookDlg::OnLeaveProtag2ID() { load(ProtagTyp, protag2IDctrl, prtg2FNctrl, prtg2LNctrl);}


void ABookDlg::load(PerTyp typ, CEdit& idCtrl, CEdit& fnCtrl, CEdit& lnCtrl) {
int      n;
Tchar    buf[12];
int      id;
uint     x;
Person*  person;

  n = idCtrl.GetLine(0, buf, noElements(buf));   if (!n) return;

  String s(buf, n);    id = s.stoi(x);

  person = persons.find(typ, id);

  if (person) {fnCtrl.SetWindowText(person->fName);   lnCtrl.SetWindowText(person->lName);}
  }

