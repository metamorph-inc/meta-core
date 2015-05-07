#ifndef _TTP_GENERATOR_H_
#define _TTP_GENERATOR_H_

#include <map>
#include <list>

#include "GenBuilder.h"

class TTPGenerator : public virtual PFGenerator
{
public:
	TTPGenerator(void);
	virtual ~TTPGenerator(void);

	virtual void AddAppNode( Node & n );
	//virtual void AddNodeTask( Node & n, Task & t ) { }
	virtual void AddTaskComponent( Task & t, Component & c );
	virtual void AddComponentPort( Component & c, IOPort & p );
	virtual void AddTaskMessage( Task & t, Message & m );
	
	virtual void AddBusMsg( Bus & b, Message & m );


	virtual void Generate( ConfigKeeper * ck );

protected:
	bool AddBuildLine(const std::string& hname, const std::string& hno);
	bool MakeBuildScripts(const std::string & appName);

	std::list< EsMoL::Node > _nodes;
	std::string _releaseDir;
	std::string _buildDir;
};

#define TTP_PLAN_PATH "cd %s & C:\\TTTech\\TTPplan\\5.4\\TTPplan_batch.exe -scripts %s database_name=%s"
#define TTP_BUILD_PATH "cd %s & C:\\TTTech\\TTPbuild\\5.4\\TTPbuild_batch.exe -scripts %s"

#define FNAME_COPY_NODE_TO_DDB "copy_node_to_ddb.bat"
#define BATCH_COPY_NODE_TO_DDB "\
@echo off\n\
\n\
if exist node%%1\\node%%1.s19 copy node%%1\\node%%1.s19 ..\\release\\%s.ddb\\node%%1_appl.s19\n\
\n"

