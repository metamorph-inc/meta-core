#!/bin/python
def check_version(min_version=(2,7,3), 
                  max_version=(2,7,4),
                  architecture='32bit',
                  msg_requirement='Please use Python 2.7.3 win32'):
    """
    Parameter architecture takes values '32bit', '64bit' or '' if check is not required. 
    
    If we only accept one version (a,b,c) -> min_version = (a,b,c) and max_version (a,b,c+1), 
    e.g., to specify 2.7.2 then set min_version=(2,7,2) and max_version=(2,7,3)
    
    """
    is_version_ok = None
    is_arch_type_ok = None

    # from python 1.5.2
    import sys
    print(sys.version)

    if not 'version_info' in dir(sys):
        print('Python version is too old')
    else:
        # from python 2.0
        print(sys.version_info)
        
        # TODO: what if one of them is not defined? min or max or both?
        if sys.version_info > min_version and sys.version_info < max_version:
            print('OK version number.')
            is_version_ok = True
        else:
            print('Wrong version number.')
            
        if architecture:
            if sys.version_info < (2, 3):
                print('Cannot check architecture python version is too old.')
            else:
                import platform
                if platform.architecture()[0] == architecture:
                    print('OK architecture ' + str(platform.architecture()))
                    is_arch_type_ok = True
                else:
                    print('Wrong architecture: ' + str(platform.architecture()) + ' required : ' + architecture)
        else:
            is_arch_type_ok = eval("True")
    return  is_version_ok and is_arch_type_ok

