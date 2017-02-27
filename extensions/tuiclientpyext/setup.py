# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension

module = Extension('tuiclient',
                    libraries = ['TUIFrameworkClient', 'TUIFrameworkCore', 'pthreadVCE2','Ws2_32','TUILibTUITypes'],
                    include_dirs = ['../../include',
					'../../3rdParty/Pre-built.2/include'],
                    library_dirs = ['../../bin/Win32/14.0/x86/Debug/lib/TUIPlugins/static',
					'../../bin/Win32/14.0/x86/Debug/lib/TUIFramework',
					'../../3rdParty/Pre-built.2/lib'],
					define_macros=[('HAVE_STRUCT_TIMESPEC', None),
									('WIN32', None),
									('NDEBUG',None),
									('_WINDOWS', None),
									('_MBCS', None)],
                    sources = ['./src/TUIClientPythonModule.cpp', './src/TUIClient.cpp', './src/EventDelegationFactory.cpp', './src/eventDelegationRegistration.cpp'],
                    language = "c++")

setup (name = 'tuiclient',
       version = '1.0',
       description = 'TUI Client',
       ext_modules = [module])