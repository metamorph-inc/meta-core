#!/home/ubuntu/requests/bin/python

import jenkinsapi
from jenkinsapi.jenkins import Jenkins
import requests
import time
import calendar
import datetime
import six


old_threshold = datetime.timedelta(hours=24)

def munge_url(url):
    return url.replace("10.2.204.106:8080", "localhost:9999")

import jenkinsapi.jenkinsbase
def _poll(self):
    url = self.python_api_url(self.baseurl)
    url = munge_url(url)
    #print url
    return self.get_data(url)

jenkinsapi.jenkinsbase.JenkinsBase._poll = _poll

import jenkinsapi.build
def _poll(self):
    #For build's we need more information for downstream and upstream builds
    #so we override the poll to get at the extra data for build objects
    url = self.python_api_url(self.baseurl) + '?depth=2'
    url = munge_url(url)
    #print url
    return self.get_data(url)

jenkinsapi.build.Build._poll = _poll


j = Jenkins('http://localhost:9999')
execs = []
for (name, node) in six.iteritems(j.get_nodes()):
    execs.extend((exec_ for exec_ in j.get_executors(name).__iter__() if not exec_.is_idle()))

for exec_ in execs:
    exec_url = exec_.get_current_executable()['url']
    timestamp = requests.get(munge_url(exec_url + 'api/json')).json()['timestamp']
    delta = datetime.timedelta(seconds=calendar.timegm(time.gmtime()) - timestamp/1000)
    if delta > old_threshold:
        print exec_url + " " + str(delta)

#import pytz
#naive_timestamp = datetime.datetime(*time.gmtime(timestamp / 1000.0)[:6])
#pytz.utc.localize(naive_timestamp)

    
#for (job_name, job) in j.get_jobs():
#    if job.is_running():
#        print job_name

