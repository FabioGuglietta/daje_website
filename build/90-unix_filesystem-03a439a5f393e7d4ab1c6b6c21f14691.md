# The Unix filesystem: structure

Unix systems organize files in a single tree-like structure called the **filesystem**.

The top of the tree is the **root directory**:

```text
/
```

Every file and directory is located somewhere below `/`.

Example:

```text
/
├── home/
│   └── fabio/
│       ├── notes.txt
│       └── code/
├── bin/
├── etc/
├── dev/
└── tmp/
```

The path:

```text
/home/fabio/notes.txt
```

means:

```text
start from /
enter home
enter fabio
find notes.txt
```

In Unix, many things are represented as files: normal files, directories, terminals, disks, and devices.

---

# Files and directories

A **file** contains data.

A **directory** contains names of other files and directories.

Common directories are:

```text
/home     user directories
/etc      system configuration files
/bin      essential commands
/usr      installed programs and libraries
/tmp      temporary files
/dev      device files
```

The command:

```bash
ls
```

lists the contents of the current directory.

The command:

```bash
ls -l
```

prints a long listing:

```bash
-rw-r--r-- 1 fabio users 1200 Jun 17 10:20 notes.txt
drwxr-xr-x 2 fabio users 4096 Jun 17 10:21 code
```

The first character tells the type of filesystem entry:

```text
-    regular file
d    directory
l    symbolic link
c    character device
b    block device
```

For example:

```text
-rw-r--r--   regular file
drwxr-xr-x   directory
lrwxrwxrwx   symbolic link
crw-rw-rw-   character device
brw-rw----   block device
```

---

# Owner and group

Every file has:

```text
owner
group
permissions
```

Example:

```bash
-rw-r--r-- 1 fabio users 1200 Jun 17 10:20 notes.txt
```

Here:

```text
owner: fabio
group: users
```

The **owner** is usually the user who created the file.

The **group** is a collection of users. Group ownership allows several users to share access to the same file.

For example, if several users belong to the group `project`, then a file owned by the group `project` can be made readable or writable by all members of that group.

---

# Permissions

Unix permissions define who can **read, modify, or execute** a file.

There are three permission classes:

```text
user    the owner of the file
group   users belonging to the file group
other   everyone else
```

For each class, there are three basic permissions:

```text
r    read
w    write
x    execute
```

Example:

```text
-rw-r--r--
```

Ignore the first character for the moment. The permissions are:

```text
rw-   r--   r--
user  group other
```

Meaning:

```text
user:  read and write
group: read only
other: read only
```

So:

```bash
-rw-r--r-- 1 fabio users notes.txt
```

means that:

```text
fabio can read and modify the file
users in the group can read it
everyone else can read it
```

## Permissions on regular files

For regular files, permissions have this meaning:

```text
r    read the contents of the file
w    modify the contents of the file
x    execute the file as a program or script
```

Example:

```text
-rwxr-xr--
```

This means:

```text
user:  read, write, execute
group: read, execute
other: read only
```

## Permissions on directories

For directories, permissions have a different meaning.

```text
r    list the names inside the directory
w    create, delete, or rename entries inside the directory
x    enter the directory and access files inside it
```

Example:

```text
drwxr-xr-x code
```

This means:

```text
user:  can enter, list, and modify the directory
group: can enter and list the directory
other: can enter and list the directory
```

The `x` permission on directories is especially important.

Without `x`, you cannot enter the directory:

```bash
cd directory
```

Without `r`, you cannot list the directory:

```bash
ls directory
```

Without `w`, you cannot create or delete files inside it.

To create or delete a file inside a directory, you need write permission on the directory, not only on the file.

---

# Changing permissions with `chmod`

The command `chmod` changes file permissions.

Examples:

```bash
chmod u+x script.sh
chmod g+w data.txt
chmod o-r secrets.txt
chmod a+r public.txt
```

Meaning:

```text
u+x    add execute permission for the owner
g+w    add write permission for the group
o-r    remove read permission from others
a+r    add read permission for everyone
```

Permissions can also be written using numbers.

The values are:

```text
r = 4
w = 2
x = 1
```

Then:

```text
rwx = 4 + 2 + 1 = 7
rw- = 4 + 2     = 6
r-x = 4 + 1     = 5
r-- = 4         = 4
--- = 0
```

An example which uses numbers:

```bash
chmod 755 script.sh
```

means:

```text
user:  rwx = 7
group: r-x = 5
other: r-x = 5
```

So the file becomes:

```text
-rwxr-xr-x
```

Common modes are:

```text
644    regular readable file
755    executable program or directory
600    private file readable only by the owner
700    private directory accessible only by the owner
```


> [!IMPORTANT]
> A script needs execute permission to be run directly: `./script.sh`
> 
> If the file is not executable, the shell may return: `Permission denied`
> 
> You can add execute permission with:
> ```bash
> chmod u+x script.sh
> ```

---

# Changing ownership with `chown`

The owner and group of a file can be changed.

The command `chown` changes the owner.

Example:

```bash
sudo chown fabio notes.txt
```

This sets the owner of `notes.txt` to `fabio`.

To change both owner and group:

```bash
sudo chown fabio:users notes.txt
```

Meaning:

```text
owner: fabio
group: users
```

The command `chgrp` changes only the group:

```bash
chgrp project data.txt
```

This sets the group of `data.txt` to `project`.

On most Unix systems, changing ownership requires administrator privileges.

---

# The root user

The **root user** is the system administrator.

Root has user ID 0.

Root can usually read, modify, and delete files even when normal users cannot.

The command:

```bash
sudo command
```

runs `command` with administrator privileges, usually as root.

Example:

```bash
sudo chown root:root system_file
```

Another example:

```bash
sudo rm file.txt
```

removes `file.txt` with root privileges.

> [!WARNING]
> Root is powerful and dangerous. A wrong command executed as root can damage the system.
> For example:
> ```bash
> sudo rm -r important_directory
> ```
> can delete files even if the normal user would not have permission.

The basic rule is:

```text
use root privileges only when necessary
```

> [!TIP]
> Most work should be done as a normal user.

---

# Summary

The Unix filesystem is a tree starting from:

```text
/
```

Everything is placed somewhere below this root directory.

Every file has:

```text
owner
group
permissions
```

Permissions are divided into:

```text
user
group
other
```

The basic permissions are:

```text
r    read
w    write
x    execute
```

The root user is the administrator and can override most permission restrictions.

Ownership can be changed with:

```bash
chown
chgrp
```

Permissions can be changed with:

```bash
chmod
```

The central Unix idea is:

```text
everything is organized as a file in a single filesystem tree
```
