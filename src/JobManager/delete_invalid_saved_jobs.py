import sys
import datetime
import six.moves.urllib.parse
import json
import os.path
import sqlite3

from delete_old_jobs import get, post, root_url


if __name__=='__main__':
    jobs = {}

    with sqlite3.connect('jobs.db') as conn:
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS jobs (name text, json text)''')
        print("Number of jobs before: " + str(list(c.execute('SELECT COUNT(*) FROM jobs', ()))[0]))
        for cp in list(range(0, 0x1F + 1)) + [0x22, 0x5c]:
        #for cp in [0x1b]:
            if cp in (0, 0x22, 0x5c): continue
            jobs = list(c.execute('SELECT name FROM jobs WHERE json LIKE ?', ('%' + chr(cp) + '%',)))
            print(hex(cp) + " " + repr(jobs))
            if jobs:
                res = c.execute('UPDATE jobs SET json = replace(json, ?, "") WHERE json LIKE ?', (chr(cp), '%' + chr(cp) + '%'))
                print(res.rowcount)
            #del_ = c.execute('DELETE FROM jobs WHERE name = ?', (job['name'],))
            
            #print del_

        print("Number of jobs before: " + str(list(c.execute('SELECT COUNT(*) FROM jobs', ()))[0]))
        print("Press enter to commit")
        sys.stdin.readline()
        conn.commit()
        #conn.execute('vacuum')
        #conn.commit()
