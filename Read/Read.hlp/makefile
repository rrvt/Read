

Read.chm : *.htm Read.hhp Read.hhc App.css
  -1 "C:\Program Files (x86)\HTML Help Workshop\hhc" Read


Clean.cln    :
  @echo "Cleaning"
  cd $(MAKEDIR)
  del /F Read.chm


Release/Read.chm : Read.chm
  copy $(MAKEDIR)\Read.chm  $(MAKEDIR)\..\Release\Read.chm


Release/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Release
  del Read.chm
  cd $(MAKEDIR)


Debug/Read.chm : Read.chm
  copy $(MAKEDIR)\Read.chm  $(MAKEDIR)\..\Debug\Read.chm


Debug/Clean.cln    : Clean.cln
  cd $(MAKEDIR)\..\Debug
  del Read.chm
  cd $(MAKEDIR)


Release/Rebuild.rbd : Release/Clean.cln Release/Read.chm


Debug/Rebuild.rbd   : Debug/Clean.cln Debug/Read.chm


