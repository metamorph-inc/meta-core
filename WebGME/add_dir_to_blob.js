/**
 * Created by Zsolt on 5/28/2014.
 *
 * Adds full directory with all subdirectories to the blob-local-storage
 *
 * node add_dir_to_blob.js samples\RollingWheel
 *
 */

if (process.argv.length < 3) {
    console.error('Usage: node add_dir_to_blob.js start_directory_path');
    console.error('       run this from the root directory, where blob-local-storage is located.');
    return;
}

var requirejs = require('requirejs');

var config = require('./config.json'),
    webgme = require('webgme'),
    fs = require('fs'),
    path = require('path');

var startDir = path.resolve(process.argv[2]);

// updating default configuration with ours
webGMEGlobal.setConfig(config);

var BlobFSBackend = requirejs('blob/BlobFSBackend');
var BlobRunPluginClient = requirejs('blob/BlobRunPluginClient');

// FIXME: should we use HTTP here?
var blobBackend = new BlobFSBackend();
var blobClient = new BlobRunPluginClient(blobBackend);

var walk = function(dir, done) {
    var results = [];
    fs.readdir(dir, function(err, list) {
        if (err) return done(err);
        var i = 0;
        (function next() {
            var file = list[i++];
            if (!file) return done(null, results);
            file = dir + '/' + file;
            fs.stat(file, function(err, stat) {
                if (stat && stat.isDirectory()) {
                    walk(file, function(err, res) {
                        results = results.concat(res);
                        next();
                    });
                } else {
                    results.push(file);
                    next();
                }
            });
        })();
    });
};


walk(startDir, function (err, files) {
    if (err) {
        console.error(err);
        return;
    }

    console.log('Directory ' + startDir + ' has ' + files.length + ' files.');

    var artifactName = path.basename(startDir);

    var resultArtifact = blobClient.createArtifact(artifactName);

    if (files.length === 0) {
        console.log('No files in directory.');
        return;
    }

    console.log('Adding files ...');

    var remaining = files.length;

    for (var i = 0; i < files.length; i += 1) {


        var filename = path.relative(startDir, files[i]).replace(/\\/g,'/');

        resultArtifact.addFileAsSoftLink(filename, fs.createReadStream(files[i]), function (err, hash) {
            remaining -= 1;

            if (err) {
                console.error(err);
                return;
            }

            if (remaining === 0) {
                resultArtifact.save(function (err, artifactHash) {
                    if (err) {
                        console.error(err);
                        return;
                    }

                    console.log('All files were added successfully.');

                    var port = webGMEGlobal.getConfig().port;

                    console.log(artifactName + ' - localhost:' + port);
                    console.log(' - metadata: localhost:' + port + blobClient.getMetadataURL(artifactHash));
                    console.log(' - download: localhost:' + port + blobClient.getDownloadURL(artifactHash));

                });
            }
        });
    }

});
