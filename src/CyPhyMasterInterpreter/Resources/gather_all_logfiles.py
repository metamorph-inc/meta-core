__author__ = 'James'

import os
import glob
import shutil
import datetime
import tempfile
import zipfile


class GatherAllLogFiles(object):
    """
    Class definition

    """
    # hard-coded names
    new_project_logs_folder_name = 'project_logs'
    job_manager_folder_name = os.path.join('META', 'JobManager')
    right_now = ''
    new_logs_folder_name = ''

    # paths
    project_logs_folder_path = ''
    project_root_dir = ''
    project_results_folder_path = ''
    temp_folder = ''
    new_logs_folder_path = ''
    new_project_logs_folder_path = ''
    META_JobManager_path = ''
    path_within_archive = ''

    # lists of names/paths
    project_log_file_names = []
    project_log_file_paths = []
    result_folder_paths = []
    job_manager_log_paths = []
    file_name_pairs = []

    def __init__(self):
        """
        Constructor

        """
        self.right_now = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
        self.new_logs_folder_name = 'logs_' + self.right_now
        self.project_logs_folder_path = os.getcwd()
        self.project_root_dir = os.path.abspath(os.pardir)
        self.project_results_folder_path = os.path.join(self.project_root_dir, 'results')
        self.new_logs_folder_path = os.path.join(self.project_logs_folder_path, self.new_logs_folder_name)
        self.new_project_logs_folder_path = os.path.join(self.new_logs_folder_path, self.new_project_logs_folder_name)
        self.temp_folder = tempfile.gettempdir()
        self.META_JobManager_path = os.path.join(self.temp_folder, self.job_manager_folder_name)
        self.file_name_pairs = []

    def _copy_result_logs(self, result_root, dst_folder):
        """
        Helper function for copying

        """
        # if _FAILED.txt exists, copy it
        # if stderr.txt exists
        # if stdout.txt exists
        # if debug.log exists --removed 3/8/2013--
        # if consoleText.txt exists
        file_names = [
            '_FAILED.txt',
            'stderr.txt',
            'stdout.txt',
            'consoleText.txt',
        ]
        # copy the files
        for file_name in file_names:
            full_file_path = os.path.join(result_root, file_name)
            if os.path.exists(full_file_path):
                # if not os.path.exists(dst_folder):
                #     os.makedirs(dst_folder)
                # shutil.copy2(full_file_path, dst_folder)
                self.path_within_archive = os.path.relpath(dst_folder, self.new_logs_folder_path)
                self.file_name_pairs.append((full_file_path, self.path_within_archive))

        # if log directory exists, copy contents
        result_log_folder_path = os.path.join(result_root, 'log')

        # get all files in this folder, copy them to new directory
        if os.path.exists(result_log_folder_path):
            for result_log_file in os.listdir(result_log_folder_path):
                log_file_path = os.path.join(result_log_folder_path, result_log_file)
                if os.path.exists(log_file_path):
                    # if not os.path.exists(dst_folder):
                    #     os.makedirs(dst_folder)
                    # shutil.copy2(log_file_path, dst_folder)
                    self.path_within_archive = os.path.relpath(dst_folder, self.new_logs_folder_path)
                    self.file_name_pairs.append((log_file_path, self.path_within_archive))

        return 0

    def populate_lists(self):
        """
        make lists of the file paths to be copied

        """
        # find *.log files in the project's log folder, and store the paths
        os.chdir(self.project_logs_folder_path)
        project_log_file_names = glob.glob('*.log')
        for fn in project_log_file_names:
            fp = os.path.join(self.project_logs_folder_path, fn)
            self.project_log_file_paths.append(fp)

        # find the 8char result folder names, and store the paths
        if os.path.exists(self.project_results_folder_path):
            os.chdir(self.project_results_folder_path)
            for entry in os.listdir(self.project_results_folder_path):
                if os.path.isdir(entry):
                    dir_path = os.path.abspath(entry)
                    self.result_folder_paths.append(dir_path)

        # look for the META_JobManager in the %TEMP% folder
        if os.path.exists(self.META_JobManager_path):
            os.chdir(self.META_JobManager_path)
            for entry in os.listdir(self.META_JobManager_path):
                dir_path = os.path.abspath(entry)
                self.job_manager_log_paths.append(dir_path)

    def copy_log_files(self):
        """
        copy the files from the lists from "populate_lists" to the new directory

        """
        # create a new folder to place the copied log files
        # if not os.path.exists(self.new_logs_folder_path):
        #     os.makedirs(self.new_logs_folder_path)

        # copy the JobManger log files
        new_job_manager_log_folder = os.path.join(self.new_logs_folder_path, self.job_manager_folder_name)

        for log_path in self.job_manager_log_paths:
            if os.path.isfile(log_path):
                # if not os.path.exists(new_job_manager_log_folder):
                #     os.makedirs(new_job_manager_log_folder)
                # shutil.copy2(log_path, new_job_manager_log_folder)
                self.path_within_archive = os.path.relpath(new_job_manager_log_folder, self.new_logs_folder_path)
                self.file_name_pairs.append((log_path, self.path_within_archive))


        for project_log_file_path in self.project_log_file_paths:
            if os.path.isfile(project_log_file_path):
                # if not os.path.exists(self.new_project_logs_folder_path):
                #     os.makedirs(self.new_project_logs_folder_path)
                # shutil.copy2(project_log_file_path, self.new_project_logs_folder_path)
                self.path_within_archive = os.path.relpath(self.new_project_logs_folder_path, self.new_logs_folder_path)
                self.file_name_pairs.append((project_log_file_path, self.path_within_archive))

        for result_folder_path in self.result_folder_paths:
            dst_folder_name = os.path.join(self.new_logs_folder_path, 'results', os.path.basename(result_folder_path))
            self._copy_result_logs(result_folder_path, dst_folder_name)

    def create_log_archive(self):
        """
        create an archive from of the copied log folder, delete the log folder

        """
        os.chdir(self.project_logs_folder_path)
        name_of_archive = os.path.basename(self.new_logs_folder_path) + '.zip'
        temp_dir = os.path.basename(self.new_logs_folder_path)

        # with zipfile.ZipFile(name_of_archive, "w") as zf:
        #     for dirname, subdirs, files in os.walk(temp_dir):
        #         zf.write(dirname)
        #         for filename in files:
        #             f_name = os.path.join(dirname, filename)
        #             zf.write(f_name, compress_type=zipfile.ZIP_DEFLATED)
        #
        # shutil.rmtree(self.new_logs_folder_path)

        with zipfile.ZipFile(name_of_archive, "w") as zf:
            for original_file, zip_path in self.file_name_pairs:
                f_name = os.path.join(zip_path, os.path.basename(original_file))
                zf.write(original_file, f_name, compress_type=zipfile.ZIP_DEFLATED)

        return 0

if __name__ == '__main__':
    glf = GatherAllLogFiles()
    glf.populate_lists()
    glf.copy_log_files()
    glf.create_log_archive()
