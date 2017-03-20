# -*- coding: utf-8 -*-
from distutils.core import setup
from distutils.extension import Extension

module = Extension('tuiclient',
                    libraries = ['TUIFrameworkClientD', 'TUIFrameworkCoreD', 'pthreadVCE2','Ws2_32','TUILibTUITypesD'],
                    include_dirs = ['../../src',
                    '../../include',
					'../../platforms/MSVC2015/3rdParty/Pre-built.2/include'],
                    library_dirs = ['../../install/bin/x86/TUIFramework',
					'../../install/lib/x86/TUIFramework',
					'../../platforms/MSVC2015/3rdParty/Pre-built.2/lib'],
					define_macros=[('HAVE_STRUCT_TIMESPEC', None),
									('WIN32', None),
									('NDEBUG',None),
									('_WINDOWS', None),
									('_MBCS', None)],
                    sources = ['../../src/TUIExampleClients/TUIPythonClient/TUIClientPythonModule.cpp',
                    '../../src/TUIExampleClients/TUIPythonClient/TUIClient.cpp',
                    '../../src/TUIExampleClients/TUIPythonClient/EventDelegationFactory.cpp',
                    '../../src/TUIExampleClients/TUIPythonClient/eventDelegationRegistration.cpp'],
                    language = "c++")

setup (name = 'tuiclient',
       version = '1.0',
       description = 'TUI Client',
       ext_modules = [module])