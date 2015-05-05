from version_check import check_version

if check_version(min_version=(2,7,3), max_version=(2,8)):
    print('Version test passed calling real script which is compatible with a couple of versions only.')
    import run_sot
    import sys
    sys.exit(run_sot.main())
else:
    import sys
    sys.exit(5)
