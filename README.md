Design and Implement a simple network file transfer protocol that communicate between server and client. 
The protocol is the solo reference to which the client and server can seperately implemented by using TCP as a transport layer protocol to delivery the data & the server port number.
Myftp is the standard mechanism provided by TCP/IP for copying a file from one host to another.
Myftp has two connections: Control connection and Data connection.
Control Connection: Remains connected through entire Myftp session
Data Connection: It opens and closes for each file transfer.
User interacts through user interface
Server protocol and User protocol  are connected to send control information like User Identification,  Password, Commands to change the remote directory, Commands to retrieve and store files.
Data Transfer of client and Data Transfer of server are connected to send the actual file. A data connection is initiated on port number 20.

When  Myftp session is started between a client and a server, the client Initiates a control TCP connection with the server side.
The client sends control information over this. When the server receives this, it initiates a data connection to the client side. 
Only one file can be sent over one data connection. 
But the control connection remains active throughout the user session.

Myftp Command:

Client will sent a command as string along with file name. So the server will check for a filename, If it is present then it go with the control commands 
```
    	
      a) GET  :  Get a file from server and save it in client.
      b) PUT  : Upload a file from client to server.
      c) CD   : Change the current working directory of server to directory specified in path of client
      d) PWD  : Displays all the path of directories or files present in current directory of server. 
      e) DIR  : Displays all the list of directories or files present in current directory of server.
      f) LPWD : Displays all the path of directories or files present in current directory of client.
      g) LCD  : Change the current working directory of client to directory specified in path.
      h) LDIR : Displays all the list of directories or files present in current directory of client. 
      i) QUIT : Terminate the process
```
Building:

```
./ser (in one terminal)
./cli (in other terminal)

```
give the commands in client terminal
