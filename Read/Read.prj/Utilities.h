// Utility Functions not tied to any single class


#pragma once
#include "Date.h"



// notePad << offsets[] until offsets[x] is zero

void setTabs(int initTab, int offsets[]);

String titleKey(String& title);

Date&  getYYDate(TCchar* tc);
