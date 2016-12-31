
## Instructions to run the smoke test

### Usage
```sh
$ ./test.sh <path-to-smoke-test>
```
If the option `<path-to-smoke-test>` is empty, then the current directory
`./` is taken as input.
 
### Exit Codes

0. test passed
1. the code under test doesn't compile 
2. the test itself doesn't compile
3. test failed
