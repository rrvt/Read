// PersonDlg.cpp : implementation file
//

#include "pch.h"
#include "Read.h"
#include "afxdialogex.h"
#include "PersonDlg.h"
#include "Resource.h"


// PersonDlg dialog

IMPLEMENT_DYNAMIC(PersonDlg, CDialogEx)


PersonDlg::PersonDlg(CWnd* pParent) : CDialogEx(IDD_Person, pParent) { }


PersonDlg::~PersonDlg() { }


void PersonDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_FirstName, firstName);
  DDX_Text(pDX, IDC_LastName,  lastName);
  }


BEGIN_MESSAGE_MAP(PersonDlg, CDialogEx)
END_MESSAGE_MAP()



