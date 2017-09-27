# RPC
Simple Remote Procedure Calls

## Background

The program is to create a simple hand-rolled RPC service that takes 2-dimensional points (pairs of double-precision floating point numbers) and perform useful functions.

The functions return 0 on success and -1 on failure, so the result value to be returned must be passed in as a pointer arguement. The caller must allocate space for the return result.

RPC is a C++ class (which you will define) containing the parameters for making a UDP call to the RPC service, including host, port, timeout, and max_retries.

The client-side stubs will marshal and unmarshal the arguments, but use a method of the RPC class to perform the network communication.

Use the setsockopt() parameter SO_RCVTIMEO to control the timeout waiting for a response from the server.

To keep things simple, we set timeout to 5 seconds and max_retries to 3.


## Server Program

The server program takes the port number as a command-line argument.

The server will receve UDP packets on the specified port and sends UDP replies to the sender.

To simulate failures, if either point is in the left half-plane (negative x), the server will ignore the request (so the client will timeout). If the point is in the lower right quardrant (positive x, negative y), the server should return -1 to indicate error.

I use cerr to print log and trace messages.


## Client Program

The client program will take server host and port as command-line arguments and will read points from standard input (each input line should contain 4 floating-point numbers in text format).


## Wire Format

Each interaction between client and server will be done through a pair of UDP datagrams.

#### The client sends the following (binary data):

transaction id (4 bytes)

operation code (4 bytes): enum value

parameters (each floating-point number is 8 bytes)



#### The server replies with

transaction id (4 bytes): same value sent by client

result code (4 bytes): 0 for success, -1 for failure

result value
