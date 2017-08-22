import sys
import datetime
import urlparse
import json
import os.path
import sqlite3

from delete_old_jobs import get, post, root_url


if __name__=='__main__':
    jobs = {}

    with sqlite3.connect('jobs.db') as conn:
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS jobs (name text, json text)''')
        root = get(root_url + '/api/json?depth=0')
        i = 0
        for job in json.loads(root.content)['jobs']:
        #for (i, job) in enumerate(json.load(open('jenkins_jobs_2013-01-22T092757.744000.json')).values()):
            previous = list(c.execute('SELECT json FROM jobs WHERE name = ?', (job['name'],)))
            if previous:
                previous_job = json.loads(previous[0][0])
                if previous_job['lastCompletedBuild'] and not previous_job['lastCompletedBuild']['building']:
                    print "skip " + previous_job['name']
                    continue
            print job['name']
            job['name'] = job['name'].decode('UTF-8')
            c.execute('DELETE FROM jobs WHERE name = ?', (job['name'],))
            c.execute('INSERT INTO jobs VALUES (?, ?)', (job['name'], get(job['url'] + 'api/json/?depth=5').content.decode('UTF-8')))
            #c.execute('INSERT INTO jobs VALUES (?, ?)', (job['name'], json.dumps(job).decode('UTF-8')))
            i += 1
            if i % 20 == 0:
                conn.commit()

        conn.commit()
        #conn.execute('vacuum')
        #conn.commit()