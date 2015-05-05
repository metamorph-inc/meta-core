echo off
where node
if %ERRORLEVEL% == 0 (
	node app.js
) else (
	echo ERROR: node is not in path! Make sure to install node and append it to Path.
	pause
)