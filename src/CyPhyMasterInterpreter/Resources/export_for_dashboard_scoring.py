import os
import json
import time
import shutil
import subprocess
from glob import glob

# remarks: should TestBenches inside the project-file be empty?

DST_FOLDER = "results_light"

class Exporter(object):

    #   Default directories
    dashboard_dir = './dashboard/'
    designs_dir = './designs/'
    design_space_dir = './design-space/'
    requirements_dir = './requirements/'
    results_dir = './results/'
    test_benches_dir = './test-benches/'

    #   Single files
    meta_results_file = ''
    project_file = ''
    requirements_file = ''

    #   Sets of collected .json files
    design_files_dir = set()        # designs inside /designs_dir
    design_files_ms = set()         # designs linked from meta_results
    design_files_pr = set()         # designs linked from project_file
    design_files_OK = set()         # designs that are linked from an OK result_file

    design_space_files_dir = set()  # design spaces inside /design_space_dir
    design_space_files_pr = set()   # design spaces linked from project_file

    result_files_dir = set()        # results inside /results_dir/4digit-ID/s
    result_files_ms = set()         # results linked from meta_results
    result_files_OK = set()         # results with OK AnalysisStatus

    test_bench_files_dir = set()    # test-benches inside /test_benches_dir
    test_bench_files_ms = set()     # test-benches linked from meta_results
    #test_bench_files_pr = set()     # test-benches linked from project_file

    stat_files = []                 # all encountered stat.json files

    #   Final sets of .json after intersections been taken
    design_files = set()
    design_space_files = set()
    result_files = set()
    test_bench_files = set()

    #   summary.report.json linked files
    files_linked_from_sum_reps = []
    folders_linked_from_sum_reps = []
    cur_8digit_dir = ''
    # These are interpreter specific files linked from the summary.report.json.
    # They are gathered at the very end. Going through the results_files.

    def run_test(self):
        """
        Test function that populates all sets with files in correct order
        and writes out all sets in test_run.json

        """

        # populate *_ps sets
        self.enter_project_file()

        # populate *_dir sets
        self.enter_directories()

        # The files in the directories makes up the largest possible set of files
        self.result_files = self.result_files_dir
        self.design_files = self.design_files_dir
        self.design_space_files = self.design_space_files_dir
        self.test_bench_files = self.test_bench_files_dir

        # populate *_ms sets
        self.enter_meta_results_file()

        # populate *_OK sets
        self.check_analysis_status()

        df = {'design_files_dir' : list(self.design_files_dir),'design_files_pr' : list(self.design_files_pr),
              'design_files_ms' : list(self.design_files_ms), 'design_files_OK' : list(self.design_files_OK)}

        ds = {'design_space_files_dir' : list(self.design_space_files_dir),
              'design_space_files_pr' : list(self.design_space_files_pr)}

        rs = {'result_files_dir' : list(self.result_files_dir), 'result_files_ms' : list(self.result_files_ms),
              'result_files_OK' : list(self.result_files_OK)}

        tb = {'test_bench_files_dir' : list(self.test_bench_files_dir),
              'test_bench_files_ms' : list(self.test_bench_files_ms)}

        srl = SummaryReportsLinks(self.result_files_dir)

        lf = {'files_linked_from_sum_reps' : srl.get_files(),
              'folders_linked_from_sum_reps' : srl.get_folders()}

        # 'test_bench_files_pr' : list(self.test_bench_files_pr),
        
        json_test = {'design_files' : df, 'design_space_files' : ds, 'result_files' : rs,
                     'test_bench_files' : tb, 'stat_files' : self.stat_files,
                     'files_linked_from_sum_reps' : lf}

        with open('test_run.json','wb') as f_out:
            json.dump(json_test, f_out, indent=4)

    def run_ui(self):
        print "In order to see all options run this script with input option -h.\n"

        do_svn = False
        do_export = False
        if check_svn():
            print "This folder, {0}, is under subversion control.".format(os.getcwd())
            print "Files will be added to svn."
            #do_svn = prompt_yes_or_no("Would you like to add files to repository?")
            do_svn = True
        else:
            print "Files will be exported."
            #do_export = prompt_yes_or_no("Would you like to export files locally?")
            do_export = True
        """
        if not(do_svn or do_export):
            print "You have selected that no local exporting nor adding files to " \
                  "repository will take place."
            if not prompt_yes_or_no("Do you still want to continue?"):
                return
        """
        # Gather files in directories and project file
        if not self.enter_project_file():
            print "Execution aborted!"
            return

        self.enter_directories()

        # The files in the directories makes up the largest possible set of files
        self.result_files = self.result_files_dir
        self.design_files = self.design_files_dir
        self.design_space_files = self.design_space_files_dir
        self.test_bench_files = self.test_bench_files_dir
        """
        # Check if the files in the directories are all referenced in the project file
        pr1 = self.design_files.issubset(self.design_files_pr)
        pr2 = self.design_space_files.issubset(self.design_space_files_pr)
        #pr3 = self.test_bench_files.issubset(self.test_bench_files_pr)

        #if not (pr1 and pr2 and pr3):

        if not (pr1 and pr2):
            print "Some files in the directories do not have a reference in the project-file."
            if prompt_yes_or_no("Would you like to dismiss those files in the export?"):
                self._pr_intersect()

        # Gather files referenced in the meta-results file
        if not self.enter_meta_results_file():
            print "Execution aborted!"
            return

        # Check if files in "directories" are all referenced in the meta-results file
        ms1 = self.result_files.issubset(self.result_files_ms)
        ms2 = self.design_files.issubset(self.design_files_ms)
        ms3 = self.test_bench_files.issubset(self.test_bench_files_ms)

        if not (ms1 and ms2 and ms3):
            print "Some files in the directories do not have a reference in the " \
                  "meta-results file."
            if prompt_yes_or_no("Would you like to dismiss those files in the export?"):
                self._ms_intersect()

        # Check analysis statuses

        if prompt_yes_or_no("Would you like to check the AnalysisStatus of all "
                            "summary.reports.json?", False):
            self.check_analysis_status()
            all_OK1 = self.design_files.issubset(self.design_files_OK)
            all_OK2 = self.result_files.issubset(self.result_files_OK)
            if all_OK1 and all_OK2:
                print "All AnalysisStatuses are OK..."
            else:
                print "Some AnalysisStatuses are not OK."
                if prompt_yes_or_no("Would you like to dismiss such files?", True):
                    self._OK_intersect()

        project_file_references_valid, meta_results_file_references_valid = \
            self._check_pr_and_ms_references()

        clean_pr = False
        clean_ms = False
        if not(project_file_references_valid and meta_results_file_references_valid):
            # Three possible cases
            if not project_file_references_valid and not meta_results_file_references_valid:
                print "The project-file and meta-results file both contain references to files "\
                      "that will not be exported."
            elif not project_file_references_valid:
                print "The project-file contains references to files that will not be exported."
            else:
                print "The meta-results file contains references to files that will not be "\
                      "exported."

            if prompt_yes_or_no("Would you like to clean up such references inside the file(s)?"
                                , False):
                clean_pr = not project_file_references_valid
                clean_ms = not meta_results_file_references_valid

        if clean_pr:
            self._clean_up_project_file()
        if clean_ms:
            self._clean_up_meta_results_file()

        if do_export and do_svn:
            include_dashboard = prompt_yes_or_no("Would you like to export and add the dashboard "
                                                 "directory to svn?")
        elif do_export:
            include_dashboard = prompt_yes_or_no("Would you like to export the dashboard "
                                                 "directory?")
        elif do_svn:
            include_dashboard = prompt_yes_or_no("Would you like to add the dashboard "
                                                 "directory to svn?")
        """

        # go through all summary.report.json files and extract linked files
        srl = SummaryReportsLinks(self.result_files)
        self.files_linked_from_sum_reps = srl.get_files()
        self.folders_linked_from_sum_reps = srl.get_folders()

        include_dashboard = True
        if do_export:
            self.export_locally(include_dashboard)

        if do_svn:
            self.add_to_svn(include_dashboard)

    def run_silent(self, opts):
        """
        Functions for running a silent execution, all options are set from script call
        options.
        """

        if not opts.add_to_svn and not opts.export_locally:
            print "This execution will probably not have any effect. Make sure at least one of "\
                "the options -l and -a are activated."

        svn_OK = False
        if opts.add_to_svn:
            svn_OK = check_svn()

        # look for and enter project file
        if not self.enter_project_file():
            raw_input("Execution aborted!\nPress any key to close.")
            return

        # which files are in directories
        self.enter_directories()

        self.result_files = self.result_files_dir
        self.design_files = self.design_files_dir
        self.design_space_files = self.design_space_files_dir
        self.test_bench_files = self.test_bench_files_dir

        if opts.project_file_link:
            self._pr_intersect()

        if opts.meta_results_file_link:
            if not self.enter_meta_results_file():
                raw_input("Execution aborted!\nPress any key to close.")
                return
            self._ms_intersect()

        if opts.summary_reports_must_be_ok:
            self.check_analysis_status()
            self._OK_intersect()

        if opts.clean_up_missing_references:
            project_file_references_valid, meta_results_file_references_valid =\
            self._check_pr_and_ms_references()

            if not project_file_references_valid:
                self._clean_up_project_file()
            if not meta_results_file_references_valid:
                self._clean_up_meta_results_file()

        # go through all summary.report.json files and extract linked files
        srl = SummaryReportsLinks(self.result_files)
        self.files_linked_from_sum_reps = srl.get_files()
        self.folders_linked_from_sum_reps = srl.get_folders()

        include_dashboard = not opts.do_not_export_dashboard
        if svn_OK:
            self.add_to_svn(include_dashboard)

        if opts.export_locally:
            self.export_locally(include_dashboard)

    def enter_project_file(self):

        files = glob('*.project.json')

        if len(files) == 0:
            print "\nThere is no x.project.json file in this directory."
            return False
        elif len(files) > 1:
            print "\nThese are multiple x.project.json files in this directory:\n{0}\n".format(files)
            return False
        else:
            self.project_file = files[0]
            print "\nObtaining project data from {0}...\n".format(self.project_file)

        with open(self.project_file,'r') as f_in:
            project_json = json.load(f_in)

        # ASSUMPTION: there is one and only one of each of these files
        self.meta_results_file = project_json['Project']['Results']['UrlHints'][0]
        self.requirements_file = project_json['Project']['Requirements']['UrlHints'][0]

        for design in project_json['Project']['Configurations']:
            self.design_files_pr.add(os.path.basename(design))

        for design_space in project_json['Project']['DesignSpaceModels']:
            self.design_space_files_pr.add(os.path.basename(design_space))

        #for test_bench in project_json['Project']['TestBenches']:
        #    self.test_bench_files_pr.add(os.path.basename(test_bench))
        self._look_for_stat('./')

        return True

    def enter_directories(self):

        root_dir = os.getcwd()
        # get designs
        os.chdir(self.designs_dir)
        self.design_files_dir.update(glob('*metadesign.json') + glob('*adm'))
        self._look_for_stat(self.designs_dir)
        os.chdir(root_dir)

        # get design-spaces
        os.chdir(self.design_space_dir)
        self.design_space_files_dir.update(glob('*metadesign.json') + glob('*adm'))
        self._look_for_stat(self.design_space_dir)
        os.chdir(root_dir)

        # get test-benches
        os.chdir(self.test_benches_dir)
        self.test_bench_files_dir.update(glob('*testbench.json'))
        self._look_for_stat(self.test_benches_dir)
        os.chdir(root_dir)

        # get results
        for (path, dirs, files) in os.walk(self.results_dir):
            os.chdir(self.results_dir)
            self._look_for_stat(self.results_dir)
            for dir in dirs:
                os.chdir(dir)
                sum_rep = 'testbench_manifest.json'
                if os.path.exists(sum_rep):
                    self.result_files_dir.add('./{0}/{1}'.format(dir, sum_rep))
                self._look_for_stat(os.path.join(self.results_dir, dir))
                os.chdir('..')
            os.chdir(root_dir)
            break

        # enter requirements folder and look for stat.json
        os.chdir(self.requirements_dir)
        self._look_for_stat(self.requirements_dir)
        os.chdir(root_dir)

        return True

    def enter_meta_results_file(self):

        if not os.path.exists(self.meta_results_file):
            print 'The file {0} does not exists.'.format(self.meta_results_file)
            return False

        with open(self.meta_results_file, 'r') as f_in:
            meta_results_json = json.load(f_in)

        for item in meta_results_json['Results']:
            self.design_files_ms.add(item['Design'])
            self.test_bench_files_ms.add(os.path.basename(item['TestBench']))
            self.result_files_ms.add(item['Summary'])

        return True

    def check_analysis_status(self):
        root_dir = os.getcwd()
        os.chdir(self.results_dir)
        for sum_rep in self.result_files:
            with open(sum_rep, 'r') as f_in:
                sum_json = json.load(f_in)
                analysis_status = sum_json['AnalysisStatus']
            if analysis_status == 'OK':
                self.result_files_OK.add(sum_rep)
                self.design_files_OK.add(sum_json['Design'])

        os.chdir(root_dir)
        return True

    def add_to_svn(self, include_dashboard=True):

        print "Adding files to repository..."
        if include_dashboard:
            subprocess_call('svn add {0} --force'.format('dashboard'))
            subprocess_call('svn add {0} --force'.format('index.html'))

        # add single files
        subprocess_call('svn add {0} --force'.format(self.project_file))
        subprocess_call('svn add {0} --parents --force'.format(self.requirements_file))
        subprocess_call('svn add {0} --parents --force'.format(self.meta_results_file))

        # build up a single svn add command
        svn_add_command_root = 'svn add --parents --force'
        svn_add_command_files = ''

        # add files inside lists
        for file in self.design_files:
            svn_add_command_files += '\n{0}'.format(os.path.join(self.designs_dir, file))

        for file in self.design_space_files:
            svn_add_command_files += '\n{0}'.format(os.path.join(self.design_space_dir, file))

        for file in self.result_files:
            svn_add_command_files += '\n{0}'.format(os.path.join(self.results_dir, file))

        for file in self.test_bench_files:
            svn_add_command_files += '\n{0}'.format(os.path.join(self.test_benches_dir, file))

        for file in self.stat_files:
            svn_add_command_files += '\n{0}'.format(file)

        for file in self.files_linked_from_sum_reps:
            svn_add_command_files += '\n{0}'.format(file)

        file_list = open('__files_to_add.tmp','w')
        file_list.writelines(svn_add_command_files)
        file_list.close()

        svn_add_command = svn_add_command_root + ' --targets __files_to_add.tmp'
        subprocess_call(svn_add_command)
        os.remove('__files_to_add.tmp')

        print "Adding done! Use your svn-client to commit the added files."

        return True

    def export_locally(self, include_dashboard=True):

        dst_folder_name = '{0}_{1}'.format(DST_FOLDER, time.strftime('%Y%m%d_%H%M%S'))
        dst_dir = os.path.join(os.getcwd(), '..', dst_folder_name)

        # in case the exporter is run twice during the same second append _new
        while os.path.exists(dst_dir):
            dst_dir += '_new'

        os.makedirs(dst_dir)
        print "Exporting files to : {0}".format(os.path.abspath(dst_dir))
        # copy entire dashboard_dir
        if include_dashboard:
            dashboard_dst = os.path.join(dst_dir, self.dashboard_dir)
            shutil.copytree(self.dashboard_dir, dashboard_dst)
            shutil.copy2('index.html', dst_dir)


        # copy single files
        shutil.copy2(self.project_file, dst_dir)

        requirements_dst = os.path.join(dst_dir, self.requirements_dir)
        os.makedirs(requirements_dst)
        shutil.copy2(self.requirements_file, requirements_dst)

        results_dst = os.path.join(dst_dir, self.results_dir)
        os.makedirs(results_dst)
        shutil.copy2(self.meta_results_file, results_dst)

        # copy gathered .json files from sets
        designs_dst = os.path.join(dst_dir ,self.designs_dir)
        os.makedirs(designs_dst)
        for file in self.design_files:
            shutil.copy2(os.path.join(self.designs_dir, file), designs_dst)

        design_space_dst = os.path.join(dst_dir, self.design_space_dir)
        os.makedirs(design_space_dst)
        for file in self.design_space_files:
            shutil.copy2(os.path.join(self.design_space_dir, file), design_space_dst)

        test_benches_dst = os.path.join(dst_dir, self.test_benches_dir)
        os.makedirs(test_benches_dst)
        for file in self.test_bench_files:
            shutil.copy2(os.path.join(self.test_benches_dir, file), test_benches_dst)

        for file in self.result_files:
            sum_rep_dst = os.path.dirname(os.path.join(results_dst, file))
            os.makedirs(sum_rep_dst)
            shutil.copy2(os.path.join(self.results_dir, file), sum_rep_dst)

        for file in self.stat_files:
            stat_file_dst = os.path.dirname(os.path.join(dst_dir, file))
            if not os.path.exists(stat_file_dst):
                os.makedirs(stat_file_dst)
            shutil.copy2(file, stat_file_dst)

        for file in self.files_linked_from_sum_reps:
            linked_file_dst = os.path.dirname(os.path.join(dst_dir, file))
            if not os.path.exists(linked_file_dst):
                os.makedirs(linked_file_dst)
            shutil.copy2(file, linked_file_dst)

        # copy entire folders that are linked from the summary.report.json:s
        #for folder in self.folders_linked_from_sum_reps:
        #    linked_folder_dst = os.path.dirname(os.path.join(dst_dir, folder))
        #    shutil.copytree(folder, linked_folder_dst)

        print "Exportation done!"

    def _pr_intersect(self):
        self.design_files.intersection_update(self.design_files_pr)
        self.design_space_files.intersection_update(self.design_space_files_pr)
        #self.test_bench_files.intersection_update(self.test_bench_files_pr)

    def _ms_intersect(self):
        self.result_files.intersection_update(self.result_files_ms)
        self.design_files.intersection_update(self.design_files_ms)
        self.test_bench_files.intersection_update(self.test_bench_files_ms)

    def _OK_intersect(self):
        self.design_files.intersection_update(self.design_files_OK)
        self.result_files.intersection_update(self.result_files_OK)

    def _clean_up_project_file(self):
        """
        Cleans up broken references inside the project-file and writes out
        the modified dictionary

        """

        print "Reading in project file..."
        with open(self.project_file,'r') as f_in:
            project_json = json.load(f_in)

        # Go through design_files references
        configurations = project_json['Project']['Configurations']
        n = len(configurations)
        indices_to_delete = []
        for i in range(n):
            if not os.path.basename(configurations[i]) in self.design_files:
                indices_to_delete.append(i)

        indices_to_delete.reverse()
        for i in indices_to_delete:
            del configurations[i]

        # Go through design_space_files references
        design_space_models = project_json['Project']['DesignSpaceModels']
        n = len(design_space_models)
        indices_to_delete = []
        for i in range(n):
            if not os.path.basename(design_space_models[i]) in self.design_space_files:
                indices_to_delete.append(i)

        indices_to_delete.reverse()
        for i in indices_to_delete:
            del design_space_models[i]

        # Go through test_bench_files references
        #test_benches = project_json['Project']['TestBenches']
        #n = len(test_benches)
        #indices_to_delete = []
        #for i in range(n):
        #    if not os.path.basename(test_benches[i]) in self.test_bench_files:
        #        indices_to_delete.append(i)
        #
        #for i in indices_to_delete.reverse():
        #    del test_benches[i]

        # Write out the new, reduced in size, project dictionary
        with open(self.project_file,'wb') as f_out:
            json.dump(project_json, f_out, indent=4)

        print "Written out cleaned up project dictionary."

    def _clean_up_meta_results_file(self):
        """
        Cleans up broken references inside the project-file and writes out
        the modified dictionary

        """

        print "Reading in meta-results file..."
        with open(self.meta_results_file, 'r') as f_in:
            meta_results_json = json.load(f_in)

        results = meta_results_json['Results']
        n = len(results)
        indices_to_delete = []
        for i in range(n):
            # Assumption if any file is missing skip entire dictionary item.
            design_valid = results[i]['Design'] in self.design_files
            test_bench_valid = os.path.basename(results[i]['TestBench']) in self.test_bench_files
            sum_rep_valid = results[i]['Summary'] in self.result_files
            if not (design_valid and test_bench_valid and sum_rep_valid):
                indices_to_delete.append(i)

        indices_to_delete.reverse()
        for i in indices_to_delete:
            del results[i]

        # Write out the new, reduced in size, results dictionary
        with open(self.meta_results_file,'wb') as f_out:
            json.dump(meta_results_json, f_out, indent=4)

        print "Written out cleaned up results dictionary."

    def _check_pr_and_ms_references(self):

        # Check if the files referenced in the project file are considered for export
        pr_ref_valid1 = self.design_files_pr.issubset(self.design_files)
        pr_ref_valid2 = self.design_space_files_pr.issubset(self.design_space_files)
        #pr_ref_valid3 = self.test_bench_files_pr.issubset(self.test_bench_files)

        project_file_references_valid = pr_ref_valid1 and pr_ref_valid2 \
        #and pr_ref_valid3

        # Check if the files referenced in the meta-results file are considered for export
        ms_ref_valid1 = self.result_files_ms.issubset(self.result_files)
        ms_ref_valid2 = self.design_files_ms.issubset(self.design_files)
        ms_ref_valid3 = self.test_bench_files_ms.issubset(self.test_bench_files)

        meta_results_file_references_valid = ms_ref_valid1 and ms_ref_valid2 and ms_ref_valid3

        return project_file_references_valid, meta_results_file_references_valid

    def _look_for_stat(self, dir):
        """
        Looks if there exist a json file in the current dir,
        if so appends it to the list of gathered such.
        """

        if os.path.exists('stat.json'):
            self.stat_files.append(os.path.join(dir,'stat.json'))
            return True
        else:
            return False

