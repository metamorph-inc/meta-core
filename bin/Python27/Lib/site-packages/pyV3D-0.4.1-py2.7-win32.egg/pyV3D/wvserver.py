import os
import sys
import logging

from argparse import ArgumentParser
from tornado import httpserver, web, ioloop

from pyV3D.handler import WSHandler

# this server demo just supports viewing of STL files and a simple cube
from pyV3D.stl import STLSender
from pyV3D.cube import CubeSender

def get_argument_parser():
    ''' create a parser for command line arguments
    '''
    parser = ArgumentParser(description='launch the test server')
    parser.add_argument('-p', '--port', type=int, dest='port', default=8000,
                        help='port to run server on')
    parser.add_argument('--logging', action='store', type=str, dest='logging',
                        default='warning',
                        help='set logging level, e.g., --logging=info or --logging=debug')
    parser.add_argument('viewdir', nargs='?', default='.',
                        help='pathname of directory containing files to view')
    return parser


def main():
    ''' Process command line arguments and run.
    '''
    parser = get_argument_parser()
    options, args = parser.parse_known_args()

    loglevels = {
        'debug': logging.DEBUG,
        'info': logging.INFO,
        'warning': logging.WARNING,
        'error': logging.ERROR,
        'critical': logging.CRITICAL,
    }
    logging.basicConfig(level=loglevels.get(options.logging, logging.WARNING))

    # associate a couple of senders with pyv3d protocols
    # NOTE: the server in its current state doesn't do anything with the pyv3d-txt-1.0
    #       channel, but the plumbing is there so that it SHOULD be pretty easy to 
    #       get it working...
    for proto in ['pyv3d-bin-1.0', 'pyv3d-txt-1.0']:
        WSHandler.protocols.setdefault(proto, []).append(CubeSender)
        WSHandler.protocols[proto].append(STLSender)

    viewdir = os.path.expanduser(os.path.abspath(options.viewdir))
    if not os.path.isdir(viewdir):
        sys.stderr.write("view directory '%s' does not exist.\n" % viewdir)
        sys.exit(-1)

    handler_data = {
       'view_dir': viewdir,
    }

    APP_DIR = os.path.dirname(os.path.abspath(__file__))

    handlers = [
        web.url(r'/',                web.RedirectHandler, {'url': '/index.html', 'permanent': False}),    
        web.url(r'/viewers?(.*)',    WSHandler, handler_data),
        web.url(r'/(.*)',            web.StaticFileHandler, {'path': os.path.join(APP_DIR,'wvclient')}),
    ]

    app_settings = {
        'static_path': APP_DIR,
        'debug':       True,
    }
   
    app = web.Application(handlers, **app_settings)

    http_server = httpserver.HTTPServer(app)
    http_server.listen(options.port)

    print 'Serving on port %d' % options.port
    try:
        ioloop.IOLoop.instance().start()
    except KeyboardInterrupt:
        print 'interrupt received, shutting down.'


if __name__ == '__main__':
    main()
