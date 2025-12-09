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
- Closes the unused **write end**.
- Continuously reads all incoming messages from the pipe.
- Stops when all children close their write ends and `read()` returns `0`.
- Waits for all children using `wait()` to prevent zombie processes.
  

Author

Dimitrios Dalaklidis is an aspiring backend developer with a strong academic foundation in Informatics and hands-on experience in systems programming, data structures, and software architecture. His work reflects a methodical approach to problem solving, supported by practical exposure to multi-language development environments and structured programming disciplines. He has completed a range of projects involving low-level system operations in C, object-oriented application design in Java, browser-based scripting, and networked communication models.

His technical interests center on backend system design, algorithmic efficiency, and the construction of reliable, maintainable software. He actively pursues opportunities to expand his expertise through academically driven projects and independent research, with an emphasis on building robust systems that adhere to professional development practices and modern software engineering principles.

For professional communication, he can be reached at: dalaklidesdemetres@gmail.com

