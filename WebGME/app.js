/**
 * Created by Zsolt on 4/4/2014.
 */

var config = require('./config.json'),
    webgme = require('webgme');

// updating default configuration with ours
webGMEGlobal.setConfig(config);

// standalone server uses webGMEGlobal.getConfig() if no configuration defined
var myServer = new webgme.standaloneServer();
myServer.start();