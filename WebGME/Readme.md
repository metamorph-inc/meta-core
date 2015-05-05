# Getting started with WebCyPhy #

1. Install [nodejs](http://nodejs.org/download/)
2. Install [mongodb](http://www.mongodb.org/downloads) if you want to use a local database
3. Install META, and open the WebGME directory (C:\Users\Public\Documents\META Documents\WebGME)
4. Install dependencies listed in package.json 
 - Double-click on `install_script.cmd`
 - Using command line: `npm install`
5. Start MongoDB
 - Double-click on `launch_database.cmd`
 - Using command line: `"C:\Program Files\MongoDB 2.6 Standard\bin\mongod" --dbpath mongodatabase`
6. Start the server
 - Double-click on `launch_app.cmd` 
 - Using command line: `node app.js` or `npm start`
7. Open a web browser and visit [http://localhost:8855/extlib/node_modules/webgme-cyphy/src/client/](http://localhost:8855/extlib/node_modules/webgme-cyphy/src/client/) to open the CyPhy workspace manager UI
 - Chrome and Safari are used for testing and verification
 - Visit [localhost:8855](http://localhost:8855) for the default WebGME editor

# Notes #

## Database ##
- The default database name is `CyPhy` (`multi` is the default webgme database name)
- On Windows, use command line to start a database `"C:\Program Files\MongoDB 2.6 Standard\bin\mongod" --dbpath mongodatabase`, or double-click on `launch_database.cmd`

## App ##
- Start the WebGME server from command line using `node app.js` or `npm start`, or double-click on `launch_app.cmd`

### 2 Ways to import/view/edit projects ###
1. Visit [localhost:8855](http://localhost:8855) to open the default WebGME editor
 - Click on the first icon in the toolbar `Projects`
 - Create new project from file
 - Name the project as `ADMEditor`
 - Drag and drop or select the `meta/ADMEditor_metaOnly.json` file
 - After you see `Initializing tree ...` in the tree browser then refresh the page and reopen the project. 
2. To open the CyPhy workspace manager UI, visit [http://localhost:8855/extlib/node_modules/webgme-cyphy/src/client/](http://localhost:8855/extlib/node_modules/webgme-cyphy/src/client/)
 - The ADMEditor language will be imported automatically
 - Create a new workspace and name it MassSpringDamper
 - Open the `samples\MSD` directory (C:\Users\Public\Documents\META Documents\WebGME\samples\MSD)
 - Open the `components` directory and drag and drop all the files onto the newly created MassSpringDamper workspace
 - Go back to `samples\MSD` and drag and drop `MyMassSpringDamper.adm` onto the newly created workspace

_Compare Options 1 & 2 by opening the same workspace in adjacent tabs, and making changes in one to see the effects in the other._

## Utils ##
- Adding a full directory to the local blob `node add_dir_to_blob.js samples\RollingWheel`





