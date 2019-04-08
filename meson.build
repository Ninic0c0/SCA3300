# Main meson.build file
#
project('Hello_sca3300', 'cpp', default_options : ['cpp_std=c++14'],
        version : '0.1')

# Subfolder with own meson.build file ( like recursive makefile )
#
subdir('src')
subdir('example')
subdir('tests')

# Project sources
#
sca3300_sources = ['.src/sca3300.cpp', '.src/sca3300-tools.cpp']
