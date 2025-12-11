// Utility Functions not tied to any single class


#include "pch.h"
#include "Utilities.h"
#include "NotePad.h"


static void setTab(int tab, bool right = false);
static bool isNonWord(String& word);


void setTabs(int initTab, int offsets[]) {
bool right = initTab < 0;
int  tab   = abs(initTab);
int  nextTab;
int  i;

  notePad << nClrTabs;   setTab(tab, right);

  for (i = 0; offsets[i] > 0; i++)
              {nextTab = tab + abs(offsets[i]);   setTab(nextTab, offsets[i] < 0);  tab = nextTab;}
  }


void setTab(int tab, bool right) {
  if (!right) notePad << nSetTab(tab);
  else        notePad << nSetRTab(tab);
  }


String titleKey(String& title) {
int    pos  = title.find(_T(' '));   if (pos < 0) return title;
String word = title.substr(0, pos);

  if (isNonWord(word)) return title.substr(pos).trim();

  return title;
  }


static TCchar* nonWord[] = {_T("The"),
                            _T("A")
                            };

bool isNonWord(String& word) {
int i;
int n = noElements(nonWord);

  for (i = 0; i < n; i++) if (word == nonWord[i]) return true;

  return false;
  }


Date& getYYDate(TCchar* tc) {
String      s = tc;
int         yy;
uint        pos;
static Date dt;

  if (s.isEmpty()) return dt;

  if (s.find(_T('/')) < 0) {yy = s.stoi(pos);    Date d(yy, 1, 1, 0, 0, 0);  dt = d;   return dt;}

  Date d(s);   dt = d;   return dt;
  }

