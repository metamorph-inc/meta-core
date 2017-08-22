import os
import glob
import zipfile
import datetime
import json
import urlparse


root_url = 'http://129.59.105.108:8080/api/json'
username = 'zsolt'

def _request(method, *args, **kwargs):
    args = list(args)
    # TODO: configure for firewall/reverse proxy/etc
    #args[0] = args[0].replace(':8080', '')
    url = urlparse.urlparse(args[0])
    rurl = urlparse.urlparse(root_url)
    args[0] = urlparse.ParseResult(rurl.scheme, rurl.netloc, url.path, url.params, url.query, url.fragment).geturl()
    kwargs.setdefault('cookies', {})
    kwargs['cookies'].update(dict(username=username))
    kwargs.setdefault('headers', {})
    kwargs['headers']['X-Forwarded-User'] = username
    import requests
    m = getattr(requests, method)
    ret = m(*args, **kwargs)
    if ret.status_code > 399:
        ret.raise_for_status()
    return ret

def get(*args, **kwargs):
    return _request('get', *args, **kwargs)

class GatherStatJsonFiles(object):
    """
    Class definition

    """
    # hard-coded names
    compiled_json_name = 'Stats.json'
    compiled_json_name_jenkins = 'Jenkins_Stats.json'
    local_archive_name = 'Stats'
    jenkins_archive_name = 'JenkinsStats'
    right_now = ''
    jenkins_root_url = 'http://129.59.105.108:8080/api/json'
    # paths
    script_home = ''  # where this script is located
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
        self.script_home = os.path.abspath(os.getcwd())
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
        make a single dictionary from the local stat.json files, should only be used from a GME project root folder

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

        if self.local_json_dict['stats']:
            if not os.path.exists(self.path_to_stats_folder):
                os.makedirs(self.path_to_stats_folder)
            os.chdir(self.path_to_stats_folder)
            with open(self.compiled_json_name, 'wb') as f_out:
                json.dump(self.local_json_dict, f_out)

        if self.jenkins_json_dict['stats']:
            os.chdir(self.script_home)
            with open(self.compiled_json_name_jenkins, 'wb') as f2_out:
                json.dump(self.jenkins_json_dict, f2_out)

    def compute_statistics(self):
        """
        In this method, we will compute statistics from the master Stats.json

        """
        os.chdir(self.path_to_stats_folder)

    def make_stats_archive(self):
        """
        Write out main_dict as a new file, "stats.zip"

        """
        local_json_archive = self.local_archive_name + '_' + self.right_now + '.zip'
        jenkins_json_archive = self.jenkins_archive_name + '_' + self.right_now + '.zip'
        
        if os.path.exists(self.path_to_stats_folder):
            os.chdir(self.path_to_stats_folder)
            if os.path.exists(self.compiled_json_name):
                with zipfile.ZipFile(local_json_archive, 'w') as zf:
                    zf.write(self.compiled_json_name, compress_type=zipfile.ZIP_DEFLATED)
                os.remove(self.compiled_json_name)

        os.chdir(self.script_home)
        if os.path.exists(self.compiled_json_name_jenkins):
            with zipfile.ZipFile(jenkins_json_archive, 'w') as zf2:
                zf2.write(self.compiled_json_name_jenkins, compress_type=zipfile.ZIP_DEFLATED)
            os.remove(self.compiled_json_name_jenkins)

    def get_stats_from_Jenkins(self, number_recent_jobs=10):
        """
        collect the stat.json files from remote jobs at jenkins_root_url
        [number_recent_jobs = 0] returns all jobs

        """
        root = get(self.jenkins_root_url)
        api_json = 'api/json/'
        add_stat = False
        i_stat = 0
        for job in json.loads(root.content)['jobs']:
            add_stat = False
            job_info = get(job['url'] + api_json)
            last_build_success = json.loads(job_info.content)['lastSuccessfulBuild']
            if not last_build_success:
                continue
            last_build_url = last_build_success['url']  # job['url'] + '1/'
            build_info = get(last_build_url + api_json)
            stat_json_url = ''
            stat_json = ''
            for artifact in json.loads(build_info.content)['artifacts']:
                if artifact['fileName'] == 'stat.json':
                    stat_json_url = last_build_url + 'artifact/' + artifact['relativePath']
                    stat_json = json.loads(get(stat_json_url).content)  # create dictionary
                    self.jenkins_json_dict['stats'].update({job['name']: stat_json})
                    add_stat = True
                    i_stat += 1
                    print i_stat
                    break

            if add_stat == True:
                if i_stat == number_recent_jobs:
                    break



        return 0

if __name__ == '__main__':
    gsf = GatherStatJsonFiles()
    #gsf.get_local_stats()
    gsf.get_stats_from_Jenkins(0)  # if no argument, gets 10 jobs as default
    gsf.write_stat_jsons()
    gsf.make_stats_archive()
