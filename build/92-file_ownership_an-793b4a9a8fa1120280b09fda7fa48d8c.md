# File Ownership and Permissions

The [Unix filesystem](../1-Operating_Systems_and_Unix/90-unix_filesystem.md) organizes files and directories
in a single tree. Ownership and permissions determine who can access them
and which operations they can perform.

## Reading a long listing

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

## Owner and group

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

## Permissions

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

### Permissions on regular files

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

### Permissions on directories

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

To create, delete, or rename entries, you normally need both `w` and `x`
on the containing directory. Access through a path also requires `x` on
its parent directories. Deleting a file depends on the containing directory's
permissions, rather than write permission on the file itself. Additional
restrictions, such as the sticky bit on shared directories like `/tmp`, can
limit deletion even when the directory is writable.

---

## Changing permissions with `chmod`

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

## Changing ownership with `chown`

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

Changing a file's owner normally requires administrator privileges. The owner
can usually change its group to a group they belong to; other group changes
require administrator privileges.

---

## The root user

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

## Summary

- Each file and directory has an owner, a group, and access permissions.
- `ls -l` displays the file type, permissions, owner, and group.
- Permissions apply to the owner, the group, and other users.
- For files, `r`, `w`, and `x` control reading, writing, and execution.
- For directories, they control listing names, changing entries, and traversal.
- Use `chmod` to change permissions, `chown` to change ownership, and `chgrp`
  to change group ownership.
- The root user is an administrator; it is different from the root directory `/`.
  Use administrator privileges only when necessary.
