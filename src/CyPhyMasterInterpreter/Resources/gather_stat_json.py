import os
import glob
import zipfile
import datetime
import json


class GatherStatJsonFiles(object):
    """
    Class definition

    """
    # hard-coded names
    compiled_json_name = 'Stats.json'
    local_archive_name = 'Stats'
    right_now = ''
    # paths
    project_root = ''
    path_to_stats_folder = ''
    # lists
    stat_json_paths = []
    # dictionaries
    local_json_dict = {}
    jenkins_json_dict = {}

    def __init__(self):
        """
        Constructor

        """
        self.project_root = os.path.abspath(os.pardir)
        self.path_to_stats_folder = os.path.join(self.project_root, 'stats')
        self.right_now = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
        self.local_json_dict = {'stats': {},
                               'TimeStamp': self.right_now,
                               }
        self.jenkins_json_dict = {'stats': {},
                                  'TimeStamp': self.right_now,
                                  }

    def get_local_stats(self):
        """
        make a single dictionary from the local stat.json files

        """
        # use os.walk to go through the tree beginning at project_root and find "stat.json"
        os.chdir(self.project_root)
        os.chdir('..')
        for dir_name, subdirs, files in os.walk(self.project_root):
            for file_name in files:
                if file_name == 'stat.json':
                    rel_path = os.path.relpath(dir_name, self.project_root)
                    self.stat_json_paths.append(rel_path)

        os.chdir(self.project_root)
        for stat_json_path in self.stat_json_paths:
            os.chdir(stat_json_path)
            with open('stat.json', 'r') as f_in:
                stat = json.load(f_in)
            self.local_json_dict['stats'].update({os.path.basename(stat_json_path): stat})
            os.chdir(self.project_root)

    def write_stat_jsons(self):
        """
        write out the .json files from the stat dictionaries

        """
        os.chdir(self.project_root)

        if not os.path.exists(self.path_to_stats_folder):
            os.makedirs(self.path_to_stats_folder)
        os.chdir(self.path_to_stats_folder)
        if self.local_json_dict['stats']:
            with open(self.compiled_json_name, 'wb') as f_out:
                json.dump(self.local_json_dict, f_out)

    def compute_statistics(self):
        """
        In this method, we will compute statistics from the master Stats.json

        """
        os.chdir(self.path_to_stats_folder)

    def make_stats_archive(self):
        """
        Write out main_dict as a new file, "stats.zip"

        """
        os.chdir(self.path_to_stats_folder)
        local_json_archive = self.local_archive_name + '_' + self.right_now + '.zip'

        if os.path.exists(self.compiled_json_name):
            with zipfile.ZipFile(local_json_archive, 'w') as zf:
                zf.write(self.compiled_json_name, compress_type=zipfile.ZIP_DEFLATED)
            os.remove(self.compiled_json_name)


if __name__ == '__main__':
    gsf = GatherStatJsonFiles()
    gsf.get_local_stats()
    gsf.write_stat_jsons()
    gsf.make_stats_archive()
