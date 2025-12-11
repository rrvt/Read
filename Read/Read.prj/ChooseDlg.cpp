// Choose Record to edit or delete


#include "pch.h"
#include "ChooseDlg.h"
#include "Books.h"
#include "Persons.h"


// ChooseDlg dialog

IMPLEMENT_DYNAMIC(ChooseDlg, CDialogEx)


BEGIN_MESSAGE_MAP(ChooseDlg, CDialogEx)
  ON_BN_CLICKED(IDC_EditBook,   &onEditBook)
  ON_BN_CLICKED(IDC_EditAuthor, &onEditAuthor)
  ON_BN_CLICKED(IDC_EditProtag, &onEditProtag)
  ON_BN_CLICKED(IDC_DelBook,    &onDelBook)
  ON_BN_CLICKED(IDC_DelAuthor,  &onDelAuthor)
  ON_BN_CLICKED(IDC_DelProtag,  &onDelProtag)
END_MESSAGE_MAP()


ChooseDlg::ChooseDlg(CWnd* pParent) : CDialogEx(IDD_ChooseRcd, pParent) { }

ChooseDlg::~ChooseDlg() { }


BOOL ChooseDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  books.loadCbx(bookCtl);
  persons.loadCbx(AuthorTyp, authorCtl);
  persons.loadCbx(ProtagTyp, protagCtl);

  return TRUE;
  }


void ChooseDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_Book,   bookCtl);
  DDX_Control(pDX, IDC_Author, authorCtl);
  DDX_Control(pDX, IDC_Protag, protagCtl);
  }


// ChooseDlg message handlers

void ChooseDlg::onEditBook()   {book = (Book*) getDataPtr(bookCtl);    EndDialog(ID_EditBookRcd);}


void ChooseDlg::onEditAuthor()
                          {person = (Person*) getDataPtr(authorCtl);   EndDialog(ID_EditAuthRcd);}


void ChooseDlg::onEditProtag()
                          {person = (Person*) getDataPtr(protagCtl);   EndDialog(ID_EditPrtgRcd);}


void ChooseDlg::onDelBook()    {book = (Book*) getDataPtr(bookCtl);    EndDialog(ID_DelBookRcd);}


void ChooseDlg::onDelAuthor()
                          {person = (Person*) getDataPtr(authorCtl);   EndDialog(ID_DelAuthRcd);}


void ChooseDlg::onDelProtag()
                          {person = (Person*) getDataPtr(protagCtl);   EndDialog(ID_DelPrtgRcd);}
