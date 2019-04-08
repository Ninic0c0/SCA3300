test=executable('sca3300-test', sources : ['sca3300.test.cpp'],
          link_with : sca3300_static_lib,
          include_directories: include_directories('../src'))

# Test execution 
#
test('SCA3300 test', test)

# We can specify other test execution passing arguments or environment variables
#
#test('SCA3300 test with args and env', test, args : ['arg1', 'arg2'], env : ['FOO=bar'])
