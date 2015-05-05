echo off
if exist "C:\Program Files\MongoDB 2.6 Standard\bin" (
    "C:\Program Files\MongoDB 2.6 Standard\bin\mongod" --dbpath mongodatabase
exit
) else (
    echo ERROR: "C:\Program Files\MongoDB 2.6 Standard\bin\mongod" does not exist.
    echo Make sure to install mongodb. If it is already installed at different location
    echo please edit the path in this script.
    pause
)