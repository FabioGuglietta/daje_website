# Connecting to remote machines with `ssh`
In scientific computing, you often need to work on a machine that is not physically in front of you.

For example, you may need to connect to:
- a university server;
- a computing cluster;
- a remote workstation;
- a supercomputer login node.

To do this, we commonly use the command `ssh`.

The name `ssh` means **secure shell**. 
It allows you to open an encrypted terminal session on a remote machine.

## Basic syntax
The basic syntax is:
```bash
> ssh username@hostname
```
Here:
* `username` is your user account on the remote machine;
* `hostname` is the name or address of the remote machine.

For example:
```bash
> ssh m.sega@login.cluster.university.edu
```
This means "connect to the machine `login.cluster.university.edu` using the username `m.sega`"

If the connection is successful, you will be asked for your password, unless you have already configured key-based authentication.

After login, the prompt changes. From that point on, the **commands you type are executed on the remote machine**, not on your local computer.

For example:
```bash
> ssh m.sega@login.cluster.university.edu
m.sega@login.cluster.university.edu's password:
[m.sega@login ~]$ pwd
/home/m.sega
```
The command:
```bash
pwd
```
now prints the current directory on the remote machine.

## First connection to a remote host

The first time you connect to a remote machine, `ssh` may show a message similar to:
```bash
The authenticity of host 'login.cluster.university.edu' can't be established.
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```
If you are sure that the address is correct, type:
```bash
yes
```
After that, the host is stored in a local file called `known_hosts`, and the warning should not appear again for the same machine.

## Running commands on the remote machine

Once connected, you can use the same Unix commands introduced earlier.

For example:
```bash
$ pwd
$ ls
$ cd Documents
$ mkdir test
$ touch file.txt
```
These commands operate on files and directories on the remote machine.

This distinction is important:
```
before ssh  → commands run on your local computer
after ssh   → commands run on the remote computer
```

## Leaving the remote machine

To close the remote session, use:
```bash
$ exit
```
or press:
```bash
Ctrl + D
```
You will return to the local terminal.

Example:
```bash
$ exit
logout
Connection to login.cluster.university.edu closed.
>
```
After this, commands are again executed on your local machine.

## Copying files to and from a remote machine: `scp`

The command `ssh` opens a remote shell.

To copy files between your local computer and a remote machine, one common command is `scp`.

The name `scp` means **secure copy**.

### Copying a file to a remote machine

The basic syntax is:
```bash
> scp local_file username@hostname:remote_directory
```
For example:
```bash
> scp input.dat m.sega@login.cluster.university.edu:/home/m.sega/simulations/
```
This copies the local file:
```
input.dat
```
to the remote directory:
```
/home/m.sega/simulations/
```
on the machine:
```
login.cluster.university.edu
```

### Copying a file from a remote machine

To copy a file from the remote machine to your local computer:
```bash
> scp username@hostname:remote_file local_directory
```
For example:
```bash
> scp m.sega@login.cluster.university.edu:/home/m.sega/simulations/output.dat .
```
The final `.` means "copy the file into the current local directory".

So this command copies:
```
/home/m.sega/simulations/output.dat
```
from the remote machine into the current directory on your local computer.

### Copying directories with `scp -r`

To copy a directory and all its contents, use the option `-r`, which means recursive.

For example:
```bash
> scp -r results m.sega@login.cluster.university.edu:/home/m.sega/simulations/
```
This copies the local directory results to the remote machine.

To copy a remote directory to the current local directory:
```bash
> scp -r m.sega@login.cluster.university.edu:/home/m.sega/simulations/results .
```
Remote paths and local paths

When using `scp`, it is important to distinguish local and remote paths.

A remote path has the form:
```
username@hostname:/path/to/file
```
For example:
```
m.sega@login.cluster.university.edu:/home/m.sega/output.dat
```
A local path does not have `username@hostname:` in front of it.

For example:
```
./output.dat
/home/m.sega/output.dat
```
In this command:
```bash
> scp output.dat m.sega@login.cluster.university.edu:/home/m.sega/
```
the source is local:
```
output.dat
```
and the destination is remote:
```
m.sega@login.cluster.university.edu:/home/m.sega/
```
In this command:
```bash
> scp m.sega@login.cluster.university.edu:/home/m.sega/output.dat .
```
the source is remote:
```
m.sega@login.cluster.university.edu:/home/m.sega/output.dat
```
and the destination is local:
```
.
```

## Summary
```bash
> ssh username@hostname                              # opens a terminal session on a remote machine.
$ exit                                               # closes the remote session.
> scp file.txt username@hostname:/remote/path/       # copies a local file to a remote machine.
> scp username@hostname:/remote/path/file.txt .      # copies a remote file to the current local directory.
> scp -r directory username@hostname:/remote/path/   # copies a local directory recursively to a remote machine.
> scp -r username@hostname:/remote/path/directory .  # copies a remote directory recursively to the current local directory.
```

## Exercise

