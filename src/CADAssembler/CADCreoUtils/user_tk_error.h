
#ifndef USER_TK_ERROR_H
#define USER_TK_ERROR_H

#include <TestError.h>

#if 0 /* Commented out for logging and reporting */
#ifdef DEBUG
#define ERROR_CHECK( func, call, err )	\
    if ( err != PRO_TK_NO_ERROR )	\
    fprintf( stderr, "%s in %s returned %s\n", call, func, protk_perror(err) );
#else
    #define ERROR_CHECK( func, call, err ) /* */
#endif
#endif /* if 0 */
    
#endif /* USER_TK_ERROR_H */
