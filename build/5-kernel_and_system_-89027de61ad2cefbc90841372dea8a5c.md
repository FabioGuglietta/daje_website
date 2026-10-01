# The Kernel


In any modern operating system, the kernel is the fundamental piece of software that acts as the bridge between the hardware 
and the user-space applications running on it. It’s responsible for critical functions like managing hardware resources (CPU, memory, storage devices), 
handling system security, and ensuring efficient communication between the hardware and software layers.

Examples of widely used kernels include

* Unix: The original kernel, developed in the 1970s, forms the foundation of many modern systems.
* BSD Kernels: The family of kernels originating from the Berkley Software Distribution (a variant of UNIX) used in operating systems like FreeBSD, OpenBSD, and NetBSD.
* Linux: The kernel used in many operating systems, including GNU/Linux distributions like Ubuntu, Fedora, and Debian.
* Windows NT: The kernel used in modern versions of Microsoft Windows, including Windows 10 and Windows Server.
* Darwin: The kernel used in some of Apple's macOS systems. 

Some early computer systems operated without what we would consider a modern kernel, relying on simpler architectures for hardware and software management. 
For example, MS-DOS lacked a true kernel, operating in a single-tasking environment where programs had direct hardware access. 
IBM’s OS/360 featured a more advanced system with a Supervisor that managed tasks and memory, but it was part of a larger monolithic structure rather than a distinct, modular kernel. 
The AS/400 system (now known as IBM i) used the so-called Licensed Internal Code (LIC) to manage hardware, though its architecture was highly integrated, abstracting much of 
the kernel-like functionality into tightly coupled layers like TIMI (Technology Independent Machine Interface), including also an integrated relational database as part of the operating system
itself. 


To understand the principles behind how a modern kernel like Linux works, it's helpful to break it down into its main components:

## Process Management
The kernel is responsible for managing processes—the individual programs that run on your computer. 
It ensures that each process gets a fair share of CPU time, coordinates multitasking, and handles communication between processes and providing, for example:
1. Process Scheduling: to decide which processes get CPU time and in what order. This ensures that system resources are used efficiently, allowing multiple processes to run concurrently (multitasking).
1. Process Isolation: so that one program cannot access or interfere with another’s memory space or data.

Example: When you have multiple programs running (e.g., a browser, music player, and text editor), the kernel manages these processes so they don't conflict and ensures each gets CPU time 
through a mechanism called time-sharing.

## Memory Management

Memory serves as the workspace for programs and data.  Every program you run must be loaded into memory before it can be executed by the CPU.  The performance of computer code is often limited by the speed of transfering data between different types of memory, but reaching the physical limit and thus optimizing your code requires to carefully structuring your data and how these are accessed. 

The kernel manages each process's virtual address space, maps it to physical
memory, and enforces access restrictions. When swap space is configured, some
memory contents can be moved to storage and retrieved later. Swap is only one
part of memory management; virtual memory is not simply extra RAM on disk.

Applications still need to manage their allocations correctly: the kernel
does not automatically prevent memory leaks in a running program. Under severe
memory pressure, allocations can fail or processes may be terminated.

The next lesson, [Memory: RAM, Registers, Cache, and Data Layout](5a-memory_and_data_layout.md),
explains the hardware hierarchy, addresses, and why array layout matters for
numerical calculations.

## Device Drivers
These are specialized pieces of code that allow the operating system to communicate with hardware devices like printer, network card, or hard drive. 
The kernel includes a wide range of device drivers, providing also hardware abstraction, so that user programs don't need to know the details of how 
the hardware works. 

*Example: when you write to a file on a hard drive, the kernel takes care of communicating with the drive’s hardware through a device driver.*

*Example: when you type on your keyboard, the kernel uses a keyboard driver to receive the input and pass it along to the appropriate application, like your text editor.*

## File System Management
The Linux kernel provides a unified file system interface that allows you to store and retrieve files from various storage devices, whether they're local hard drives, SSDs, or 
remote file systems accessed over a network. The kernel supports many types of file systems (like ext4, FAT32, NTFS), which dictate how data is stored and retrieved on disk. 
It also handles the organization of files into directories and maintains metadata like file permissions, timestamps, and ownership.
      
*Example: When you run the ls command to list files in a directory, the kernel interacts with the file system to retrieve and display this information.*

## Networking
The kernel provides implementations of networking protocols (e.g., TCP/IP) that allow computers to communicate over networks, such as the internet. 
This includes also handling routing of communications, packet filtering (the so-called "firewall").
In particular, the linux kernel provides also a socket interface to user applications, allowing programs to send and receive data over the network as though they were reading from or writing to a file.

*Example: When you load a webpage in your browser, the kernel handles sending requests to the web server and receiving the response over the network.*

## Inter-Process Communication (IPC)
The kernel provides mechanisms for processes to communicate and share data with each other. These mechanisms include pipes, message queues, shared memory, and signals. In particular:
1. Pipes and Message Queues: allow one process to send data to another and let processes exchange structured messages.
1. Signals: are used to notify processes of asynchronous events, e.g., when a user presses `Ctrl+C` to terminate a program.

*Example: When a program like a web server forks a child process to handle a new request, it can use pipes to send data back and forth between the parent and child processes.*

## Security and Access Control
The Linux kernel is responsible for ensuring that users and processes can only access the resources they're allowed to. This includes managing file permissions, user authentication, and process isolation.
The kernel enforces user and group-based permissions, ensuring that users can only access the files and devices they're authorized to.
        
*Example: If you try to delete a system file without the proper permissions, the kernel will prevent you from doing so, protecting the integrity of the system.*

