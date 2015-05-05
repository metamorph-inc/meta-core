#!/usr/bin/env python

# Take a filename, stereotype, and kind
# If the current tag matches the stereotype and the string specified
#   for the kind attribute, suppress its output

# Based on the XML filter example from IBM DeveloperWorks
# http://www.ibm.com/developerworks/xml/library/x-tipsaxflex/index.html

import sys
import xml.sax
from xml.sax.saxutils import XMLFilterBase, XMLGenerator

#Define constants for the two states we care about
ALLOW_CONTENT = 1
SUPPRESS_CONTENT = 2

class RemoveTagFilter(XMLFilterBase):
    def __init__(self, upstream, downstream, stereo, kind):
        XMLFilterBase.__init__(self, upstream)
        self._downstream = downstream
	self._stereotype = stereo
	self._kind = kind
        return

    def startDocument(self):
        #Set the initial state, and set up the stack of states
        self._state = ALLOW_CONTENT
        #self._state_stack = [ALLOW_CONTENT]
        return

    def startElement(self, name, attrs):
        #Check if there is any language attribute
        kind = attrs.get('kind', '')
        if name == self._stereotype and kind == self._kind:
            #Set the state as appropriate
            self._state = SUPPRESS_CONTENT
        #else:
        #    self._state = ALLOW_CONTENT
        #Always update the stack with the current state
        #Even if it has not changed
        #self._state_stack.append(self._state)
        #Only forward the event if the state warrants it
        if self._state == ALLOW_CONTENT:
            self._downstream.startElement(name, attrs)
        return

    def endElement(self, name):
        #self._state = self._state_stack.pop()
        #Only forward the event if the state warrants it
        if self._state == ALLOW_CONTENT:
            self._downstream.endElement(name)
        elif name == 'reference':
            self._state = ALLOW_CONTENT
        return

    def characters(self, content):
        #Only forward the event if the state warrants it
        if self._state == ALLOW_CONTENT:
            self._downstream.characters(content)
        return


if __name__ == "__main__":
    parser = xml.sax.make_parser()
    #XMLGenerator is a special SAX handler that merely writes
    #SAX events back into an XML document
    downstream_handler = XMLGenerator()
    #upstream, the parser, downstream, the next handler in the chain
    stereo = sys.argv[2]
    kind = sys.argv[3]
    filter_handler = RemoveTagFilter(parser, downstream_handler, stereo, kind)
    import sys
    #The SAX filter base is designed so that the filter takes
    #on much of the interface of the parser itself, including the
    #"parse" method
    filter_handler.parse(sys.argv[1])

