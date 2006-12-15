#!/bin/env python

import os
import sys

version="0.1.3"

#######################################
# OPTIONS
#######################################
opts = Options(ARGUMENTS)
# conditionally build parts of gaia
#opts.Add(BoolOption('gaia', 'Compile Qt gaia client', 1));

# conditional features
opts.Add(BoolOption('gpsd', 'Compile with gpsd support (libgps)', 0));
opts.Add(BoolOption('extras', 'Compile examples and tests', 0));
opts.Add(BoolOption('use_env', 'Use environment variables (CC, CXX, CCFLAGS, CXXFLAGS, CPPPATH, LIBPATH, QTDIR)', 0));

# paths
opts.Add(('prefix', 'Install prefix', ''));
opts.Add(('bindir', 'Path (relative to prefix) where to install executables to (default bin)', 'bin'));
opts.Add(('datadir', 'Path (relative to prefix) where to install read-only data to (default share/gaia)', 'share/gaia'));

#######################################
# ENVIRONMENT & SYSTEM_SPECIFIC CONFIGURATION
#######################################
env = Environment( CCFLAGS = [ '-Wall', '-g' ],
		   CXXFLAGS = [ '-Wall', '-g' ],
		   QTDIR = '/usr/X11R6',
		   options = opts )

if env['prefix'] == "":
	env['prefix'] = '.'
	env['bindir'] = '.'
	env['datadir'] = 'data'

SConsignFile()

if os.path.exists("/usr/local/bin/moc"):
	env['QTDIR'] = '/usr/local'
elif os.path.exists("/usr/bin/moc"):
	env['QTDIR'] = '/usr'

if not env.GetOption('clean'):
	if sys.platform.startswith('freebsd'):
		print "Compiling for FreeBSD"
		env.Append(CPPPATH = [ '/usr/local/include', '/usr/X11R6/include' ])
		env.Append(LIBPATH = [ '/usr/local/lib', '/usr/X11R6/lib' ])
	elif sys.platform == 'openbsd':
		# TODO: confirm that sys.platform == 'openbsd' for OpenBSD (feedback welcome)
		print "Compiling for OpenBSD"
		env.Append(CPPPATH = [ '/usr/local/include', '/usr/X11R6/include' ])
		env.Append(LIBPATH = [ '/usr/local/lib', '/usr/X11R6/lib' ])
	elif sys.platform == 'darwin':
		print "Compiling for MacOSX"
		env.Append(CPPPATH = [ '/usr/local/include', '/usr/X11R6/include', '/opt/local/include', '/sw/include' ])
		env.Append(LIBPATH = [ '/usr/local/lib', '/usr/X11R6/lib', '/opt/local/lib', '/sw/lib' ])
	else:
		print "Compiling for Unix/Posix/Linux"
		env.Append(CPPPATH = [ '/usr/local/include', '/usr/X11R6/include' ])
		env.Append(LIBPATH = [ '/usr/local/lib', '/usr/X11R6/lib' ])

if int(env['use_env']):
	for key in ['CC', 'CCFLAGS', 'CXX', 'CXXFLAGS', 'CPPPATH', 'LIBPATH', 'QTDIR' ]:
		if os.environ.has_key(key):
			env.Replace( **{key: os.environ[key].split(' ')} )

env.Append(CCFLAGS = [ '-DGAIA_VERSION="\\"' + version + '\\""' ])
env.Append(CXXFLAGS = [ '-DGAIA_VERSION="\\"' + version + '\\""' ])

#######################################
# TARGETS
#######################################

libwwfetch = SConscript('lib/wwfetch/SConscript', exports = ['env'])

SConscript('programs/gaia/SConscript', exports = ['env', 'libwwfetch'])

if env['prefix'] != ".":
	env.Alias('install', env['prefix'])
