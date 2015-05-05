/* Himanshu: muParser math function routines.
 * Actual definition exists in the 'core' project in mathfunc.cpp
 */
#ifdef __cplusplus
extern "C"
#endif
double mtbdd_call_muParser_math_function(double to, bdd_unary_math_function math_function);
int call_muParser_math_function(double arg, double *ret, bdd_unary_math_function math_function);