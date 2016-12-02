import cad_library
import os
import sys

def main():
    asminfo = cad_library.AssemblyInfo()
    asminfo.read_metrics_file(sys.argv[1]);
    asminforesult = cad_library.AssemblyInfo()
    asminforesult.read_metrics_file(sys.argv[2]);

    if asminfo.compare(asminforesult) == False:
        exit(-1)
    else:
        exit(0)

if __name__ == '__main__':
    main()
