# Local and Remote Machines: The Client–Server Model

Scientific work often involves more than one computer. You might write a
program on a laptop, run it on a university server, and store results on a
shared filesystem. Before learning remote-access commands, it helps to
identify which machine performs each operation.

## Local and remote are relative terms

The **local machine** is the computer from whose perspective you are working.
A **remote machine** is another computer reached over a network.

For a student sitting at a laptop, the laptop is local and a university server
is remote. From a program running on the server, that server's own resources
are local. These terms describe a relationship rather than a type of hardware.

Each machine has its own operating system, running processes, and resources.
Connecting to another machine does not merge the two computers.

## Clients request; servers provide

A **client** requests a service. A **server** listens for requests and provides
that service. These terms can refer to software roles, not just physical boxes.

```text
local machine                         remote machine
client program  ─── network ────────>  server program
                <── responses ──────
```

Examples include a browser requesting pages from a web server and a remote-login
client requesting a session from a login service. One computer can run both
clients and servers, and one server can serve many clients.

## Finding the service

A **hostname** is a name used to identify a machine, such as
`compute.example.org`. Name resolution commonly translates that name into
an IP address used for network communication.

A network **port** identifies an endpoint for a service on that machine.
A correct hostname alone does not guarantee a successful connection: the
service must be running and reachable through the available network.

Account names identify users; hostnames identify machines. Having the same
username on two computers does not by itself make the accounts or their files
the same. Institutions may arrange shared account management, but that is an
additional service.

## Where does a remote session run?

In a remote terminal session, the local application carries keyboard input
and displays output. The remote shell and the programs it launches execute
on the remote machine.

For example, when a simulation is started on a server:

- its CPU and memory use occur on that server;
- its file paths are interpreted in the server's filesystem;
- the displayed output travels back over the network.

Seeing output on a laptop does not imply that the calculation runs on the laptop.
Similarly, `/home/alice/data` on one computer need not contain the same files
as the identical path on another computer.

## Accessing a machine is not copying files

Remote login, file transfer, and shared storage are different operations.

| Operation | Result |
| --- | --- |
| Remote login | A session in which programs run on another machine |
| File transfer | A copy of data is created at a destination |
| Shared storage | Multiple machines access storage made available to them |

If you copy a file and then edit the original, the copy does not automatically
change. Shared storage may expose the same underlying data on several machines,
but this must be configured; remote login alone does not provide it.

## Trust and access

Remote access involves checking both the destination and the user. The client
needs to establish that it is communicating with the intended service, and
the service needs to authenticate the account requesting access.

SSH provides encrypted remote access and mechanisms for host verification and
user authentication. Authentication does not remove the server's permission
rules: a remote session still operates with the account's allowed access.

## A cluster is more than one server

A computing cluster often separates **login nodes**, used for access and
preparation, from **compute nodes**, where scheduled calculations run.
An institution's usage rules determine which work belongs on each node.

Logging into a cluster does not necessarily mean that you are already on a
compute node or that resources have been allocated to your calculation.
The details of job submission will be introduced with HPC workflows.

## Summary

Clients request services from servers over a network. A remote session runs
programs on the remote machine; its accounts, files, and resources must be
understood in that context. Login, data transfer, and shared storage serve
different purposes.

After learning the shell basics, continue with
[Connecting to Remote Machines with SSH](../3-Working_with_the_Unix_Shell_Advanced/100-Connecting_to_remote_machines_with_ssh.md).
