Unix-like systems support **multitasking** by letting many processes make progress over time. 
On a **single CPU core**, only one process executes instructions at any instant, so the kernel uses **time-sharing**: it runs a process for a short time slice, p
reempts it (typically via a timer interrupt), then schedules another. On **multiple cores**, different processes (or threads) can run truly in parallel, 
but the programming model is the same: you write code assuming multiple independent execution contexts, and the kernel decides when and where they run. 
In Unix, `fork()` is the classic system call to create a new **process**: it produces a child process that begins as a near-copy of the parent 
(same program image and inherited open file descriptors) and returns twice—0 in the child, and the child’s PID in the parent—after which the two processes 
continue independently. From that point, the parent can keep doing useful work while the child performs a long task, and the two can coordinate using 
primitives such as `waitpid()` (to reap the child and obtain its exit status) and IPC mechanisms like `pipe()` to exchange data.

This is how one can use the `fork()` syscall to start a child process:

```C
// fork_wait_min.c — minimal fork()/waitpid() skeleton
// Build:  cc -O2 -Wall -Wextra -pedantic fork_wait_min.c -o fork_wait_min
// Run:    ./fork_wait_min

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0)  { perror("fork"); exit(1); }

    if (pid == 0) {
        // Child process
        printf("child:  pid=%ld, ppid=%ld\n", (long)getpid(), (long)getppid());
        exit(42);                  // choose an exit code to demonstrate status handling
    }

    // Parent process
    printf("parent: pid=%ld, spawned child pid=%ld\n", (long)getpid(), (long)pid);

    int status = 0;

    if (waitpid(pid, &status, 0) < 0) { perror("waitpid"); exit(1); }

    if (WIFEXITED(status)) {
        printf("parent: child exited normally, code=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("parent: child killed by signal %d\n", WTERMSIG(status));
    } else {
        printf("parent: child ended in an unusual way (status=0x%x)\n", status);
    }

    return 0;
}
```

The code first spawns a new process with `fork()`:

```C
    pid_t pid = fork();
```

If something goes wrong (e.g., exceeding the number of allowed processes or memory limits) the kernel sets the value of the variable `errrno`
to that of the specific error, and -1 is stored in `pid`. In this case we can use `perror()` to print the associated message, e.g.,

```bash
fork: Resource temporarily unavailable
```

If everything went well, there will be two processes running (either alternating in time sharing or in parallel on multiple cores/CPUs) from the moment  `fork()` was called:

1. the old, parent process, with information about the process ID of the child stored in `pid`
2. the new, child process, with zero stored in `pid`.

The child process can access its own pid by calling `getpid()`, and the pid of its parent by calling `getppid()`
```C
    if (pid == 0) {
        // Child process
        printf("child:  pid=%ld, ppid=%ld\n", (long)getpid(), (long)getppid());
        exit(42);                  // choose an exit code to demonstrate status handling
    }
```

> [! TODO]
> discuss zombie processes

After compiling it with
```bash
gcc fork_example.c -o fork_example
```
The output is

```bash
parent: pid=35272, spawned child pid=35276
child:  pid=35276, ppid=35272
parent: child exited normally, code=42
```

