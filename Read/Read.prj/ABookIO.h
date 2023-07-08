// A Book IO class


#pragma once
#include "Archive.h"
#include "Date.h"
#include "MessageBox.h"


class ABookIO {
Archive& ar;
                                          // rBuf(buf), wBuf(buf), endBuf(buf + noElements(buf))
public:

  ABookIO(Archive& ar0) : ar(ar0) {}
 ~ABookIO() {}

  void write( String& s);
  void write( Tchar  ch) {if (!ar.write(ch))            msg(_T("write Tchar: "));}
  void write( int     v) {if (!ar.write(&v, sizeof(v))) msg(_T("write int: "));}
  void write( Date&   d) {if (!ar.write(&d, sizeof(d))) msg(_T("write Date: "));}
  void write( Byte    v) {if (!ar.write(v))             msg(_T("write Byte: "));}

  void read(String&   s);
  void read(Tchar&   ch) {if (!ar.read(ch))             msg(_T("read Tchar: "));}
  void read(int&      v) {if (!ar.read(v))              msg(_T("read int: "));}
  void read(Date&     d) {int x = sizeof(d);  if (!ar.readBlk(&d, x))  msg(_T("read Date: "));}
  void read(Byte&     v) {if (!ar.read(v))              msg(_T("read Byte: "));}

private:

  void msg(TCchar* fn) {String s = _T("ABookIO ");   s += fn;   messageBox(s);}
  };

