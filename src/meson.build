# Project sources
#
sca3300_sources = ['./sca3300.cpp', './sca3300-tools.cpp']

# Static Library
#
sca3300_static_lib = static_library('sca3300',sca3300_sources)
sca3300_dep = declare_dependency(link_with: sca3300_static_lib, 
                             include_directories: include_directories('.'))

# Shared Library
#
sca3300_shared_lib = shared_library('sca3300_sha',sca3300_sources)
