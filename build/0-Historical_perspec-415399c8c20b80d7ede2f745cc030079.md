# From Hardware to Operating Systems

## A Very Brief History of Computing

> [!NOTE]
> This historical overview is not essential for learning how to program on a modern UNIX-like system. It can be skipped on a first reading. Its purpose is simply to explain where some of the abstractions we use today — processes, files, virtual memory, signals, networking — actually come from.

Modern operating systems can easily look like collections of arbitrary conventions. Why do we create processes in the peculiar UNIX way? Why is C still so closely connected to system programming? Why are files and network connections accessed through similar interfaces? Why did the transition to 64-bit architectures matter so much for scientific computing?

These choices make more sense when viewed historically. Most of them were not designed all at once. They emerged gradually as computers changed, became faster, acquired more memory, started serving multiple users, and eventually became connected through networks.

What follows is therefore not intended as a complete history of computing. It is a short path through some of the developments that are particularly useful for understanding the systems we use today.

---

## Before Digital Computers

Computing did not begin with digital machines.

Before electronic digital computers became practical, many scientific and engineering problems were attacked using **analog machines**. These machines represented physical quantities directly through other physical quantities: a voltage could represent a temperature, for example, or the position of a mechanical component could represent the value of a mathematical variable.

One of the best-known examples is the differential analyzer developed by Vannevar Bush and his collaborators at MIT during the 1930s. Machines of this type could solve differential equations by mechanically performing mathematical operations such as integration.

Analog computers had an important advantage: they could represent the evolution of a system continuously and often in real time. But they also had serious limitations. Precision depended on the physical components, machines had to be manually reconfigured for different problems, and results depended on calibration.

The transition to digital computing changed the nature of computation.

---

## The Digital Turn

A digital computer does not represent a variable by continuously varying some physical quantity. Information is instead encoded using discrete states — ultimately, **bits**.

This apparently simple change has profound consequences. Once information is represented digitally, the same sequence of operations can be reproduced exactly. Programs can contain conditional decisions and loops. More importantly, the description of an algorithm becomes progressively separated from the particular physical mechanism used to execute it.

Some of the first large electronic digital machines appeared during the 1940s. ENIAC became operational in 1945. Machines developed soon afterwards explored the idea of storing program instructions in memory together with data, an idea that became central to modern computer architecture.

By the 1950s, computers such as the IBM 701 and later the IBM 7090 were being used in scientific and technical institutions.

These were enormously expensive machines. The problem was therefore not only how to compute something, but how to avoid wasting the machine's time.

---

## Batch Processing

Early users did not normally sit in front of a computer and interact with it.

A program might be prepared using punched cards, handed to an operator, placed in a queue, and executed hours later. The output would then be returned to the user.

This way of working became known as **batch processing**.

From our present perspective it sounds extraordinarily inconvenient, but it reflected the economics of computing at the time. Computer time was much more valuable than user time. Keeping the processor busy was therefore the priority.

Early operating systems — often little more than supervisory programs — automated the transition between jobs. One program finished, the next one was loaded, and the machine continued working with as little idle time as possible.

The computer was being managed as a shared resource, but interaction was still minimal.

That changed in the 1960s.

---

## Time Sharing

Suppose instead that many users are connected to one computer through terminals.

No individual user needs the processor continuously. A person types a command, pauses, reads the result, thinks, and types another command. During those pauses the processor can execute somebody else's program.

This observation led to **time sharing**.

At MIT, the Compatible Time-Sharing System (**CTSS**) demonstrated in the early 1960s that a single computer could support multiple interactive users. The operating system rapidly switched the processor between them. Each user experienced something resembling a personal machine, even though the hardware was being shared.

This introduced a much richer problem for the operating system. It now had to manage several users, several programs, memory, files, permissions, and interactions between running computations.

One of the most ambitious attempts to solve these problems was **Multics**, developed beginning in the 1960s by MIT, Bell Labs, and General Electric.

Multics introduced or developed many ideas that are now familiar: hierarchical file systems, dynamic linking, segmented memory, sophisticated access control, and a system designed from the beginning for multiple simultaneous users.

Multics was technically ambitious and complicated. Its influence, however, extended far beyond the machines on which it actually ran.

One of the systems that emerged from that environment was UNIX.

---

## UNIX

Bell Labs had participated in the Multics project but eventually withdrew.

In 1969, Ken Thompson began developing a much smaller operating system on a DEC PDP-7. Dennis Ritchie soon joined the effort. Rather than reproducing the complexity of Multics, the new system emphasized a relatively small collection of simple mechanisms that could be combined.

That system became **UNIX**.

The first versions were written largely in assembly language. This was normal: operating systems were closely tied to the hardware on which they ran.

