// A Book IO class


#include "pch.h"
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