#define FNAME_PRJ_SETUP "prj_setup.bat"
#define BATCH_PRJ_SETUP "\
@echo off\n\
rem ******************************************************** \n\
rem\n\
rem Project related setting\n\
rem\n\
rem ********************************************************\n\
\n\
if %%1.==. goto setit\n\
if %%1==-set goto setit\n\
if %%1==-clear goto clearit\n\
\n\
goto error\n\
\n\
rem ***************\n\
:setit\n\
rem ***************\n\
\n\
if %%BSP_MPC555%%.==. goto find_mysetup\n\
if %%BSP_TTPC%%.==. goto find_mysetup\n\
if %%BSP_TTTECH%%.==. goto find_mysetup\n\
if %%DIABDATA_PATH%%.==. goto find_mysetup\n\
\n\
set HWMODEL=pn312\n\
set CTYPE=C2NF\n\
\n\
rem set the toolset\n\
rem -----------------\n\
\n\
set TOOLSET=%%BSP_MPC555%%\\diabdata\n\
\n\
rem compiler settings\n\
rem -----------------\n\
\n\
set PROJ_COMOPT=-O -XO\n\
\n\
set PROJ_COMOPT=%%PROJ_COMOPT%% -Xpass-source -Xnested-interrupts -Xdollar-in-ident\n\
set COM_OPT=%%PROJ_COMOPT%% %%USER_COMOPT%% -D USE_BLDB_UTIL\n\
\n\
set PROJ_INC_PATH=-I %%BSP_MPC555%%\\include -I %%BSP_TTPC%%\\include -I %%BSP_TTTECH%%\\include\n\
set PROJ_INC_PATH=%%PROJ_INC_PATH%% -I %%TTPOS_MPC555_AS8202NF%%\\include\n\
\n\
set INC_PATH=%%PROJ_INC_PATH%% %%USER_INC_PATH%%\n\
\n\
rem assembler options\n\
rem -----------------\n\
set PROJ_ASMOPT=\n\
set ASM_OPT=%%PROJ_ASMOPT%% %%USER_ASMOPT%%\n\
\n\
rem linker options\n\
rem -----------------\n\
\n\
set PROJ_LNKOPT=-Xextern-in-place \n\
set LNK_OPT=%%PROJ_LNKOPT%% %%USER_LNKOPT%%\n\
\n\
set PROJ_LIB_PATH=-L %%TTPOS_MPC555_AS8202NF%%\\lib\n\
set LIB_PATH=%%PROJ_LIB_PATH%% %%USER_LIB_PATH%%\n\
set PROJ_LIBS=-l ttpos555_%%CTYPE%%_did -lc\n\
set LIBS=%%PROJ_LIBS%% %%USER_LIBS%%\n\
\n\
goto done\n\
\n\
rem ***************\n\
:clearit\n\
rem ***************\n\
\n\
set PMODEL=\n\
set HWMODEL=\n\
set PROJ_COMOPT=\n\
set PROJ_INC_PATH=\n\
set PROJ_AMSOPT=\n\
set PROJ_LNKOPT=\n\
set PROJ_LIB_PATH=\n\
set PROJ_LIBS=\n\
set TOOLSET=\n\
set DLD_PATH=\n\
\n\
set TTPOSPATH=%%SAVE_TTPOSPATH%%\n\
set SAVE_TTPOSPATH=\n\
\n\
goto done\n\
\n\
rem ***************\n\
:error\n\
rem ***************\n\
echo Valid commands are:\n\
echo  -set   ... set settings\n\
echo  -clear ... clear settings\n\
goto done\n\
\n\
rem ***************\n\
:find_mysetup\n\
rem ***************\n\
if not exist D:\\tttech\\bsp\\mysetup.bat goto try_c\n\
call d:\\tttech\\bsp\\mysetup.bat\n\
goto test_it\n\
\n\
rem ***************\n\
:try_c\n\
rem ***************\n\
if not exist c:\\tttech\\bsp\\mysetup.bat goto no_mysetup\n\
call c:\\tttech\\bsp\\mysetup.bat\n\
goto test_it\n\
\n\
rem ***************\n\
:test_it\n\
rem ***************\n\
\n\
if %%TTPOS_MPC555_AS8202%%.==. if %%TTPOS_MPC555_AS8202NF%%.==. goto no_mysetup\n\
if %%DIABDATA_PATH%%.==. goto no_mysetup\n\
if %%BSP_MPC555%%.==. goto no_mysetup\n\
if %%BSP_TTPC%%.==. goto no_mysetup \n\
if %%BSP_TTTECH%%.==. goto no_mysetup \n\
\n\
goto setit\n\
\n\
rem ***************\n\
:no_mysetup\n\
rem ***************\n\
echo .\n\
echo Error - Cannot find mysetup.bat\n\
echo .       or settings are invalid:\n\
echo .       TTPOS_MPC555_AS8202   = %%TTPOS_MPC555_AS8202%%\n\
echo .       TTPOS_MPC555_AS8202NF = %%TTPOS_MPC555_AS8202NF%%\n\
echo .       DIABDATA_PATH         = %%DIABDATA_PATH%%\n\
echo .       BSP_MPC555            = %%BSP_MPC555%%\n\
echo .       BSP_TTPC              = %%BSP_TTPC%%\n\
\n\
.       BSP_TTTECH            = %%BSP_TTTECH%%\n\
echo .\n\
echo Please call mysetup.bat manually!!\n\
echo .\n\
goto done \n\
\n\
rem ***************\n\
:done\n\
rem ***************\n\
\n"


