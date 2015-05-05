
"""

"""

import sys
import os
import re
import itertools
import argparse

from boto.s3.key import Key

import swift_config # modify s3_secret_key and/or s3_user

bucket_name = 'd-vjxja5ob'

def get_workspace_zip(filename, **kwargs):
    conn = swift_config.getS3Connection()

    import boto.s3
    b = boto.s3.bucket.Bucket(conn, bucket_name)
    key_name = kwargs.get('key_name') or os.path.basename(filename)
    print key_name
    k = b.get_key(key_name)
    if k is not None:
        print "Current key has etag " + k.etag
        if not kwargs.get('overwrite', False):
            print "Use --overwrite to overwrite current contents"
            return
    k = boto.s3.key.Key(b, key_name)
    k.set_contents_from_filename(filename)

if __name__=='__main__':
    parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]))
    swift_config.add_arguments(parser)
    parser.add_argument("filename", help='filename to send to swift')
    parser.add_argument("--overwrite", help='overwrite current key', action='store_true')
    parser.add_argument("--key_name", help='key name (defaults to filename)')

    args = vars(parser.parse_args())
    swift_config.extract_crentials(args)
    #import pdb; pdb.set_trace()
    get_workspace_zip(**args)
