from __future__ import print_function, unicode_literals, absolute_import

import io
import os

from jinja2 import Environment, PackageLoader, select_autoescape
env = Environment(
    loader=PackageLoader('testbenchexecutor', 'templates'),
    autoescape=select_autoescape(['html', 'xml'])
)

def generate_html_report(manifest_path, manifest_dict):
    # Fetch last ten lines of log files
    logFileExcerpts = {}
    logFileHeaders = {}

    manifest_dir = os.path.dirname(os.path.abspath(manifest_path))

    for step in manifest_dict["Steps"]:
        logPath = step["LogFile"]
        if logPath != None:
            try:
                with io.open(os.path.join(manifest_dir, logPath), "r", encoding="utf-8") as logread:
                    loglines = logread.readlines()
                    if len(loglines) > 10:
                        header = 'Partial log:'
                    else:
                        header = 'Log contents:'
                    excerpt = loglines[-10:]

                logFileExcerpts[logPath] = ''.join(excerpt)
                logFileHeaders[logPath] = header
            except IOError as err:
                logFileExcerpts[logPath] = "Log file failed to load:\n\n{0}".format(err)
                logFileHeaders[logPath] = "Log file missing:" 

    template = env.get_template("report.html")

    with io.open(os.path.join(os.path.dirname(os.path.abspath(manifest_path)), "_REPORT.html"), "w", encoding="utf-8") as reportFile:
        reportFile.write(template.render({
            "manifest": manifest_dict,
            "logFileExcerpts": logFileExcerpts,
            "logFileHeaders": logFileHeaders
        }))
