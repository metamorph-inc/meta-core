@ECHO OFF
REM BUG: tools should create output directories if necessary 
mkdir Generated\c_codegen
mkdir Generated\schedtool
mkdir Generated\schedresults
mkdir Generated\sl_codegen
mkdir Generated\sf_codegen
mkdir Generated\stage1
mkdir Generated\stage2-sched
mkdir Generated\stage2-frodo
mkdir Generated\stage2-truetime
mkdir Generated\target-frodo
mkdir Generated\target-truetime
CLS


ECHO --------------------------------------------------------------------------
ECHO Step 0: Register the ESMoL paradigm 
ECHO --------------------------------------------------------------------------
pushd "%VCP_PATH%\meta"
REM gmepy xme2xmp ESMoL.xme 
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 0 succeeded: initialized ESMoL environment
popd


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 1: Import simulink model 
ECHO --------------------------------------------------------------------------
"%VCP_PATH%\bin\mdl2mga.exe" -n ..\model\simulink\quad_integrator_subsys.mdl ..\model\QuadIntegratorBeforeUserIntervention.mga
	IF %ERRORLEVEL% NEQ 0 GOTO WORKAROUND1
	ECHO 	Step 1 succeeded: import Simulink


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Steps 2-4: Manually embellish ESMoL model w/ "deployment" aspects of model
ECHO            and add 'typing' info for datastructs
ECHO --------------------------------------------------------------------------
ECHO * currently simulating this step ... we'll just take the post-processed model we have squirrel'd away in SVN
ECHO	TODO: Debugging mga-xme-mga bug with UDM (JIRA: GME-XYZ)
	gmepy xme2mga ..\model\QuadIntegrator_post-processed.xme ..\model\QuadIntegrator_post-processed2.mga 
	REM mgadiff.py QuadIntegrator_post-processed.mga QuadIntegrator_post-processed2.mga
	REM copy ..\model\QuadIntegrator_post-processed.mga  ..\model\QuadIntegrator.mga
	copy ..\model\QuadIntegrator_post-processed2.mga ..\model\QuadIntegrator.mga
ECHO 	TODO: script the addition of the target-platform info to the model for automated tests ... 
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 2 succeeded: define hardware
ECHO 	TODO: script the addition of the architectural-deployment info to the model for automated tests ... 
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 3 succeeded: define deployment mapping
	REM "%VCP_PATH%\bin\AddTypes.exe" ..\model\QuadIntegrator.mga
	IF %ERRORLEVEL% NEQ 0 GOTO WORKAROUND2
	ECHO 	Step 4 succeeded: type inferencing
ECHO 	Steps 2-4 simulated successfully


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 5: Generate functional code
ECHO --------------------------------------------------------------------------
REM BUG: SL_CodeGen fails when writing *_SFC.xml file, when we use the '-p' flag	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
"%VCP_PATH%\bin\SL_CodeGen.exe" ..\model\QuadIntegrator.mga -p Generated\sl_codegen >Generated\sl_codegen\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 5a: simulink code blocks processed 
"%VCP_PATH%\bin\SF_CodeGen.exe" ..\model\QuadIntegrator.mga -t Generated\sf_codegen >Generated\sf_codegen\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 5b: statefolow code blocks processed
"%VCP_PATH%\bin\C_CodeGen.exe" -f ..\model\QuadIntegrator.mga -p Generated\c_codegen >Generated\c_codegen\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 5c: 'C' code blocks processed
ECHO 	Step 5 succeeded


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 6: Transform the model into its abstract description
ECHO --------------------------------------------------------------------------
"%VCP_PATH%\bin\Stage1.exe" -f ..\model\QuadIntegrator.mga -o Generated\stage1\QuadIntegrator.xml >Generated\stage1\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 6 succeeded


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 7: Generate input file for Scheduler utility
ECHO --------------------------------------------------------------------------
REM BUG: 'The Sched target ignores' "%VCP_PATH%\bin\Stage2.exe" -f Generated\stage1\QuadIntegrator.xml -p Generated\stage2-sched -o sched.scs -t Sched >Generated\stage2-sched\stdout.log 2>&1
"%VCP_PATH%\bin\Stage2.exe" -f Generated\stage1\QuadIntegrator.xml -o Generated\stage2-sched\sched.scs -t Sched -r 1ms >Generated\stage2-sched\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 7 succeeded


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 8: Generate schedule
ECHO --------------------------------------------------------------------------
"%VCP_PATH%\bin\SchedTool" -f Generated\stage2-sched\sched.scs -o Generated\schedtool\results.rslt >Generated\schedtool\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 8 succeeded


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 9: Merge schedule back into models
ECHO --------------------------------------------------------------------------
"%VCP_PATH%\bin\SchedResults" -r Generated\schedtool\results.rslt -e ..\model\QuadIntegrator.mga -a Generated\stage1\QuadIntegrator.xml >Generated\schedresults\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO 	Step 9 succeeded


