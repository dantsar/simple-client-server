# Simple Client-Server Project
This is a simple client-server program written in C that sends/receives a file between a client and a server. This project is meant to be a learning exercise so PLEASE don't take anything here as best-practices

---
# Usage
#### Client
```
./client [--hostname address:port] [--send filename] [--receive filename]
```
The client can send or receive a file, but not both, from a server given an address:port pair. The filename can either be relative or absolute, but an error will be thrown if the file does not exist  

#### Server
words words
