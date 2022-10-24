// A Book IO class


#include "stdafx.h"
#include "ABookIO.h"
#include "MessageBox.h"


void ABookIO::write(String& s) {
int n = s.length();

  write(n);  if (!ar.write(s)) msg(_T("write String: "));
  }




void ABookIO::read(String& s) {
int n;

  read(n);   if (!ar.read(s, n)) msg(_T("read String: "));
  }


#if 0
void ABookIO::write(Byte v) {

  if (wBuf >= endBuf) {messageBox(_T("ABookIO buffer overflow")); return;}

  *wBuf++ = v;
  }


void ABookIO::write( Tchar ch) {
int    i;
int    n = sizeof(ch);
Cchar* p = (Cchar*) &ch;

  for (i = 0; i < n; i++) {
    if (wBuf >= endBuf) {messageBox(_T("ABookIO buffer overflow")); return;}
    *wBuf++ = *p++;
    }
  }


void ABookIO::write(int v) {
int    i;
int    n = sizeof(v);
Cchar* p = (Cchar*) &v;

  for (i = 0; i < n; i++) {
    if (wBuf >= endBuf) {messageBox(_T("ABookIO buffer overflow")); return;}
    *wBuf++ = *p++;
    }
  }
#endif

#if 0
void ABookIO::read(Byte& v) {
  if (ar.read(buf, 1) != 1) {messageBox(_T("ABookIO read int underflow")); return;}
  v = buf[0];
  }


void ABookIO::read(Tchar& ch) {
int    i;
int    n = sizeof(ch);
char*  p = (char*) &ch;

  if (!ar.read(buf, n)) {messageBox(_T("ABookIO read int underflow")); return;}

  for (i = 0, rBuf = buf; i < n; i++) *p++ = *rBuf++;
  }


void ABookIO::read(int& v) {
int    i;
int    n = sizeof(v);
char*  p = (char*) &v;

  if (!ar.read(buf, n)) {messageBox(_T("ABookIO read int underflow")); return;}

  for (i = 0, rBuf = buf; i < n; i++) *p++ = *rBuf++;
  }
#endif

#if 0
void ABookIO::write(Date& v) {
int    i;
int    n = sizeof(v);
Cchar* p = (Cchar*) &v;

  for (i = 0; i < n; i++) {
    if (wBuf >= endBuf) {messageBox(_T("ABookIO buffer overflow")); return;}
    *wBuf++ = *p++;
    }
  }


void ABookIO::read(Date& d) {
int    i;
int    n = sizeof(d);
char*  p = (char*) &d;

  if (!ar.read(buf, n)) {messageBox(_T("ABookIO read int underflow")); return;}

  for (i = 0, rBuf = buf; i < n; i++) *p++ = *rBuf++;
  }
#endif


#if 0
void ABookIO::write(String& s) {
int     i;
int     n = s.length();
TCchar* p = s;

  write(n);

  for (i = 0; i < n; i++) {
    if (wBuf >= endBuf) {messageBox(_T("ABookIO buffer overflow")); return;}
    *wBuf++ = (char) *p++;
    }
  }


void ABookIO::read(String& s) {
int     i;
int     n;

  read(n);   ar.read(buf, n);   s.clear();

  if (n > noElements(buf)) {messageBox(_T("ABookIO buffer overflow in read string")); return;}

  for (i = 0, rBuf = buf; i < n; i++) s += (Tchar) *rBuf++;
  }


void ABookIO::msg(TCchar* fn)
                        {String s = _T("ABookIO ");   s += fn;  s += ar.getLastError();  messageBox(s);}
#endif

