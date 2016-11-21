import sys
import os
import argparse
import subprocess
import random
import string
import shutil
import logging
import datetime
import cad_library
import re

print_cmds = True


class CADJobDriver():

    def __init__(self, assembler, mesher, analyzer, mode, run_postprocessing=True):

        self.logger = None
        self.get_logger()

        if assembler is None:
            assembler = 'CREO'
            self.logger.warning('No assembler specified.')

        if mesher is None:
            mesher = 'NONE'
            self.logger.warning('No mesher specified.')

        if analyzer is None:
            analyzer = 'NONE'
            self.logger.warning('No analyzer specified.')

        if mode is None:
            mode = 'STATIC'
            self.logger.warning('No mode specified.')

        self.logger.info('Assembler: {}'.format(assembler))
        self.logger.info('Mesher: {}'.format(mesher))
        self.logger.info('Analyzer: {}'.format(analyzer))
        self.logger.info('Mode: {}'.format(mode))

        self.assembler = assembler
        self.mesher = mesher
        self.analyzer = analyzer
        self.mode = mode

        self.meta_bin_cad = os.path.join(cad_library.META_PATH, 'bin', 'CAD')

        self.run_pp = run_postprocessing

        self.run_job()

    def run_job(self):

        createAssembly_LogFile = ".\log\cad-assembler.log"

        # Run assembler
        if self.assembler == 'CREO':
            self.logger.info("Calling CREO...")

            result = 42

            try:
                result = self.run_creo_assembler()
                self.logger.info("CADCreoCreateAssembly Result: {}".format(result))
            except Exception:
                self.logger.error("CADCreoCreateAssembly Exception. " +
                "See {}".format("log/cad-assembler.log"))
                cad_library.exitwitherror(
                    'CADJobDriver.py: The CreateAssembly threw an Exception' +
                    '. See ' + createAssembly_LogFile + ' for more information.', -1)

            if result != 0:
                cad_library.exitwitherror(
                    'CADJobDriver.py: The CreateAssembly program returned with error: ' + str(result) +
                    '. See ' + createAssembly_LogFile + ' for more information.', -1)

        elif self.assembler == 'ASSEMBLY_EXISTS':
            self.logger.info('CadAssembly has already exists.')

        else:
            cad_library.exitwitherror('CADJobDriver.py: Only CREO assembler is supported.', -1)

        # Run mesher
        if self.mesher == 'ABAQUS' or self.mesher == 'ABAQUSMDLCHECK':
            if self.analyzer == 'NONE':
                self.run_abaqus_model_based(True, self.mesher == 'ABAQUSMDLCHECK')
            elif self.analyzer == 'ABAQUSMODEL':
                self.run_abaqus_model_based(False, False, self.mode)
            else:
                cad_library.exitwitherror('Abaqus mesher only supports Abaqus Model-Based.', -1)
        elif self.mesher == 'PATRAN':
            if self.analyzer == 'PATRAN_NASTRAN':
                self.logger.info("Calling Patran/Nastran")
                self.run_patran_nastran()
                # self.logger.warning("NOT CALLING POST PROCESSING.")
                # self.run_patran_nastran(False)
                self.logger.info("Finished calling Patran/Nastran")
            else:
                cad_library.exitwitherror('CADJobDriver.py: mesher=PATRAN requires analyzer=PATRAN_NASTRAN', -1)
        elif self.mesher == 'CREO':
            # Skipping, CREO has already been invoked
            pass
        elif self.mesher == 'NONE':
            # Not meshing, skip analysis
            self.copy_failed_and_exit(0)
        else:
            cad_library.exitwitherror('CADJobDriver.py: Mesher ' + self.mesher + ' is not supported.', -1)

        # Run analyzer
        if self.analyzer == 'ABAQUSMODEL':
            pass
            if self.mesher == 'ABAQUS' or self.mesher == 'ABAQUSMDLCHECK':
                # Skip this, it has already been executed in teh previous section
                pass
            else:
                self.run_abaqus_model_based(False, False, self.mode)
        elif self.analyzer == 'ABAQUSDECK':
            self.run_abaqus_deck_based()
        elif self.analyzer == 'NASTRAN':
            self.run_nastran()
        elif self.analyzer == 'CALCULIX':
            self.run_calculix()

        self.copy_failed_and_exit(0)

    def get_logger(self):

        datetime_now = datetime.datetime.now()

        # create logger with 'spam_application'
        self.logger = logging.getLogger('CADJobDriver')

        self.logger.info("======================================================")
        self.logger.info("    New CADJobDriver Instance: {}".format(datetime_now))
        self.logger.info("======================================================")

    def run_creo_assembler(self):

        isis_ext = os.environ.get('PROE_ISIS_EXTENSIONS')
        if isis_ext is None:
            cad_library.exitwitherror(
                'PROE_ISIS_EXTENSIONS env. variable is not set. Do you have the META toolchain installed properly?', -1)

        create_asm = os.path.join(isis_ext, 'bin', 'CADCreoParametricCreateAssembly.exe')
        if not os.path.isfile(create_asm):
            cad_library.exitwitherror(
                'Cannot find CADCreoParametricCreateAssembly.exe. Do you have the META toolchain installed properly?', -1)

        # logdir = os.path.join(workdir, 'log')
        result = os.system('\"' + create_asm + '" -i CADAssembly.xml')

        return result

    def call_subprocess(self, cmd, failonexit=True):
        global print_cmds
        if print_cmds is True:
            print cmd

        result = 0

        try:
            result = subprocess.call(cmd)
        except Exception as e:
            cad_library.exitwitherror('Failed to execute: ' + cmd + ' Error is: ' + e.message, -1)

        if result != 0 and failonexit:
            cad_library.exitwitherror('The command {} exited with value: {}'.format(cmd, result), -1)

        return result

    def copy_failed_and_exit(self, code):
        for (root, dirs, files) in os.walk(os.getcwd(), topdown=False):
            for file in files:
                print os.path.join(root, file)
                if file == '_FAILED.txt':
                    # copy_command = 'copy {} {}'.format(os.path.join(root, file), os.getcwd())
                    os.system('copy ' + os.path.join(root, file) + ' ' + os.getcwd())

        exit(code)

    def run_abaqus_model_based(self, meshonly, modelcheck, mode=None):
        feascript = cad_library.META_PATH + 'bin\\CAD\\Abaqus\\AbaqusMain.py'
        if meshonly:
            if modelcheck:
                param = '-b'
            else:
                param = '-o'
        else:
            if mode == 'STATIC':
                param = '-s'
            elif mode == 'MODAL':
                param = '-m'
            elif mode == 'DYNIMPL':
                param = '-i'
            elif mode == 'DYNEXPL':
                param = '-e'

        self.call_subprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat cae noGUI="' + feascript + '" -- ' + param)

    def run_abaqus_deck_based(self):
        id = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(6))
        os.chdir(os.getcwd() + '\\Analysis\\Abaqus')
        self.call_subprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat fromnastran job=' + id + ' input=..\Nastran_mod.nas')
        self.call_subprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat analysis interactive job=' + id)
        self.call_subprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat odbreport job=' + id + ' results')
        self.call_subprocess('c:\\SIMULIA\\Abaqus\\Commands\\abaqus.bat cae noGUI="' + cad_library.META_PATH + '\\bin\\CAD\\ABQ_CompletePostProcess.py\" -- -o ' + id + '.odb -p ..\\AnalysisMetaData.xml -m ..\\..\\RequestedMetrics.xml -j ..\\..\\testbench_manifest.json')

    def clean_create_patran_model_session_file(self, session_file, path_to_create_patran):

        with open(session_file, 'r') as f:
            ses_template_txt = f.read()
            ses_txt = ses_template_txt.replace("zz_CREATE_PATRAN_MODEL_SOURCE_DIR_zz", path_to_create_patran)

        with open(session_file, 'w') as f:
            f.write(ses_txt)

    def run_patran_post_processing(self):

        # Post-Processing call
        patran_pp_name = 'Patran_PP.py'
        patran_pp_path = os.path.join(self.meta_bin_cad, patran_pp_name)
        if not os.path.isfile(patran_pp_path):
            cad_library.exitwitherror(
                'Can\'t find {}. Do you have the META toolchain installed properly?', format(patran_pp_name), -1)

        post_processing_args = "{} {} {}".format(
            "..\\AnalysisMetaData.xml",
            "..\\..\\RequestedMetrics.xml",
            "..\\..\\testbench_manifest.json"
        )

        pp_command = 'runPostProcessing.bat'

        with open(pp_command, 'wb') as cmd_file_out:
            meta_python_path = sys.executable
            cmd_text = '"{}" "{}" {}'.format(
                meta_python_path, patran_pp_path, post_processing_args)
            cmd_file_out.write(cmd_text)

        print("Starting {}...".format(pp_command))

        pp_result = self.call_subprocess(pp_command)

        return pp_result

    def run_patran_nastran(self):

        meta_bin_cad_CreatePatranModel = os.path.join(self.meta_bin_cad, 'CreatePatranModel')

        cpif_name = 'CreatePatranInputFile.py'
        cpif_path_py = os.path.join(self.meta_bin_cad, cpif_name)

        result_dir = os.path.abspath(os.getcwd())
        patran_nastran_dir = os.path.join(result_dir, 'Analysis', 'Patran_Nastran')

        if not os.path.exists(patran_nastran_dir):
            os.makedirs(patran_nastran_dir)

        os.chdir(patran_nastran_dir)

        python_command = " \"{}\" {} {} {}".format(
            cpif_path_py,
            '-cadassembly ../../CADAssembly.xml',
            '-analysismetaData ../AnalysisMetaData.xml',
            '-computedvalues ../../ComputedValues.xml'
        )

        cpif_result = self.call_subprocess(sys.executable + python_command)

        if cpif_result != 0:
            if cpif_result == 99:
                msg = 'CreatePatranInputFile.py failed; see {}.'.format(os.path.join(patran_nastran_dir, '_FAILED.txt'))
                cad_library.exitwitherror(msg, -1)
            else:
                os.chdir(result_dir)
                cad_library.exitwitherror('CADJobDriver.run_patran_nastran() failed; try debugging directly.', -1)

        ses_name = 'CreatePatranModel.ses'
        ses_path = os.path.join(meta_bin_cad_CreatePatranModel, ses_name)

        if not os.path.exists(ses_path):
            cad_library.exitwitherror("Could not find {} ({}).".format(ses_name, ses_path), -1)

        try:
            shutil.copy2(ses_path, patran_nastran_dir)
            new_ses_file_path = os.path.join(patran_nastran_dir, ses_name)
            self.clean_create_patran_model_session_file(new_ses_file_path, meta_bin_cad_CreatePatranModel)

        except Exception as e:
            self.logger.error(str(e))

        pcl_command = "patran -b -graphics -sfp  {} -stdout .\\log\\CreatePatranModel_Session.log".format(ses_name)
        pcl_command_file = 'runPCLModel.bat'

        with open(pcl_command_file, 'wb') as cmd_file_out:
            cmd_file_out.write(pcl_command)

        pcl_command_script_cmd = '.\\{}'\
            .format(pcl_command_file)

        create_patran_pcl_dir = 'CreatePatranModel_PLB'
        if not os.path.exists(create_patran_pcl_dir):
            os.makedirs(create_patran_pcl_dir)

        self.call_subprocess(pcl_command_script_cmd)

        if not os.path.exists("_SUCCEEDED_CreatePatranModel.TXT"):
            cad_library.exitwitherror("CreatePatranModel.pcl failed. See .\log\CreatePatranModel_Session.log " +
            "and .\log\CreatePatranModel_Application.log", -1)

        if not self.run_pp:
            return 0

        else:
            pp_result = self.run_patran_post_processing()

            if not os.path.exists("_SUCCEEDED_PatranPostProcessing.TXT"):
                cad_library.exitwitherror("vPatranPostProcess.pcl failed. See .\log\PatranPostProcessing_Session.log " +
                "and .\log\PatranPostProcessing_Application.log", -1)

            if pp_result != 0:
                msg = "Patran Post Processing failed."
                cad_library.exitwitherror(msg, -1)

    def run_nastran(self):

        cwd = os.path.abspath(os.getcwd())

        nastran_dir = os.path.join(cwd, 'Analysis', 'Nastran')

        os.chdir(nastran_dir)

        nastran_py_cmd = ' \"' + cad_library.META_PATH + 'bin\\CAD\Nastran.py\" ..\\Nastran_mod.nas'

        self.call_subprocess(sys.executable + nastran_py_cmd)

        # Post-Processing call
        if not self.run_pp:
            return 0

        else:
            pp_result = self.run_patran_post_processing()

            if pp_result != 0:
                msg = "Patran Post Processing failed."
                cad_library.exitwitherror(msg, -1)

        # Old Post-Processing call
        # patran_pp_py = cad_library.META_PATH + 'bin\\CAD\\Patran_PP.py'
        #
        # if not os.path.isfile(patran_pp_py):
        #     msg = 'Can\'t find ' + patran_pp_py + '. Do you have the META toolchain installed properly?'
        #     cad_library.exitwitherror(msg, -1)
        #
        # nas_path = '..\\Nastran_mod.nas'
        # xdb_path = 'Nastran_mod.xdb'
        # meta_data = '..\\AnalysisMetaData.xml'
        # req_metrics = '..\\..\\RequestedMetrics.xml'
        # tb_manifest = '..\\..\\testbench_manifest.json'
        #
        # patran_pp_py_args = ' \"{}\" {} {} {} {} {}'\
        #     .format(patran_pp_py, nas_path, xdb_path, meta_data, req_metrics, tb_manifest)
        #
        # self.call_subprocess(sys.executable + patran_pp_py_args)

    def run_calculix(self):
        import _winreg
        isisext = os.environ['PROE_ISIS_EXTENSIONS']
        os.chdir(os.getcwd() + "\\Analysis\\Calculix")
        if isisext is None:
            cad_library.exitwitherror('PROE_ISIS_EXTENSIONS env. variable is not set. Do you have the META toolchain installed properly?', -1)
        deckconvexe = os.path.join(isisext, 'bin', 'DeckConverter.exe')
        self.call_subprocess(deckconvexe + ' -i ..\\Nastran_mod.nas')
        with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\CMS\CalculiX', 0,
                         _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
            bconvergedpath = _winreg.QueryValueEx(key, 'InstallLocation')[0]
        self.call_subprocess(bconvergedpath+'\\CalculiX\\bin\\ccx.bat -i ..\\Nastran_mod')
        metapython = os.path.join(cad_library.META_PATH, 'bin', 'Python27', 'Scripts', 'python.exe')
        calculix_pp = os.path.join(cad_library.META_PATH, 'bin', 'CAD', 'ProcessCalculix.py')
        self.call_subprocess(metapython + " " + calculix_pp + " -o ..\\Nastran_mod.frd -p ..\\AnalysisMetaData.xml -m ..\\..\\RequestedMetrics.xml -j ..\\..\\testbench_manifest.json -e PSolid_Element_Map.csv")


