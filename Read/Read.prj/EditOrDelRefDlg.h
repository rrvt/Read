// EditOrDelRefDlg dialog


#pragma once


class EditOrDelRefDlg : public CDialogEx {

  DECLARE_DYNAMIC(EditOrDelRefDlg)

public:
CString person;
CStatic personCtrl;

               EditOrDelRefDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual     ~EditOrDelRefDlg();

  virtual BOOL OnInitDialog();

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_EditOrDelRef };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedEditperson();
	afx_msg void OnClickedDeleteref();
};
