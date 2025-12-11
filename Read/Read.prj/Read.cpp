// Read.cpp : Defines the class behaviors for the application.


#include "pch.h"
#include "Read.h"
#include "AboutDlg.h"
#include "Books.h"
#include "IniFileEx.h"
#include "MainFrame.h"
#include "NotePad.h"
#include "ReadDoc.h"
#include "ReadView.h"
#include "ResourceExtra.h"


Read      theApp;                         // The one and only Read object
IniFileEx iniFile(theApp);
Books     books;
Persons   persons;


// Read

BEGIN_MESSAGE_MAP(Read, CWinAppEx)

  ON_COMMAND(ID_Help,             &OnHelp)
  ON_COMMAND(ID_App_About,        &OnAppAbout)
  ON_COMMAND(ID_APP_EXIT,         &onAppExit)
END_MESSAGE_MAP()


// Read initialization

BOOL Read::InitInstance() {

  CWinAppEx::InitInstance();

  iniFile.setAppDataPath(m_pszHelpFilePath);

  notePad.clear();

  SetRegistryKey(appID);

  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views

  CSingleDocTemplate* pDocTemplate;

  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(ReadDoc),
    RUNTIME_CLASS(MainFrame),       // main SDI frame window
    RUNTIME_CLASS(ReadView));

  if (!pDocTemplate) return FALSE;

  AddDocTemplate(pDocTemplate);

  // Parse command line for standard shell commands, DDE, file open

  CCommandLineInfo cmdInfo;   ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.

  if (!ProcessShellCommand(cmdInfo)) return FALSE;

  setAppName(_T("Read")); setTitle(_T("Book List"));

  view()->setFont(_T("Arial"), 12.0);

  doc()->initialLoad();

  m_pMainWnd->ShowWindow(SW_SHOW);   m_pMainWnd->UpdateWindow();   return TRUE;
  }


int Read::ExitInstance() {notePad.~NotePad();   return CApp::ExitInstance();}


void Read::onAppExit() {

  doc()->saveData();   notePad.clear();   books.clear();   persons.clear();

  CWinApp::OnAppExit();
  }

void Read::OnHelp() {
String topic = m_pszHelpFilePath; topic += _T(">Introduction");

  ::HtmlHelp(m_pMainWnd->m_hWnd, topic,  HH_DISPLAY_TOC, 0);
  }


void Read::OnAppAbout() {AboutDlg aboutDlg; aboutDlg.DoModal();}

