# The Unix Filesystem: Structure

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

## Files and directories

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

## Absolute and Relative Paths

A **path** identifies the location of a file or directory in the directory tree.
The **current working directory** is the directory used as the starting point
for interpreting a relative path. It can change while you work.

Consider this directory tree:

```text
/
└── home/
    └── m.sega/
        ├── Applications/
        └── Documents/
            ├── Articles/
            ├── Notes/
            └── Minutes.pdf
```

### Absolute paths

An **absolute path** starts from the root directory `/`.
For example:

```text
/home/m.sega/Documents/Minutes.pdf
```

Read it from left to right: start at `/`, enter `home`, then `m.sega`,
then `Documents`, and finally locate `Minutes.pdf`.

Because it starts with `/`, an absolute path does not depend on the current
working directory. It identifies the same location whether you are currently
in `/home/m.sega`, `/tmp`, or another directory.

### Relative paths

A **relative path** does not start with `/`. It is interpreted starting from
the current working directory.

If the current directory is `/home/m.sega`, then:

```text
Documents/Articles
```

means: start in `/home/m.sega`, enter `Documents`, then enter `Articles`.
It therefore identifies the same directory as the absolute path:

```text
/home/m.sega/Documents/Articles
```

If the current directory changes to `/tmp`, the same relative path instead
refers to `/tmp/Documents/Articles`. That location might not exist.
The meaning of a relative path depends on where you start.

### The current directory: `.` and `./`

The entry `.` means **the current directory**. The prefix `./` explicitly
starts a path there; it does not start at the root directory.

With `/home/m.sega` as the current directory, these two relative paths refer
to the same file:

```text
Documents/Minutes.pdf
./Documents/Minutes.pdf
```

Both identify `/home/m.sega/Documents/Minutes.pdf`.

### The parent directory: `..` and `../`

The entry `..` means **the parent directory**, one level above the current
directory. The prefix `../` starts a path from that parent.

If the current directory is `/home/m.sega/Documents`, then:

| Path | Location |
| --- | --- |
| `.` | `/home/m.sega/Documents` |
| `..` | `/home/m.sega` |
| `./Articles` | `/home/m.sega/Documents/Articles` |
| `../Applications` | `/home/m.sega/Applications` |
| `../Documents/Minutes.pdf` | `/home/m.sega/Documents/Minutes.pdf` |
| `../../` | `/home` |

Each additional `../` moves one more level up. At the root directory,
`..` still refers to `/`: there is no directory above the root.

For example, `../Applications` means: move up from `Documents` to `m.sega`,
then locate `Applications`. These are steps for interpreting a path; they
do not themselves change the current working directory.

The practical use of paths is covered in
[Getting Started with the Unix Shell](../2-Working_with_the_Unix_Shell_Basic/3-Shell_and_basic_Unix_commands_pt1.md).

## Mount points

The single directory tree can contain several filesystems, stored on different
disks, removable drives, or network servers. A filesystem becomes accessible
at a directory called a **mount point**.

For example, if a removable drive is mounted at `/media/usb`, its files appear
below that directory:

```text
/
├── home/
├── etc/
└── media/
    └── usb/          mount point for the removable drive
        └── data.txt
```

The path `/media/usb/data.txt` follows the same rules as any other path.
Crossing a mount point does not introduce a separate root into the visible tree.
Exact directory names and mount locations vary between Unix systems.

## Summary

- The directory tree starts at the root directory, `/`.
- Regular files store data; directories organize names of files and subdirectories.
- System directories separate configuration, programs, user data, and devices.
- Absolute paths start at `/`; relative paths start at the current working directory.
- `.` denotes the current directory and `..` its parent; `./` and `../` make these starting points explicit.
- Mount points connect additional filesystems to the same directory tree.

For practical control over access to files and directories, see
[File Ownership and Permissions](../2-Working_with_the_Unix_Shell_Basic/92-file_ownership_and_permissions.md).