class SummaryReportsLinks(object):
    """
    N.B! This entire classes operates from %PROJECT_DIR%/results
    """

    files_for_export = []
    folders_for_export = []
    result_files = None

    def __init__(self, result_files):
        """
        Goes through all result_files and gathers files linked from them.
        The links inside files_for_export will have the following form;

        ./results/8 digit folder/some link

        Such that when exported it can be done so directly from project_dir (=root_dir)
        """
        self.result_files = result_files

        root_dir = os.getcwd()
        os.chdir('results')

        self._go_through_summary_reports()

        os.chdir(root_dir)



    def _go_through_summary_reports(self):
        """
        Iterates through all summary.report.json-files
        and gathers linked files into files_for_export.
        """

        for result_file in self.result_files:
            self.cur_8digit_dir = os.path.split(result_file)[0]
            try:
                with open(result_file) as f_in:
                    sum_rep = json.load(f_in)
                if sum_rep.has_key('Artifacts'):
                    for linked_artifact in sum_rep['Artifacts']:
                        artifact_path = linked_artifact['Path']
                        # For now assume only files are linked (no folders)
                        rel_path_from_results = os.path.join(self.cur_8digit_dir, artifact_path)
                        if os.path.exists(rel_path_from_results):
                            self.files_for_export.append(os.path.join('results',
                                rel_path_from_results))
                        if artifact_path.endswith('.json'):
                            function_tag = artifact_path.replace('.','_').replace('/','_')

                            if hasattr(self, function_tag):
                                getattr(self, function_tag)()
            except IOError:
                print '{0} does not exist on this filesystem. I cannot be check for references '\
                      'to other files.'.format(result_file)

    def manufacturing_manifest_json(self):
        """
        Goes through the manufacturing.manifest.json
        """

        file_name = os.path.join(self.cur_8digit_dir, "manufacturing.manifest.json")

        with open(file_name) as f_in:
            man_manifest = json.load(f_in)

        for component in man_manifest["ComponentManufactureList"]:
            # Get ManufacturingModel (if exists)
            if "ManufacturingModel" in component:
                rel_path_from_results = os.path.join(self.cur_8digit_dir, component["ManufacturingModel"])
                if os.path.exists(rel_path_from_results):
                    self.files_for_export.append(os.path.join('results', rel_path_from_results))

            # Get STEPModel (if exists)
            if "STEPModel" in component:
                rel_path_from_results = os.path.join(self.cur_8digit_dir, component["STEPModel"])
                if os.path.exists(rel_path_from_results):
                    self.files_for_export.append(os.path.join('results', rel_path_from_results))

        # Since this is a PrepareForIFAB folder, gather all *.stp files in ./AP203_E2_SEPERATE_PART_FILES/
        path_to_stp_files = os.path.join(self.cur_8digit_dir,'AP203_E2_SEPERATE_PART_FILES')
        filter_for_stp_files = os.path.join(path_to_stp_files,'*.stp')
        stp_files = glob(filter_for_stp_files)
        for stp_file in stp_files:
            self.files_for_export.append(os.path.join('results',stp_file))

        # If there's a TDP tag, 
        #   add the referred file,
	#   then crawl that file and gather its file references
        if "TDP" in man_manifest:
            tdp_json_path = os.path.join(self.cur_8digit_dir,man_manifest["TDP"])
            self.tdp_json(tdp_json_path)
            self.files_for_export.append(os.path.join('results', tdp_json_path))

    # Parse a TDP.manifest file, and gather its file references
    def tdp_json(self,file_name):
        rel_path_from_results = os.path.dirname(file_name)

        with open(file_name) as f_in:
            tdp_manifest = json.load(f_in)

	# Pass the entire data structure into the recursive gathering function
        self.tdp_recursive_gather(tdp_manifest,rel_path_from_results)

    def tdp_recursive_gather(self,obj,rel_path_from_results):
	# Input is a JSON object.
		
        if isinstance(obj,dict):
            # If it's a dictionary,
            #	go through the dictionary items and recursively parse those objects, 
            #	skipping those that have a key of 'Name'
            for k,v in obj.iteritems():
                if k == 'Name':
                    continue
                else:
                    self.tdp_recursive_gather(v,rel_path_from_results)
        elif isinstance(obj,list):
            # If it's a list, parse each element in the list
            for d in obj:
                self.tdp_recursive_gather(d,rel_path_from_results)
        elif isinstance(obj,basestring):
            # If it's a string, then it's the path of a file that should be added.
            # 	We assume that strings that were marked as 'Name' have been ignored.
            linked_file_rel_path = os.path.join(rel_path_from_results, obj)
            if os.path.exists(linked_file_rel_path):
                self.files_for_export.append(os.path.join('results',linked_file_rel_path))

    def _linked_json2(self):
        pass

    def get_files(self):

        return self.files_for_export

    def get_folders(self):

        return self.folders_for_export