:STEP10
ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 10: Generate glue code
ECHO --------------------------------------------------------------------------
REM  == BUG: "%VCP_PATH%\bin\Stage2.exe" -t FRODO    -f Generated\stage1\QuadIntegrator.xml -p Generated -o stage2-frodo 
"%VCP_PATH%\bin\Stage2.exe" -t FRODO    -f Generated\stage1\QuadIntegrator.xml -o Generated\stage2-frodo >Generated\stage2-frodo\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO    Step 10a: FRODO distribution synthesized
REM  == BUG: "%VCP_PATH%\bin\Stage2.exe" -t TrueTime -f Generated\stage1\QuadIntegrator.xml -p Generated\stage2-truetime -o QuadIntegrator >Generated\stage2-truetime\stdout.log 2>&1
"%VCP_PATH%\bin\Stage2.exe" -t TrueTime -f Generated\stage1\QuadIntegrator.xml -p Generated\stage2-truetime -o QuadIntegrator >Generated\stage2-truetime\stdout.log 2>&1
	IF %ERRORLEVEL% NEQ 0 GOTO FAIL
	ECHO    Step 10b: TrueTime distribution synthesized
ECHO	Step 10 succeeded


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 11: Copy generated code to working directories for various target platforms
ECHO --------------------------------------------------------------------------
copy Generated\sl_codegen\*.c 		Generated\target-frodo
copy Generated\sl_codegen\*.h		Generated\target-frodo
copy Generated\c_codegen\*.c 		Generated\target-frodo
copy Generated\c_codegen\*.h		Generated\target-frodo
copy Generated\stage2-frodo\GS.c	Generated\target-frodo
copy Generated\stage2-frodo\RS.c	Generated\target-frodo
;
copy Generated\sl_codegen\*.c 				Generated\target-truetime
copy Generated\sl_codegen\*.h				Generated\target-truetime
copy Generated\c_codegen\*.c 				Generated\target-truetime
copy Generated\c_codegen\*.h				Generated\target-truetime
copy Generated\stage2-truetime\GS_init.cpp		Generated\target-truetime
copy Generated\stage2-truetime\RS_init.cpp		Generated\target-truetime
copy Generated\stage2-truetime\QuadIntegrator_gen.m	Generated\target-truetime


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 12: Run TrueTime simulation
ECHO --------------------------------------------------------------------------
pushd Generated\target-truetime
        ECHO.
	ECHO NOTE: `Generated/truetime/QuadIntegrator_gen.m` does not quite work 
	ECHO in the full automation loop yet.  We will instead grab the 
	ECHO pre-staged targets/truetime/QuadIntegrator_post-processed.mdl 
	ECHO file, which has scopes and signal connections already defined 
	ECHO plus a nice, readable layout of the widgets on the drawing canvas. 
        ECHO.
	copy ..\..\..\targets\truetime\QuadIntegrator_post-processed.mdl .\QuadIntegrator.mdl
	copy ..\..\..\targets\truetime\init_ttenv.m .
        ECHO.
	ECHO * NOW, PLEASE CARRY OUT THE FOLLOWING STEPS:  
        ECHO.
	ECHO `cd Generated\target-truetime`
	ECHO `matlab`
	ECHO from MATLAB: `mex -setup`
	ECHO   # Instruct MATLAB to use the Visual Studio 2008 Compiler
	ECHO   # This is a one-time initialization step necessary to force 
	ECHO   # MATLAB to use VS for all MEX file compilation
	ECHO from MATLAB: `init_ttenv`
	ECHO from MATLAB: `ttmex -g GS_init.cpp`
	ECHO from MATLAB: `ttmex -g RS_init.cpp`
	ECHO from MATLAB: `truetime` 
	ECHO   # Now open Generated\target-truetime\QuadIntegrator.mdl and 
	ECHO   # run simulation on the contained TrueTime model
        ECHO.
        ECHO.
	ECHO ** Examine the contents of tests/Generated/target-truetime/ 
	ECHO    to ensure that it is similar to those files shipped by the 
	ECHO    installer in found in targets/truetime/.  
        ECHO.
	popd


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step 13: Deploy system to Gumstix/Robostix hardware (w/ xPC simulation)
ECHO --------------------------------------------------------------------------
pushd ..\targets\hardware
        ECHO.
	ECHO * NOW, PLEASE CARRY OUT THE FOLLOWING STEPS:  
	ECHO.
	ECHO.
	ECHO 1) from targets/hardware/ directory on Windows machine:
        ECHO.
	ECHO `deploy.cmd`    # copies sources generated above to cross-compiler dir
	ECHO. 
	ECHO from MATLAB: open `xpcexplr`
	ECHO from MATLAB: compile and deploy plant dynamics model contained in 
	ECHO	targets/hardware/xPC/quad_integrator_xpc.mdl to your xPC device
        ECHO.
	ECHO.
	ECHO 2) from 'cross-compiler' Ubuntu-based VM: 
	ECHO.
	ECHO `cd targets/hardware/build`
	ECHO `bin/gf`
	ECHO `bin/rf`
	ECHO ssh root@gumstix
	ECHO `run_controller.sh`
	ECHO.
	ECHO.
	popd


ECHO.
ECHO.
ECHO --------------------------------------------------------------------------
ECHO Step N: That's all
ECHO --------------------------------------------------------------------------
echo ********** D O N E **********





:SUCCEED
	exit /b 0
:FAIL
	echo ********************  F A I L E D  ********************
	exit /b 1
:WORKAROUND1
	echo.
	echo ERROR: Please open 'model/QuadIntegratorBeforeUserIntervention.mga' in GME and "upgrade" to the currently registered 'ESMoL' paradigm.
	echo ********************  F A I L E D  ********************
	exit /b 2
:WORKAROUND2
	echo.
	echo ERROR: Please open 'model/QuadIntegrator_post-processed.mga' in GME and "upgrade" to the currently registered 'ESMoL' paradigm.
	echo ********************  F A I L E D  ********************
	exit /b 2
