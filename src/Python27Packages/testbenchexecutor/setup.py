'''
Build with:
setup.py bdist_wheel
'''
from setuptools import setup

setup(
    name='testbenchexecutor',
    version='0.1.0',
    author='MetaMorph Software, Inc',
    author_email='adam@metamorphsoftware.com',
    description='Executes the steps in a Test Bench manifest',
    packages=['testbenchexecutor'],
    package_dir={'testbenchexecutor': ''},
    entry_points = {
        "console_scripts": [
            "testbenchexecutor = testbenchexecutor.__main__:main",
        ]
    }
)