## helper_functions

def subprocess_call(command):
    """
    Subprocess call with formatted error handling.

    """
    try:
        return_out = subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True)
        if return_out.strip():
            print return_out
    except subprocess.CalledProcessError, err:
        msg = "Subprocess call failed!"\
              "\n  command       : {0}"\
              "\n  console output: \n\n{1}"\
              "\n  error message : {2}"\
              "\n  arguments     : {3}"\
              "\n  return-code   : {4}\n"\
        .format(err.cmd, err.output, err.message, err.args, err.returncode)
        raise Exception(msg)

    return return_out

def check_svn():
    """
    Check if the current folder is under subversion control

    """

    try:
        return_out = subprocess.check_output('svnversion', stderr=subprocess.STDOUT, shell=True)
        try:
            int(return_out[0])
            print "Current SVN-revision number : {0}".format(return_out)
            return True

        except ValueError:
            print return_out
            print "Looks like this folder is not under revision control.\n"
            return False

    except subprocess.CalledProcessError, err:
        print "Looks like there is no svn-client for command-window installed;"
        print "Command called : {0}".format(err.cmd)
        print "Call output    : {0}".format(err.output)
        return False

def prompt_yes_or_no(question, def_yes=True):
    """
    Ask user a yes or no question
    return with answer

    """
    if def_yes:
        ans = raw_input(question + ' [y] or n: ')
        is_yes = ans == 'y' or ans == ''
        is_no = ans == 'n'
        if not (is_yes or is_no):
            print "Invalid answer! Answer must be n or y or simple enter."
            prompt_yes_or_no(question, def_yes)
        else:
            return is_yes
    else:
        ans = raw_input(question + ' y or [n]: ')
        is_yes = ans == 'y'
        is_no = ans == 'n' or ans == ''
        if not (is_yes or is_no):
            print "Invalid answer! Answer must be n or y or simple enter."
            prompt_yes_or_no(question, def_yes)
        else:
            return is_yes

