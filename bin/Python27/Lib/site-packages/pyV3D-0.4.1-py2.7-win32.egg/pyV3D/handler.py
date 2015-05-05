import os
import traceback
from threading import Lock
import logging

from tornado import websocket

from pyV3D._pyV3D import WV_Wrapper
from pyV3D.cube import CubeGeometry

class WS_WV_Wrapper(WV_Wrapper):
    """A wrapper for the wv library that is used by a Sender to
    send updates out over a WebSocket.
    """
    
    def __init__(self):
        super(WS_WV_Wrapper, self).__init__()
        self.handlers = {}

    def send(self, first=False):
        self.prepare_for_sends()

        if first:
            self.send_GPrim(self, 1, self.send_binary_data)  # send init packet
            self.send_GPrim(self, -1, self.send_binary_data)  # send initial suite of GPrims
        else:  
            self.send_GPrim(self, -1, self.send_binary_data)  # send initial suite of GPrims

        self.finish_sends()
        
    def open(self, handler):
        if handler._protocol in self.handlers:
            raise RuntimeError("a handler for protocol %s is already registered to this wrapper" % handler._protocol)
        self.handlers[handler._protocol] = handler

    def send_binary_data(self, wsi, buf, ibuf):
        """This is called multiple times during the sending of a 
        set of graphics primitives.
        """
        try:
            handler = self.handlers['pyv3d-bin-1.0']
        except KeyError:
            raise RuntimeError("Can't send binary data. No registred binary protocol handler")
        try:
            handler.write_message(buf, binary=True)
        except Exception as err:
            logging.error("Exception in send_binary_data: %s", err)
            return -1
        return 0


class WSHandler(websocket.WebSocketHandler):

    subhandlers = {}   # map of obj pathname or file pathname to subhandler instance
    protocols   = {}   # map of protocols to lists of supporting subhandlers

    _lock = Lock()

    def initialize(self, view_dir):
        logging.debug("view dir = %s" % view_dir)
        self.view_dir = os.path.expanduser(os.path.abspath(view_dir))
        self.subhandler = None
        self.fname = None
        self.objname = None

    def _handle_request_exception(self, exc):
        logging.error("Unhandled exception: %s" % str(exc))
        super(WSHandler, self)._handle_request_exception(exc)

    def _execute(self, transforms, *args, **kwargs):
        try:
            self.fname = self.get_argument('fname')
        except:
            pass
        else:
            self.fname = os.path.abspath(os.path.join(self.view_dir, self.fname))
            if not self.fname.startswith(self.view_dir):
                logging.error("directory %s is outside of the allowed view directory")
                self.fname = None

        try:
            self.objname = self.get_argument('obj')
        except:
            pass

        self._protocol = None

        logging.debug("fname = %s" % self.fname)
        logging.debug("objname = %s" % self.objname)

        try:
            super(WSHandler, self)._execute(transforms, *args, **kwargs)
        except Exception as err:
            logging.error("%s" % err)

    def open(self, *args, **kwargs):
        wv = None
        args = (self.fname, self.objname)
        try:
            with self._lock:
            # look for the subhandler to see if we've already created one with 
            # another protocol, e.g., pyv3d-bin-1.0 and pyv3d-txt-1.0
                self.subhandler = self.subhandlers.get(args)
                if self.subhandler is not None:
                    logging.info("subhandler already existed, adding protocol %s to it." % self._protocol)

                if self.subhandler is None:
                    logging.info("creating a new subhandler for %s" % str(list(args)))
                    obj = self.fname if self.fname is not None else self._resolve(self.objname)

                    # try to create a subhandler matching the given protocol.  Take
                    # the first one that succeeds.  
                    for klass in self.protocols.get(self._protocol, []):
                        logging.debug("trying to create a %s" % klass)
                        if klass.supports(obj):
                            wv = WS_WV_Wrapper()

                            try:
                                self.subhandler = klass(wv, obj=obj)
                            except Exception as err:
                                logging.warning(err)
                                pass
                            else:
                                self.subhandlers[tuple(args)] = self.subhandler
                                break
                    else:
                        logging.error("No senders found for protocol %s." % self._protocol)
                        return
                    logging.info("created a new subhandler!")

            self.subhandler.wv.open(self)
            logging.info("subhandler opened with protocol %s" % self._protocol)

            if '-bin-' in self._protocol: # only do an initial send if it's a binary protocol 
                self.subhandler.send(obj, first=True)
        except Exception as err:
            logging.error('Exception: %s' % traceback.format_exc())

    def _resolve(self, name):
        """Try to find an object with the given name."""
        # for demo purposes, we've only got one type of object, a Cube
        return CubeGeometry()

    def on_message(self, message):
        if self.subhandler is None:
            logging.error("no subhandler to handle message")
            return
        self.subhandler.on_message(self, message)

    def on_close(self):
        try:
            if self.subhandler:
                self.subhandler.on_close()
        finally:
            with self._lock:
                for key,val in self.subhandlers.items():
                    if val is self.subhandler:
                        del self.subhandlers[key]
                        break

        logging.debug("WebSocket closed (proto=%s)" % self._protocol)

    def select_subprotocol(self, subprotocols):
        try:
            for p in subprotocols:
                if p in self.protocols:
                    self._protocol = p
                    logging.debug("matched subproto %s" % p)
                    return p
            logging.debug("returning None for subproto choices: %s" % subprotocols)
        except Exception as err:
            logging.error("%s" % err)
        self._protocol = None
        return None

