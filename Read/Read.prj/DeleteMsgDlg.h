// DeleteMsgDlg dialog


#pragma once


class DeleteMsgDlg : public CDialogEx {

  DECLARE_DYNAMIC(DeleteMsgDlg)

public:
CString message;

           DeleteMsgDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~DeleteMsgDlg();

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_DeleteMsg };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