def generate_config_template():
    """
    Generates new template file for the -f argument

    """
    lines = ['# Lines starting with # will be skipped.']
    lines.append('# Only one argument on each line.')
    lines.append('#-s This option is always assumed to be true.')
    lines.append('#-p')
    lines.append('#-m')
    lines.append('#-o')
    lines.append('#-c')
    lines.append('-l')
    lines.append('#-a')
    lines.append('#-d')

    with open('export_config.txt', 'wb') as f_new:
        f_new.write('\r\n'.join(lines))
    print 'Template generated. Edit this file as you please and call this script '\
        'with the -f option enabled.'

def call_for_options(parser):
    """
    Currently contains 9 options

    """

    parser.add_option("-s", "--silent", action="store_true", default=False,
        help='No prompting will be made during execution. And the options set here '
             'will be used during execution. [default: False]')
    parser.add_option("-p", "--project_file_link", action="store_true", default=False,
        help='For those .json-files that CAN be referenced from the project.json-file, only '
             'those that ARE linked will be considered. [default: False]')
    parser.add_option("-m", "--meta_results_file_link", action="store_true", default=False,
        help='For those .json-files that CAN be referenced from the metaresults.json-file, only '
             'those that ARE linked will be considered. [default: False]')
    parser.add_option("-o", "--summary_reports_must_be_ok", action="store_true", default=False,
        help='Check the "AnalysisStatus" of testbench_manifest.json and only consider those that '
             'have the value "OK". [default: False]')
    parser.add_option("-c", "--clean_up_missing_references", action="store_true", default=False,
        help='Remove the references inside the project.json-file and metaresults.json-file, if '
             'the referenced files do not exist.')
    parser.add_option("-l", "--export_locally", action="store_true", default=False,
        help='Export files locally. [default: False]')
    parser.add_option("-a", "--add_to_svn", action="store_true", default=False,
        help='Add files to the svn. [default: False]')
    parser.add_option("-d", "--do_not_export_dashboard", action="store_true", default=False,
        help='Active this option if the dashboard folder should not be exported nor added to svn. '
             '[default: False]')
    parser.add_option("-f", "--read_opts_from_file", action="store_true", default=False,
        help ='With this flag the options will be parsed from a file with name "export_config.txt". '
              'If this is enabled all options will be overridden by those set in the file. '
              '(Run with this option enabled to generate a template file.) [default: False]')
    parser.add_option("-t", "--run_test", action="store_true", default=False,
        help ='Perform a test run, where all different lists of .json-files are printed '
              'into test_run.json. [default: False]')

    (opts, args) = parser.parse_args()

    return opts

## Main function
def main():
    from optparse import OptionParser
    usage = "usage: %prog [options]\n Include options that you want activated, e.g. %prog -s -l -d, will "\
            "run silent and export locally without exporting the dashboard folder."
    parser = OptionParser(usage=usage)
    opts = call_for_options(parser)
    if opts.read_opts_from_file:
        if not os.path.exists('export_config.txt'):
            print 'No export_config.txt exists. \nWriting out template...'
            generate_config_template()
            return
        else:
            with open('export_config.txt', 'r') as f_in:
                file_arguments = ['-s']
                for line in f_in.readlines():
                    arg = line.strip()
                    if arg.startswith('#') or arg == '-f' or arg == '':
                        continue
                    if not arg in file_arguments:
                        file_arguments.append(arg)
            (opts, args) = parser.parse_args(file_arguments)

    exporter = Exporter()
    if opts.silent:
        exporter.run_silent(opts)
    elif opts.run_test:
        exporter.run_test()
    else:
        exporter.run_ui()
        raw_input("Press any key to close.")

## Main entry when script called
if __name__ == '__main__':

    main()
    