But this creates an obvious problem. An operating system written entirely in assembly language for one processor cannot easily be moved to another processor.

The development of C changed this situation.

---

## C and UNIX

The histories of C and UNIX are difficult to separate.

Dennis Ritchie developed C at Bell Labs in the early 1970s, partly in the context of UNIX development. C provided something unusual: it was a high-level language, but one that remained close enough to the machine to implement an operating system efficiently.

A C program can manipulate memory addresses through pointers, represent data in a way that maps naturally onto machine memory, and execute with very little runtime machinery between the program and the hardware.

At the same time, C is not tied to the instruction set of one particular processor.

This distinction became crucial.

During 1972–1973, UNIX was largely rewritten in C. Moving the operating system to a new architecture no longer required rewriting the entire system in a new assembly language. A large part of the code could remain unchanged; only the architecture-dependent portions had to be adapted.

Portability became one of UNIX's major advantages.

C and UNIX consequently evolved together. UNIX provided a demanding real system in which C could develop, while C made UNIX easier to port and distribute.

The publication of Kernighan and Ritchie's *The C Programming Language* in 1978 helped spread the language far beyond Bell Labs.

This historical connection also explains something that is still visible today: the native programming interface of UNIX-like operating systems feels fundamentally like a C interface.

---

## The UNIX Programming Model

UNIX is not built around a very large collection of highly specialized mechanisms. Instead, much of the system can be understood from a relatively small set of ideas.

A running program is a **process**.

A process can create another process using `fork()`.

It can replace the program it is executing using `exec()`.

Files and many other resources are accessed through **file descriptors** and manipulated as streams of bytes.

The output of one program can be connected to the input of another through a **pipe**.

These mechanisms are individually simple. Their strength comes from composition.

For example, the shell does not need special knowledge of every pair of programs that might communicate. It can simply connect the output file descriptor of one process to the input file descriptor of another.

This philosophy became one of the defining characteristics of UNIX.

---

## UNIX Leaves Bell Labs

During the 1970s, UNIX began spreading into universities.

Because AT&T was operating under restrictions that limited its activities in the computer business, UNIX source code could be licensed to academic institutions under conditions that were unusual for commercial software.

One particularly important centre of UNIX development emerged at the University of California, Berkeley.

The Berkeley group produced what became known as the **Berkeley Software Distribution**, or **BSD**.

BSD was not merely a repackaging of UNIX. It became an important branch of UNIX development in its own right. Among other contributions, Berkeley developers worked on virtual memory and produced a highly influential implementation of TCP/IP networking.

The relationship between UNIX and networking would become extremely important.

---

## Networks Become Part of the System

Today it is natural to assume that computers communicate with other computers. That assumption was much less obvious during the early history of computing.

UNIX developed in an environment in which networking increasingly became part of normal computer use.

BSD's TCP/IP implementation in the early 1980s played an important role in this transition and became closely associated with the growth of the Internet.

This also reinforced an important feature of UNIX design: local and remote resources could often be manipulated through relatively small and consistent programming interfaces.

The computer was no longer simply a machine executing programs in isolation. It was becoming one node in a network of machines.

---

## The UNIX Workstation

By the 1980s, UNIX had moved well beyond its original Bell Labs environment.

Several commercial UNIX families appeared. AT&T developed System V, while BSD continued to evolve. Companies such as Sun Microsystems, Silicon Graphics, Hewlett-Packard, IBM, and Digital Equipment Corporation sold UNIX workstations aimed particularly at engineering, scientific computing, graphics, and technical applications.

Systems such as SunOS and Solaris, IRIX, HP-UX, AIX, and Ultrix became common in research laboratories and universities.

For scientists, these machines offered an environment that already looks surprisingly familiar: C and Fortran compilers, numerical libraries, network access, terminals, graphical applications, and multiple users sharing the same operating system.

The modern scientific workstation was taking shape.

---

## The X Window System

Graphical interfaces on UNIX developed differently from those on many personal computers.

The **X Window System**, first released at MIT in 1984, was designed around a networked client-server architecture.

The terminology can initially appear reversed.

The machine controlling the screen, keyboard, and mouse runs the **X server**. A graphical application is an **X client**.

This means that the application does not necessarily have to run on the same machine as the display.

A program might execute on a powerful workstation or remote server while displaying its graphical interface on a terminal elsewhere on the network.

For scientific computing, this was a natural model. Computation and visualization did not need to happen on the same machine.

If you have ever logged into a remote UNIX machine and displayed a graphical program locally, you are using an idea that goes back to this period.

---

## The Move to 64 Bits

Another important transition occurred as processors moved from 32-bit to 64-bit architectures.

