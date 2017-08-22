import sys
import datetime
import urlparse
import json
import itertools
import glob
import datetime
import sqlite3

from delete_old_jobs import get, post, root_url

from collections import defaultdict, OrderedDict

results = defaultdict(int)
users = defaultdict(int)
dates = OrderedDict()

if __name__=='__main__':
    conn = sqlite3.connect('jobs.db')
    c = conn.cursor()
    for row in c.execute('SELECT name,json from jobs'):
        job_name,job = row
        job = json.loads(job)
        print job['url']
        if job['lastCompletedBuild'] and not job['lastCompletedBuild']['building']:
            build = job['lastCompletedBuild']
            #print build['result']
            results[build['result']] = results[build['result']] + 1
            causes = [d for d in build['actions'] if isinstance(d, dict) and 'causes' in d]
            if causes:
                user = [d.get('userName', 'unknown') for d in causes[0].get('causes', {}) if isinstance(d, dict) and 'userName' in d]
                user = (user or ['unknown'])[0]
                #print user
                users[user] += 1
            #import pdb;pdb.set_trace()
            date = datetime.datetime.fromtimestamp(job['lastCompletedBuild']['timestamp']/1000).date().isoformat()
            dates[date] = dates.get(date, 0) + 1
        else:
            results['NOT_BUILT'] = results['NOT_BUILT'] + 1

#lastSuccessfulBuild':{'number':1,'url':'http://10.2.204.106:8080/job/Sandeep_Neema_CyPhy2Modelica_NewComponentAssembly_cfg18_00051_C52EA554/1/'}       
#{'causes':[{'shortDescription':'Started by user neemask','userName':'neemask'}

    print dict(results)
    print dict(users)
    print dates