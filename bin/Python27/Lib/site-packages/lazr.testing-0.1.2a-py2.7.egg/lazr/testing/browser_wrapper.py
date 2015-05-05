#!/usr/bin/env python

import sys
import webbrowser

if __name__ == "__main__":
    if len(sys.argv) == 2:
        webbrowser.get().open(sys.argv[1])
