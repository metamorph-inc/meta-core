@ECHO ON

REM ============================================================
REM == Compile Solution                                       ==
REM ============================================================
CALL vsvars32
devenv VCP.sln /Build Debug


REM ============================================================
REM == Run Unit Tests                                         ==
REM ============================================================
REM pushd tests
REM esmol_test_all.m
REM popd
