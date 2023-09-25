# Concurrent Server example with TCP and UDP

TCP server echo backs the message and UDP server returns length of the string.  
TCP server can handle MAX clients (change the define value in the code) and UDP server is made iterative.

## Usage

```bash
$ gcc tcp-udp-select-server.c -o server
$ ./server
```

```bash
# In another terminal for TCP
$ telnet localhost 8888
```

For UDP, use netcat, make sure it is installed.

```bash
# In another terminal for UDP
$ nc localhost 8888 -u
```