#define FNAME_MAKE "make.bat"
#define BATCH_MAKE "\
@echo off\n\
rem ******************************************\n\
rem \n\
rem Make.bat - user lvl make file \n\
rem \n\
rem    6-Oct-2004 (AHE) CR RUP#8782: t3_ changed to sl_\n\
rem\n\
rem ******************************************\n\
if %%1%%.==. goto no_parameter\n\
set SRC_PATH=..\\..\\release\\%%1%%\n\
set SL_SRC_PATH=..\\..\\..\n\
rem *******************************\n\
rem * Please Costumize User options\n\
rem ******************************* \n\
set USER_INC_PATH=-I %%SRC_PATH%% -I %%SL_SRC_PATH%%\n\
set USER_COMOPT=\n\
set USER_ASMOPT=\n\
set USER_LNKOPT=\n\
set USER_LIB_PATH=\n\
set USER_LIBS=\n\
\n\
call prj_setup.bat -set\n\
\n\
echo %%INC_PATH%%\n\
\n\
set TRG_PATH=%%1%%\n\
cd %%TRG_PATH%%\n\
\n\
if not exist %%SRC_PATH%% goto source_path_not_found\n\
\n\
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\
echo Compiler: %%DIABDATA_PATH%% \n\
echo Toolset : %%TOOLSET%%\n\
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\
echo --------------------------------------------------------\n\
echo Step 1: Clean %%TRG_PATH%%\n\
\n\
echo --------------------------------------------------------\n\
\n\
del 2>nul *.o\n\
del 2>nul *.s\n\
del 2>nul *.dat\n\
del 2>nul *.src\n\
del 2>nul *.map\n\
del 2>nul stop\n\
\n\
echo -------------------------------------------------------\n\
echo Step 2a: Compiling *.c in %%SRC_PATH%%\n\
echo -------------------------------------------------------\n\
set CTARGETS=%%SRC_PATH%%\\*.c\n\
for %%%%c in (%%CTARGETS%%) do call %%TOOLSET%% -compile %%%%c\n\
if exist stop goto done\n\
\n\
echo -------------------------------------------------------\n\
echo Step 2b: Compiling *.c in %%SL_SRC_PATH%%\n\
echo -------------------------------------------------------\n\
set CTARGETS=%%SL_SRC_PATH%%\\*.c\n\
for %%%%c in (%%CTARGETS%%) do call %%TOOLSET%% -compile %%%%c\n\
if exist stop goto done\n\
\n\
echo -------------------------------------------------------\n\
echo Step 2c: Assembling *.s in %%SRC_PATH%%\n\
echo -------------------------------------------------------\n\
set ATARGETS=%%SRC_PATH%%\\*.s\n\
for %%%%a in (%%ATARGETS%%) do call %%TOOLSET%% -assemble %%%%a\n\
if exist stop goto done\n\
\n\
echo -------------------------------------------------------\n\
echo Step 3a: Compiling *.c in %%BSP_MPC555%%\\src\n\
echo -------------------------------------------------------\n\
set CTARGETS_SRCDIR=%%BSP_MPC555%%\\src\\*.c\n\
for %%%%c in (%%CTARGETS_SRCDIR%%) do call %%TOOLSET%% -compile %%%%c\n\
if exist stop goto done\n\
\n\
echo -------------------------------------------------------\n\
echo Step 3b: Assembling *.s in %%BSP_MPC555%%\\src\n\
echo -------------------------------------------------------\n\
set ATARGETS=%%BSP_MPC555%%\\src\\*.s\n\
for %%%%a in (%%ATARGETS%%) do call %%TOOLSET%% -assemble %%%%a\n\
if exist stop goto done\n\
\n\
echo -------------------------------------------------------\n\
echo Step 4: Compiling *.c in %%BSP_TTTECH%%\\src\n\
echo -------------------------------------------------------\n\
for %%%%c in (sl_bldb_util.c sl_crc.c sl_util.c sl_util_strlen.c) do call %%TOOLSET%% -compile %%BSP_TTTECH%%\\src\\%%%%c\n\
if exist stop goto done\n\
\n\
echo ----------------------------------------------\n\
echo Step 5:  Linking Files in ..\\%%TRG_PATH%%\\%%1%%\n\
echo -----------------------------------------------\n\
call %%TOOLSET%% -link ..\\%%TRG_PATH%%\\%%1%%\n\
goto done\n\
\n\
:no_parameter\n\
echo ------------------------------------------\n\
echo Please specify the target host to compile!\n\
\n\
------------------------------------------\n\
goto done\n\
\n\
:source_path_not_found\n\
echo -----------------------------------------------\n\
echo No directory %%1 containing source files found!\n\
echo -----------------------------------------------\n\
goto done\n\
\n\
:done\n\
echo ---------------------------------------\n\
echo done\n\
echo ---------------------------------------\n\
cd ..\n\
call copy_node_to_ddb.bat %%1%%\n\
\n"

#define FNAME_MAKE_ALL "make_all.bat"
#define BATCH_MAKE_ALL "call prj_setup.bat \n"
#define MAKE_ALL_ADDHOST "\
call make.bat %s\n\
call copy_node_to_ddb %s\n\
\n"


#endif // _TTP_GENERATOR_H_