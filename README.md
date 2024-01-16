
# Socket Talk - File Server and Client

Partner 1:
* must create a server.c file that accepts a port number as an argument
* must push to GitHub the following files: server.c, sycalls.h, syscalls.c, serverFuncs.h, serverFuncs.c, and globalDefs.h

Partner 2:
* must create a client.c file that accepts a server name and a port number as arguments
* must push to GitHub the following files: client.c, sycalls.h, syscalls.c, clientFuncs.h, clientFuncs.c, and globalDefs.h

The main function must be minimal (along the lines of fileManip.c), and all function calls in main must be to functions in other files. GlobalDefs.h should hold any user-created data types (i.e., unions or structs) and an ITEMS_TO_COPY variable set to 512 (similar to P3's NUM_BYTES_TO_COPY_AT_A_TIME).

The server is to accept a connection request from the client and prompt the client to select a file for transfer. The developer of the server may choose how they want the client to make the request (e.g., send a number or enter a string). But the server needs to check the request for validity and re-prompt when an invalid response is received.

The server, after receiving a valid request, sends the following information in JSON format. After the client reads this information (which will be less than 512 bytes), it sends a "ready" signal. The server then sends the file. After the client receives the file (and verifies the data was not corrupted in any way (i.e., the signatures match), it sends a "ready" signal. The server then asks the client if it wants another file, and the process repeats. Thus, the server is to continue prompting for a file to be transferred until the connection is closed.

{
  "file_name": "someString",
  "file_size": someSize_tValue,
  "file_type": text,
  "file_signature": someUnsignedLongValue
}

The client is to request a connection to the server. When prompted to request a file, the client responds in accordance with the request. See above for the remaining details.

The JSON object should be encrypted, but you do not need to send the size of the transmission.