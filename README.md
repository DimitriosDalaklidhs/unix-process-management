# Multi-Process Pipe Communication in C

This project demonstrates **inter-process communication (IPC)** using a single Unix **pipe** shared among multiple child processes.  
Each child sends a series of formatted messages to the parent process, which reads and prints them to standard output.

##  Features

 Creates a pipe for **unidirectional communication**.
 Spawns multiple child processes using `fork()`.
 **Each child process:**
   Sends several formatted messages through the pipe.
   Includes its PID, message index, and a static email identifier.
 **The parent process:**
   Reads all incoming messages from the pipe.
   Prints them in the order they arrive (interleaved due to concurrency).
   Waits for all children to finish, avoiding zombie processes.
##  How It Works

### 1️ Pipe Creation

The parent creates a Unix pipe using `pipe()`:

 `pipe_fd[0]` → **read end**  
 `pipe_fd[1]` → **write end**



### 2️ Process Creation

The parent forks **5 child processes**.  
Each child inherits the pipe file descriptors.



### 3️ Child Behavior

Each child:

- Closes the unused **read end** of the pipe.
- Sends **5 messages**, one per second.
- Each message contains:
  - Parent PID  
  - Message number  
  - Child PID  
  - A static email string  
- Closes the write end and exits.



### 4️ Parent Behavior

The parent:

- Closes the unused **write end**.
- Continuously reads all incoming messages from the pipe.
- Stops when all children close their write ends and `read()` returns `0`.
- Waits for all children using `wait()` to prevent zombie processes.

