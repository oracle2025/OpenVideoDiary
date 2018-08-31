#!/usr/bin/env python
# Build the project on AppVeyor.

import os
from subprocess import check_call

print "hello from appveyor.helper.py"

config = os.environ['CONFIG']
path = os.environ['PATH']
generator = os.environ['GENERATOR']
if os.environ['APPVEYOR_REPO_TAG'] == 'true':
  tag = os.environ['APPVEYOR_REPO_TAG_NAME']
else:
  tag = 'v' + os.environ['appveyor_build_version']
cmake_command = ['cmake', '-G', generator, '-DCMAKE_BUILD_TYPE=' + config, '-DTAG='+tag]

os.environ['PATH'] = r'C:\Program Files (x86)\MSBuild\14.0\Bin;' + path
build_command = ['msbuild', '/m:4', '/p:Configuration=' + config, 'OpenVideoDiary.sln']
#test_command = ['msbuild', 'RUN_TESTS.vcxproj']

pack_command = ['cpack', '-C', config]

check_call(cmake_command)
check_call(build_command)
check_call(pack_command)
#check_call(test_command)
