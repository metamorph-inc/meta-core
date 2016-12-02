import glob
import json
import logging
import os
import subprocess
import sys

# create logger with 'source_code_analysis'
logger = logging.getLogger('source_code_analysis')
logger.setLevel(logging.DEBUG)

# create file handler which logs even debug messages
fh = logging.FileHandler('source_code_analysis.log')
fh.setLevel(logging.DEBUG)

# create console handler with a higher log level
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)

# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
ch.setFormatter(formatter)

# add the handlers to the logger
logger.addHandler(fh)
logger.addHandler(ch)


def main():

    try:
        projects_to_check = {}
        with open('projects_to_check.json', 'r') as f_p:
            projects_to_check = json.load(f_p)

        # delete all *.pylint.* and *.fxcop-results.* files
        files_to_delete = list()
        files_to_delete += glob.glob('*.pylint.*')
        files_to_delete += glob.glob('*.fxcop-result.*')

        logger.info('Deleting previous results ...')
        for file_to_delete in files_to_delete:
            logger.info('Deleting {0} ...'.format(file_to_delete))
            os.unlink(file_to_delete)

        for programming_language in projects_to_check:
            logger.info('Checking {0} ...'.format(programming_language))
            language = projects_to_check[programming_language]
            if not 'projects' in language:
                logger.warning('No projects for {0}.'.format(programming_language))
                continue

            for project in language['projects']:
                if not 'command' in language:
                    logger.warning('No command specified for {0}.'.format(programming_language))
                    logger.warning('Skipping project: '.format(project))
                    continue
                cmd = language['command'].format(**project)
                logger.info('Running {0}'.format(cmd))
                output = subprocess.call(cmd, stderr=subprocess.STDOUT, shell=True)
                logger.info('Output: {0}'.format(output))

    except Exception as ex:
        logger.error(ex)
        logger.error(sys.exc_info())

    return 0

if __name__ == '__main__':
    sys.exit(main())