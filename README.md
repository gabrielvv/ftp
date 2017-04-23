# ftp
[RFC](https://www.ietf.org/rfc/rfc959.txt)

### Build
```sh
mkdir build && cd build
cmake ..
cmake --build . &> build.log.txt 
```

### Test ( using [Criterion](https://github.com/Snaipe/Criterion) )
run ```ctest``` in the build folder or execute one of the executable present in the build/test folder

# Scanning ports with netcat
```sh
# www.cyberciti.biz/faq/linux-port-scanning/
 nc -z -v 127.0.0.1 1-3000
```
