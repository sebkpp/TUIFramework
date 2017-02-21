@Echo off

rmdir /S /Q build

:: Build of the Python Interface
title Python Build
python35_d setup.py build --debug install
pause