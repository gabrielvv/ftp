# ftp
[RFC](https://www.ietf.org/rfc/rfc959.txt)

### Build
```sh
mkdir build && cd build
cmake ..
cmake --build . &> build.log.txt

gcc pi_svr_main.c pi_server.c pi_commons.c ftp_commons.c -o pi_svr
gcc pi_cli_main.c pi_client.c pi_commons.c ftp_commons.c -o pi_cli

```

### Code organization
* FTP COMMONS: fonctions de base d'initialisation et gestion de connections client-serveur entre sockets
* DTP COMMONS: macros et fonctions communes à la partie Data Protocol Interpreter
	* DTP CLIENT: fonctions propres au client DTP
	* DTP SERVER: fonctions propres au serveur DTP
* PI COMMONS: macros et fonctions communes à la partie Protocol Interpreter
	* PI CLIENT: fonctions propres au client PI
	* PI SERVER: fonctions propres au serveur PI

### Test ( using [Criterion](https://github.com/Snaipe/Criterion) )
run ```ctest``` in the build folder or execute one of the executable present in the build/test folder

### Scanning ports with netcat
```sh
# www.cyberciti.biz/faq/linux-port-scanning/
 nc -z -v 127.0.0.1 1-3000
```
