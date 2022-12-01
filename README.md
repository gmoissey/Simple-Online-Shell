# Online Shell

This is online shell program written in `c`, using socket and multi process programming. 
To run this program you will need to run these commands:

-   `make` - this will create two binaries: one for client and one for server
-   `./socket_server` - first you need to spin up server. If you get `port already in use` error, then change port number defined in client and server code.
-   `./socket_client` - run this in a different shell. **Server supports multipile clients** at a time so you can spin up other client instaces in other shells.

**important:** due to limited functionality of this shell, please **run commands that will produce standard output.** Otherwise, online shell client will hang and you will have to restart.

To communicate with the server you just need to input unix command and the produced output on server will be sent and printed on client side.
You can also view debug output in server shell.


## Examples

Example client side output:
```
[crappyShell]:▷ls -l
total 52
-rw-r--r-- 1 gmoissey ugrad   127 Nov 30 18:07 Makefile
-rw-r--r-- 1 gmoissey ugrad   940 Nov 30 19:09 README.md
-rwxr-xr-x 1 gmoissey ugrad 15384 Nov 30 19:09 socket_client
-rw-r--r-- 1 gmoissey ugrad  1130 Nov 30 18:00 socket_client.c
-rwxr-xr-x 1 gmoissey ugrad 20064 Nov 30 19:09 socket_server
-rw-r--r-- 1 gmoissey ugrad  2377 Nov 30 18:46 socket_server.c

[crappyShell]:▷echo helloWorld
helloWorld

[crappyShell]:▷
```

Example server side output:
```
recieved command: ls -l

recieved command: echo helloWorld
```