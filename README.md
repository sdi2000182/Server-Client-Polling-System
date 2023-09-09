# Multi-Threaded Network Server (Poller)

This project is a multi-threaded network server implemented in C/C++ named "poller." The server conducts a pre-election poll of political parties and stores user votes and poll statistics. It is designed to run in a Unix environment.

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
- [Implementation Details](#implementation-details)

  ## Overview

The "poller" server is divided into several components:

- **Master Thread**: Responsible for accepting client connections and placing them in a buffer.
- **Worker Threads**: Handle client requests, record votes, and communicate with clients.
- **Buffer**: A shared data structure used to synchronize communication between master and worker threads.
- **Data Storage**: Stores user votes and poll statistics in separate log files.

## server.c - Multi-Threaded Network Server

The `server.c` file contains the main implementation of the multi-threaded network server known as "poller." This server conducts a pre-election poll of political parties, stores user votes, and manages poll statistics. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Signal Handling**: The `handle_sigint` function is responsible for handling the SIGINT signal (Ctrl+C) to gracefully exit the server.

- **Thread Monitoring**: The `threadIsActive` function checks whether a given thread is active or not. It uses `pthread_kill` to determine the thread's status.

- **Initialization**: The `initialize` function initializes the pool used for buffering client connections. It allocates memory for the buffer and sets initial values for pool attributes.

- **Buffer Management**: The `place` function places a new socket descriptor into the buffer, while the `obtain` function retrieves a socket descriptor from the buffer.

- **Error Handling**: The `errors` function is used for reporting and handling errors. It closes sockets and performs shutdown operations when errors occur.

- **Main Function**: The `main` function is the entry point of the server. It initializes various components, including socket handling, buffer, worker threads, and signal handling.

## workers.c - Worker Threads

The `workers.c` file contains the implementation of worker threads used in the multi-threaded network server. These worker threads handle client connections, process user votes, and manage various states of the server. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Worker Threads**: The `worker` function represents the main routine for each worker thread. It handles client connections, processes user votes, and transitions between different states of the server.

- **Worker Identification**: Each worker thread is identified by a unique ID, which is used to track and manage worker-specific tasks and responsibilities.

- **Client Connection Handling**: Worker threads continuously monitor the buffer for incoming client connections. When a connection is available, a worker thread retrieves it, processes the client's requests, and handles voting procedures.

- **State Management**: Worker threads manage various states of the server, including the initial state, username acquisition, results retrieval, and quitting. They transition between these states based on client interactions.

- **Error Handling**: The `errors` function is used for reporting and handling errors during client interactions. It ensures that the server gracefully handles errors and cleans up resources.

## pollswayer.c - Client Application

The `pollswayer.c` file represents the client application used to connect to the multi-threaded network server and facilitate the voting process. It reads voter data from an input file, connects to the server, and sends votes for processing. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Client Threads**: The `client` function represents the main routine for each client thread. It connects to the server, sends user information, and casts votes based on the provided voter data.

- **Client Initialization**: The client application initializes socket connections to the server using the server name and port number provided as command-line arguments.

- **Voter Data Handling**: The application reads voter data from an input file, extracting voter names and party affiliations, ensuring that duplicate voters are not processed.

- **Voting Process**: Clients interact with the server, sending user information and casting votes. They respond to server prompts and display messages based on the server's responses.

## server_helper.c - Server Helper Functions

The `server_helper.c` file contains essential helper functions and logic for the multi-threaded network server in the "poller" project. These functions manage server states, handle client interactions, and process votes. Below, you'll find an overview of key functionalities and components in this file.

### Overview

- **Server State Initialization**: The `initialize_server_states` function initializes semaphores used for server state management, ensuring thread synchronization during critical sections.

- **Client Interaction**: Functions like `init_state`, `get_username`, `vote_selection`, and `results_server` handle client interactions. They manage user authentication, vote processing, and result retrieval.

- **Vote Handling**: The `vote_selection` function manages the voting process, validating votes, and recording them in the server's data structures.

- **Server State Management**: The server uses semaphores to control access to shared resources during client interactions and username processing.


