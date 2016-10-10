from __future__ import absolute_import
import sys
from testbenchexecutor import TestBenchExecutor
import argparse


def main():
    parser = argparse.ArgumentParser(description='Execute a test bench')
    parser.add_argument('--run-one', action='store_true',
                        help='run only the first unexecuted step (default: run all unexecuted steps)')
    parser.add_argument('--detailed-errors', '-d', action='store_true',
                        help='On step failure, output last few lines of log')
    parser.add_argument('manifest', type=str, nargs=1,
                        help='the path of the manifest to be executed')

    args = parser.parse_args()

    executor = TestBenchExecutor(args.manifest[0], detailed=args.detailed_errors)

    if args.run_one:
        sys.exit(executor.run_next_step())
    else:
        sys.exit(executor.run_all())

if __name__ == '__main__':
    main()
