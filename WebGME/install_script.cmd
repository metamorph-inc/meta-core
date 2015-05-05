echo off
echo Installation began: > install.log
date /t >> install.log
echo %TIME% >> install.log
where npm >> install.log
where node >> install.log
if %ERRORLEVEL% == 0 (
    echo #### node -v #### >> install.log
    node -v >> install.log
    echo #### npm -v #### >> install.log
    npm -v >> install.log
    echo #### npm install #### >> install.log
	npm install >> install.log
    if %ERRORLEVEL% == 0 (
        echo #### npm list -json #### >> install.log
        npm list >> install.log
        if exist "C:\Program Files\MongoDB 2.6 Standard\bin" (
            echo To start a Mongo Database: double-click on `launch_database.cmd` or using the command "C:\Program Files\MongoDB 2.6 Standard\bin\mongod" --dbpath mongodatabase
            pause
        ) else (
            echo ERROR: "C:\Program Files\MongoDB 2.6 Standard\bin\mongod" does not exist. >> install.log
            echo ERROR: "C:\Program Files\MongoDB 2.6 Standard\bin\mongod" does not exist.
            echo Make sure to install mongodb. If it is already installed at different location
            echo please edit the path in this script.
            pause
        )
    ) else (
        echo ERROR: npm install failed. Check to make sure you have an internet connection.
    )
) else (
    echo ERROR: npm and/or node is not in path! Make sure to install node and select the npm option. >> install.log
	echo ERROR: npm and/or node is not in path! Make sure to install node and select the npm option.
	pause
)