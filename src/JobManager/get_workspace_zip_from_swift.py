
"""
import get_workspace_zip_from_swift
get_workspace_zip = get_workspace_zip_from_swift.get_workspace_zip
get_workspace_zip('')
"""

import sys
import os
import re
import itertools
import argparse

from boto.s3.key import Key

import swift_config # modify s3_secret_key and/or s3_user

def get_workspace_zip(inp):
    conn = swift_config.getS3Connection()

    match = re.search("AUTH[^/]+/([^/]+)/(Allura/User/[^/]+/[^/]+/(?:workspace|config).zip)", inp)
    if match is None:
        raise Exception("Input is not in expected format. It should be the resultsPutUrl parameter")
    bucket_name, key_name = match.groups()

    #b = conn.get_bucket(bucket_name)
    import boto.s3
    b = boto.s3.bucket.Bucket(conn, bucket_name)
    k = b.get_key(key_name)
    with open('workspace.zip', 'wb') as workspace_zip:
        k.get_contents_to_file(workspace_zip)

if __name__=='__main__':
    parser = argparse.ArgumentParser(prog='get_workspace_zip_from_swift.py')
    swift_config.add_arguments(parser)
    parser.add_argument("url", help='resultsPutUrl from Jenkins build')

    args = vars(parser.parse_args())
    swift_config.extract_crentials(args)

    get_workspace_zip(args['url'])
