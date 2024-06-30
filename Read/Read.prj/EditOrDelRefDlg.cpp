// EditOrDelRefDlg.cpp : implementation file
//

#include "pch.h"
#include "EditOrDelRefDlg.h"
#include "resource.h"
#include "afxdialogex.h"


// EditOrDelRefDlg dialog

IMPLEMENT_DYNAMIC(EditOrDelRefDlg, CDialogEx)

EditOrDelRefDlg::EditOrDelRefDlg(CWnd* pParent) : CDialogEx(IDD_EditOrDelRef, pParent),
                                                                               person(_T("")) {  }


EditOrDelRefDlg::~EditOrDelRefDlg() { }


BOOL EditOrDelRefDlg::OnInitDialog() {CDialogEx::OnInitDialog();   return TRUE;}


void EditOrDelRefDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_Person, personCtrl);
  DDX_Text(   pDX, IDC_Person, person);
  }


BEGIN_MESSAGE_MAP(EditOrDelRefDlg, CDialogEx)
  ON_BN_CLICKED(IDC_EditPerson, &EditOrDelRefDlg::OnClickedEditperson)
  ON_BN_CLICKED(IDC_DeleteRef,  &EditOrDelRefDlg::OnClickedDeleteref)
END_MESSAGE_MAP()


// EditOrDelRefDlg message handlers


void EditOrDelRefDlg::OnClickedEditperson() {EndDialog(IDC_EditPerson);}


void EditOrDelRefDlg::OnClickedDeleteref() {EndDialog(IDC_DeleteRef);}
