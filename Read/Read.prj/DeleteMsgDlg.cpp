// DeleteMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DeleteMsgDlg.h"
#include "Resource.h"
#include "afxdialogex.h"


// DeleteMsgDlg dialog

IMPLEMENT_DYNAMIC(DeleteMsgDlg, CDialogEx)


DeleteMsgDlg::DeleteMsgDlg(CWnd* pParent) : CDialogEx(IDD_DeleteMsg, pParent), message(_T("")) { }


DeleteMsgDlg::~DeleteMsgDlg() { }

void DeleteMsgDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_DeleteMsg, message);
}


BEGIN_MESSAGE_MAP(DeleteMsgDlg, CDialogEx)
END_MESSAGE_MAP()


// DeleteMsgDlg message handlers
