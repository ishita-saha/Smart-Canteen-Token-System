@echo off

echo ==========================================
echo SMART CANTEEN - MEMBER 1 GITHUB PUSH
echo ==========================================

echo.
echo Adding member1.cpp...
git add member1.cpp

echo.
echo Creating commit...
git commit -m "Add Member 1 student and order module"

echo.
echo Pushing to GitHub...
git push origin main

echo.
echo ==========================================
echo PUSH COMPLETED
echo ==========================================

pause