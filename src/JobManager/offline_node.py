
import sys
import os.path
import jenkinsapi
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]))
    parser.add_argument('worker', nargs='*')
    parser.add_argument('--message', help='Offline message shown in Jenkins', default='upgrade META')

    args = vars(parser.parse_args())

    from jenkinsapi.jenkins import Jenkins
    j = Jenkins('http://localhost:9999')
    for node in args['worker']:
        j.get_node(node).set_offline(args['message'])
