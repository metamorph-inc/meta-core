#!/bin/bash

# Copy the app.c files into place, making backups.

cp -f build/robostix-frodo/files/app/app.c build/robostix-frodo/files/app/app.c.bak
cp -f qint/RS.c build/robostix-frodo/files/app/app.c

cp -f build/gumstix-frodo/files/app/app.c build/gumstix-frodo/files/app/app.c.bak
cp -f qint/GS.c build/gumstix-frodo/files/app/app.c



