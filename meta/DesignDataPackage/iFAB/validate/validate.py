"""
validate.py
Validate XML file against XSD schema using minixsd library.
Matthew Woodruff (mjw5407@arl.psu.edu)
The Pennsylvania State University
Applied Research Laboratory
2012
"""
import argparse
from collections import namedtuple
from minixsv.pyxsval import parseAndValidate
from genxmlif import GenXmlIfError
from minixsv.xsvalErrorHandler import XsvalError

ValidationResult = namedtuple("Result", "passed message")

def get_args():
    description = "Validate XML file against XSD schema using "\
                  "the minixsd Python library."
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("xml", help="XML file to validate")
    parser.add_argument("xsd", help="XSD schema to validate against")
    return parser.parse_args()

def validate(xml, xsd):
    try:
        parseAndValidate(xml, xsdFile=xsd)
    except IOError as errstr:
        return ValidationResult(False, errstr)
    except GenXmlIfError as errstr:
        return ValidationResult(False, errstr)
    except XsvalError as errstr:
        return ValidationResult(False, errstr)

    return ValidationResult(True, 
                "{0} complies to {1}".format(xml, xsd))

def cli():
    args = get_args()
    result = validate(args.xml, args.xsd)
    print(result.message)

if __name__ == "__main__":
    cli()

# vim:ts=4:sw=4:expandtab:fdm=indent:wrap lbr:ai
