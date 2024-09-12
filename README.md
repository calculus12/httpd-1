## Simpe HTTP 1.0 Concurrent Server Only support for GET, HEAD

### Getting started
<code style="color : red">Warning: Do not open this server to the Internet.</code>
```sh
$ ./httpd --port 8080 --debug <document root>
```
**Option arguments**
- port
  - specify port that server socket will listen
- chroot
  - enable chroot(not default)
- user, group
  - After calling `chroot`, change to less authorized credentials (specify the user name and group name)
- debug
  - use standard streams to debug (no syslog)

### Used technique
- #### Concurrent server
  - use `fork()` to implement concurrent server
  - make children process to respond (write to socket)
  - parent process become daemon
- #### Memory allocation
  - allocate and free memories for string(`char*`), custom structs(e.g. `HTTPRequest`)
- #### Option arguments parsing
  - `getopt_long`
- #### Signal handling
  - register custom signal handler for `SIGCHLD` with special flag `SA_NOCLDWAIT`
  - parent process dosen't need to wait children process which is reponsible for responding
- #### Sockets
  - Server sockets: `getaddrinfo()`, `socket()`, `bind()`, `listen()`
- #### File handling
  - use basic APIs (e.g. open, read, write, etc..)for regular files and sockets
  - handling file metadata using `lstat` 
- #### Logging API
  - use `openlog()` to log syslog
  
