## How to compile and run tests (cross compile)

## Compile
[nicolas@lib]% ninja -C buildcross && scp -o IdentitiesOnly=yes buildcross/src/tests/sca3300-test pi@1.2.3.4:/home/pi

## Run
[nicolas@lib]% sca3300-test --reporter compact --success

## Expected Output
pi@raspberrypi:~ $ ./sca3300-test --reporter compact
Passed all 3 test cases with 60 assertions.
