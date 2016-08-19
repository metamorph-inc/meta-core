import PCC as this_package
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-v", "--version", action="store_true", default=False,
    help='Displays the version number of this package.')
(opts, args) = parser.parse_args()

if opts.version:
    print this_package.__version__
else:
    print help(this_package)
