set PYTHONPATH=%PYTHONPATH%;..\generated\BlockTemplate\Python;..\src\lib\BlockTemplate\Python
echo %PYTHONPATH%

python generate_dist.py py2exe