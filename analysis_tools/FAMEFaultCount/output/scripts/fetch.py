import os, sys, urllib, urlparse, tempfile, zipfile, shutil

def fetch_and_unpack_zip_file(file_uri, output_directory):
    if not os.path.exists(output_directory) or not os.path.isdir(output_directory):
        raise InvalidValue("output directory %s does not exist or is not a directory!" % (output_directory,))
    parsed = urlparse.urlparse(file_uri)
    if not parsed.scheme or not parsed.path or not parsed.path.endswith(".zip"):
        raise ValueError("Invalid URI provided: %s" % (file_uri,))
    tdir = tempfile.mkdtemp()
    tfilename = os.path.join(tdir, parsed.path.split("/")[-1])
    urllib.urlretrieve(file_uri, tfilename)
    zfile = zipfile.ZipFile(tfilename)
    zfile.extractall(output_directory)
    del zfile
    shutil.rmtree(tdir)

# some test code
if __name__ == "__main__":
    fetch_and_unpack_zip_file(sys.argv[1], sys.argv[2])
