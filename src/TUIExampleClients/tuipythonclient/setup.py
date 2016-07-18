from distutils.core import setup, Extension

module1 = Extension('consoleAction',
                    sources = ['consoleAction.cpp', 'TUIMouse.cpp'], 
					include_dirs=['C:/Users/keppseba/Documents/TUI2/include',
								'C:/Users/keppseba/Documents/TUI2/3rdParty/Pre-built.2/include'],
					define_macros=[('HAVE_STRUCT_TIMESPEC', None),
									('WIN32', None),
									('NDEBUG',None),
									('_WINDOWS', None),
									('_MBCS', None)],
					library_dirs=['C:/Users/keppseba/Documents/TUI2/bin/lib/MSVC2015/TUIFramework/Release',
									'C:/Users/keppseba/Documents/TUI2/bin/lib/MSVC2015/TUIPlugins/Release',
									'C:/Users/keppseba/Documents/TUI2/3rdParty/Pre-built.2/lib',
									'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10150.0/ucrt/x86',
									'C:/Program Files (x86)/Python35-32/libs'],
					libraries=['pthreadVCE2','TUIFrameworkCore','TUIFrameworkClient','TUILibTUITypes','Ws2_32','python35']
					)

setup (name = 'consoleAction',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])