Connect to a remote machine:
```bash
> ssh username@hostname
```
Check where you are:
```bash
$ pwd
```
List files:
```bash
$ ls
```
Create a test directory:
```bash
$ mkdir ssh_test
$ cd ssh_test
$ touch remote_file.txt
$ ls
```
Exit the remote machine:
```bash
$ exit
```
Back on your local machine, copy a file to the remote machine:
```bash
> scp local_file.txt username@hostname:/home/username/ssh_test/
```
Then connect again and check that the file is there:
```bash
> ssh username@hostname
$ ls /home/username/ssh_test/
```





## SSH keys, the `.ssh` directory, and the config file

When using `ssh`, authentication can be done with a password or with an **SSH key**.

SSH keys are usually more convenient and more secure than typing a password every time.

An SSH key pair contains two files:
- a **private key**, which stays on your computer;
- a **public key**, which can be copied to remote machines.
> [!WARNING]
> The **private** key must remain **private**. It should not be shared, emailed, or copied to other people.
> The **public** key can be **safely copied to remote machines**. It tells the remote machine which private key is allowed to connect.

### The `.ssh` directory
SSH-related files are usually stored in a hidden directory inside your home directory:
```text
~/.ssh
```
The symbol `~` means your home directory. Therefore, `~/.ssh` means:
```
/home/username/.ssh
```
on many Linux systems.

You can inspect it with:
```bash
> ls ~/.ssh
```
Common files inside `~/.ssh` include:
```
id_rsa              # private RSA key
id_rsa.pub          # public RSA key
id_ed25519          # private Ed25519 key
id_ed25519.pub      # public Ed25519 key
known_hosts         # list of remote hosts already contacted
authorized_keys     # public keys allowed to connect to this account
config              # user-specific SSH configuration file
```
Files such as `id_rsa` or `id_ed25519` are **private** keys!

Files ending in `.pub`, such as `id_rsa.pub` or `id_ed25519.pub`, are **public** keys.

### Creating an SSH key

A common modern choice is an **Ed25519 key**:
```bash
> ssh-keygen -t ed25519
```
This command creates a new key pair.

By default, the files are usually saved as:
```
~/.ssh/id_ed25519
~/.ssh/id_ed25519.pub
```
During key creation, you may be asked for a passphrase. A passphrase protects the private key in case someone obtains the file.

### Copying the public key to a remote machine

To connect using a key, the public key must be added to the remote machine.

A common command is:
```bash
> ssh-copy-id username@hostname
```
This copies your public key to the remote file:
```bash
~/.ssh/authorized_keys
```
After that, you should be able to connect with:
```bash
> ssh username@hostname
```
**without typing the account password every time**, depending on the remote system configuration.

> [!NOTE]
> If `ssh-copy-id` is not available, the public key can be copied manually.
> You can print your public key with:
> ```bash
> cat ~/.ssh/id_ed25519.pub
> ```
> The resulting line can be added to the file:
> ```bash
> ~/.ssh/authorized_keys
> ```
> on the remote machine.

### The `known_hosts` file

The file:
```
~/.ssh/known_hosts
```
stores the identity of remote machines you have contacted before.

The first time you connect to a host, you may see a message such as:
```
The authenticity of host 'login.cluster.university.edu' can't be established.
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```
If you answer `yes`, the host information is saved in `known_hosts`.

On future connections, SSH checks that the remote host has the same identity. If it changes, SSH shows a warning. This can happen if the remote machine was reinstalled, but it can also indicate a security problem.

### The SSH config file

The file:
```
~/.ssh/config
```
allows you to define shortcuts for SSH connections.

For example, instead of typing:
```bash
> ssh m.sega@login.cluster.university.edu
```
you can create or edit the file:
```bash
> vim ~/.ssh/config
```
and add:
```
Host cluster
    HostName login.cluster.university.edu
    User m.sega
```
Then you can connect simply with:
```bash
> ssh cluster
```
The name cluster is an alias chosen by the user.

A more complete example is:
```
Host cluster
    HostName login.cluster.university.edu
    User m.sega
    IdentityFile ~/.ssh/id_ed25519
```
Here:
* `Host cluster` defines the shortcut name;
* `HostName` is the real remote machine address;
* `User` is the username on the remote machine;
* `IdentityFile` specifies which private key to use.

This is useful when you connect often to the same machine or when different machines require different usernames or keys.

### File permissions

SSH is strict about permissions. Private keys should not be readable by other users.

A typical permission setup is:
```bash
> chmod 700 ~/.ssh
> chmod 600 ~/.ssh/id_ed25519
> chmod 644 ~/.ssh/id_ed25519.pub
> chmod 600 ~/.ssh/config
```
If permissions are too open, SSH may refuse to use the key.

Summary
```bash
> ssh-keygen -t ed25519
```
creates a new SSH key pair.
```bash
> ssh-copy-id username@hostname
```
copies the public key to the remote machine.
```bash
> cat ~/.ssh/id_ed25519.pub
```
prints the public key.
```bash
> ssh username@hostname
```
connects to a remote machine.
```bash
> ssh cluster
```
connects using an alias defined in ~/.ssh/config.

Important files:
```
~/.ssh/id_ed25519        # private key
~/.ssh/id_ed25519.pub    # public key
~/.ssh/authorized_keys   # public keys allowed to connect
~/.ssh/known_hosts       # known remote hosts
~/.ssh/config            # SSH shortcuts and options
```
