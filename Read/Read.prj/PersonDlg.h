// Person Dialog


#pragma once


// PersonDlg dialog

class PersonDlg : public CDialogEx {

  DECLARE_DYNAMIC(PersonDlg)

public:

Cstring firstName;
Cstring lastName;

           PersonDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~PersonDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Person };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
