Setlocal EnableDelayedExpansion

set XME=CyPhyML.xme
set LEFT_R=r28719
set RIGHT_R=r28828

if not exist %GME_ROOT%\GME\Parser\xme_id2guid.py (echo Need SVN GME & exit /b 1)

git init . || exit /b !ERRORLEVEL!
c:\Python27\python %GME_ROOT%\GME\Parser\xme_id2guid.py %XME%.merge-left.%LEFT_R% || exit /b !ERRORLEVEL!
move /y %XME%.merge-left_guids.xme %XME%_guids.xme || exit /b !ERRORLEVEL!
git add %XME%_guids.xme || exit /b !ERRORLEVEL!
git commit -am "left" || exit /b !ERRORLEVEL!

git checkout -b working || exit /b !ERRORLEVEL!
c:\Python27\python %GME_ROOT%\GME\Parser\xme_id2guid.py %XME%.working || exit /b !ERRORLEVEL!
git commit -am "working" || exit /b !ERRORLEVEL!

git checkout master || exit /b !ERRORLEVEL!
git checkout -b right || exit /b !ERRORLEVEL!
c:\Python27\python %GME_ROOT%\GME\Parser\xme_id2guid.py %XME%.merge-right.%RIGHT_R% || exit /b !ERRORLEVEL!
move /y %XME%.merge-right_guids.xme %XME%_guids.xme || exit /b !ERRORLEVEL!
git commit -am "right" || exit /b !ERRORLEVEL!

git merge working
echo resolve conflict on %XME%_guids.xme: make new project/@guid
pause
git add %XME%_guids.xme || exit /b !ERRORLEVEL!
git commit -am "merged" || exit /b !ERRORLEVEL!

set PYTHONPATH=%GME_ROOT%\Tests\
c:\python27\python %GME_ROOT%\Tests\GPyUnit\util\gme.py xme2mga %XME%_guids.xme || exit /b !ERRORLEVEL!
c:\python27\python %GME_ROOT%\Tests\GPyUnit\util\gme.py mga2xme %XME%_guids.mga %XME% || exit /b !ERRORLEVEL!
set PYTHONPATH=
svn resolved %XME% || exit /b !ERRORLEVEL!
rd /s/q .git
rem git diff master
rem head -4 CyPhyML.xme