It is easy to describe this simply as “using larger numbers”, but for scientific computing the more important consequence was often the size of the **address space**.

A 32-bit address can distinguish roughly \(2^{32}\) memory locations. In a conventional byte-addressed system, that corresponds to about 4 GB of addressable memory.

For increasingly large simulations and datasets, this became a serious limitation.

64-bit architectures made vastly larger address spaces possible.

Digital Equipment Corporation's **Alpha** architecture, introduced in 1992, was an important early commercial 64-bit architecture. Other architectures, including MIPS, SPARC, and IBM POWER, also moved into the 64-bit world.

Eventually the x86 architecture acquired its own 64-bit extensions, usually referred to as **x86-64** or AMD64.

What had once been a feature of expensive scientific and engineering machines became standard in ordinary computers.

---

## Meanwhile: The Personal Computer

A different computing ecosystem had been developing in parallel.

The IBM PC, introduced in 1981, and the Intel processors used by it helped establish the architecture that eventually became dominant in personal computing.

Early PC operating systems such as MS-DOS were far simpler than contemporary UNIX systems. They lacked features such as strong memory protection and preemptive multitasking that UNIX users already regarded as normal.

Personal computers, however, became extraordinarily successful because they were inexpensive and increasingly powerful.

Microsoft Windows gradually acquired the more sophisticated mechanisms expected from a modern operating system, although its internal architecture and programming interfaces developed along a different historical path from UNIX.

For some time, therefore, the computing world contained two quite different traditions: powerful multi-user UNIX workstations and comparatively inexpensive personal computers.

The distinction would eventually become much less clear.

---

## Linux

In 1991, Linus Torvalds began developing a new kernel for Intel 80386 processors.

The project became **Linux**.

Linux implemented a UNIX-like operating-system model on inexpensive PC hardware. Combined with tools developed by the GNU project and other free-software projects, it provided a complete environment containing compilers, shells, libraries, utilities, and eventually graphical desktops.

This combination was particularly attractive to universities and research laboratories.

Instead of purchasing expensive proprietary UNIX workstations, it became possible to run a UNIX-like environment on commodity hardware while retaining access to the source code.

Linux spread rapidly.

This development would have major consequences for scientific computing.

---

## macOS and the UNIX Lineage

Apple's modern operating system also has a UNIX history.

After leaving Apple, Steve Jobs founded NeXT in 1985. NeXT developed an operating system called **NeXTSTEP**, built using the Mach kernel developed at Carnegie Mellon University together with substantial BSD technology.

Apple acquired NeXT in 1997.

NeXT's software architecture then became the foundation of Mac OS X, released in 2001 and later renamed macOS.

As a result, a modern Mac combines Apple's graphical environment and frameworks with an operating-system foundation that belongs to the broader UNIX tradition.

This is why opening a terminal on macOS reveals an environment containing familiar UNIX concepts: processes, file descriptors, permissions, signals, sockets, shells, and a POSIX-oriented programming interface.

---

## UNIX-like Systems and Scientific Computing

By the late 1990s and early 2000s, scientific computing was undergoing another transition.

Instead of relying exclusively on large proprietary machines, researchers increasingly constructed clusters from relatively inexpensive commodity computers.

Linux was particularly well suited to this model.

At the same time, **MPI — the Message Passing Interface —** provided a standard programming model for processes communicating across distributed-memory systems.

A large parallel simulation could therefore be decomposed into many UNIX processes running on different machines and exchanging messages through a network.

The underlying ideas were not entirely new. Processes and networking had been part of UNIX for decades. What changed was the scale.

The same abstractions that worked on university workstations could now be used across thousands of processors.

Linux eventually became the dominant operating system in high-performance computing and remains so today.

---

## What Survived?

The machines changed almost beyond recognition.

Mechanical integrators gave way to vacuum tubes, transistors, integrated circuits, workstations, personal computers, clusters, GPUs, and supercomputers.

Memory increased by enormous factors. Processors became dramatically faster. Networks moved from experimental infrastructure to a fundamental component of computing.

Yet several operating-system concepts survived this entire evolution.

We still have:

- processes;
- virtual memory;
- files and file descriptors;
- signals;
- sockets;
- system calls.

This persistence is not accidental.

These concepts do not describe one particular processor. They provide abstractions for problems that every general-purpose computer must solve: how to execute programs, isolate them, give them access to memory, store information, communicate with devices, and exchange data with other programs or machines.

The hardware underneath those abstractions has changed repeatedly.

The abstractions themselves have proved remarkably durable.

This is the reason we will spend time studying them. The objective is not to learn the historical details of UNIX for their own sake. It is to understand the mechanisms that still sit between our programs and the machine.

And because C developed together with UNIX, it gives us an unusually direct way to examine that boundary.