def main():

    # create file handler which logs even debug messages
    log_path = 'log'

    # R.O. 10/10/2016, When running a PET, there is some cleanup work that is necessary.
    # Notes - 1) If this is not a PET, then the Analysis directory would not exist and the following code
    #              would be skipped.
    #          2) Deleting the analysis directory should not be necessary.  The analysis scripts should
    #             handle multiple runs in the same directory; however, currently they do not.  Specifically,
    #             without deleting the analysis directory, output.csv will have the same results for each
    #             entry (i.e. each run).

    if os.path.exists("Analysis"):
        shutil.rmtree("Analysis")

    # R.O. 10/10/2016, When running a PET, it is desirable to delete the old versions of the Creo models/trail-file; otherwise,
    # the models will accumulate and waste disk space.
    for file in os.listdir('.'):
        if re.match(".*[.]{1}prt[.]{1}[0-9]+$", file) or re.match(".*[.]{1}asm[.]{1}[0-9]+$", file) or re.match("trail[.]{1}txt[.]{1}[0-9]+$", file):
            os.remove(file)

    if not os.path.isdir(log_path):
        os.mkdir(log_path)

    logger = logging.getLogger('CADJobDriver')
    logger.setLevel(logging.DEBUG)
    handler = logging.FileHandler(os.path.join(log_path, 'CADJobDriver.py.txt'), 'w')
    formatter = logging.Formatter(
        '%(asctime)s %(name)-12s %(levelname)-8s %(message)s')
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    global args

    parser = argparse.ArgumentParser(description='Executes a CAD or FEA job. Invokes the specified assembler, mesher and analyzer in this sequence.')
    parser.add_argument('-assembler', choices=['CREO'])
    parser.add_argument('-mesher', choices=['NONE', 'CREO', 'ABAQUS', 'PATRAN', 'ABAQUSMDLCHECK', 'GMESH'])
    parser.add_argument('-analyzer', choices=['NONE', 'ABAQUSMODEL', 'ABAQUSDECK', 'NASTRAN', 'CALCULIX', 'PATRAN_NASTRAN'])
    parser.add_argument('-mode', choices=['STATIC', 'MODAL', 'DYNIMPL', 'DYNEXPL'])
    args = parser.parse_args()

    cad_job_driver = CADJobDriver(args.assembler, args.mesher, args.analyzer, args.mode)
    # cad_job_driver = CADJobDriver('ASSEMBLY_EXISTS', args.mesher, args.analyzer, args.mode, False)
    # cad_job_driver = CADJobDriver(args.assembler, args.mesher, args.analyzer, args.mode, False)


if __name__ == '__main__':
    import win32api
    import win32job
    hProcess = win32api.GetCurrentProcess()
    if not win32job.IsProcessInJob(hProcess, None):
        hJob = win32job.CreateJobObject(None, "")
        extended_info = win32job.QueryInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation)
        extended_info['BasicLimitInformation']['LimitFlags'] = win32job.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | win32job.JOB_OBJECT_LIMIT_BREAKAWAY_OK
        win32job.SetInformationJobObject(hJob, win32job.JobObjectExtendedLimitInformation, extended_info)
        win32job.AssignProcessToJobObject(hJob, hProcess)
    main()
