import os
import sys
from boto.s3.connection import S3Connection, OrdinaryCallingFormat

s3_secret_key = ''
if s3_secret_key == '':
    s3_secret_key = os.environ.get('AWS_SECRET_ACCESS_KEY')

s3_user = 'meta_analysis:metax'

def add_arguments(parser):
    parser.add_argument("--user", help="S3 account:user")
    parser.add_argument("--password", help="S3 password")

def extract_crentials(args):
    global s3_user
    global s3_secret_key
    if args['user']:
        s3_user = args['user']
    if args['password']:
        s3_secret_key = args['password']

    if not s3_secret_key:
        sys.stderr.write('Must provide one of --password, %AWS_SECRET_ACCESS_KEY%\n')
        sys.exit(11)

    
def getS3Connection():
    return S3Connection(s3_user, s3_secret_key, host='swift.vehicleforge.org', calling_format=OrdinaryCallingFormat(), is_secure=True)
    #return S3Connection(s3_user, s3_secret_key, host='cloud.isis.vanderbilt.edu', port=8888, calling_format=OrdinaryCallingFormat(), is_secure=True)
