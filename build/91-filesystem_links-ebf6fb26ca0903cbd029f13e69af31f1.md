# Filesystem: Links

Unix has two main kinds of links:

```text
hard links
symbolic links
```

Both allow a file to be accessed through more than one name.

They work in different ways.

---

## Hard links

A **hard link** is another name for the same underlying file.

Example:

```bash
echo "hello" > file1.txt
ln file1.txt file2.txt
```

Now both names refer to the same file content:

```bash
cat file1.txt
cat file2.txt
```

Both print:

```text
hello
```

If one name is used to modify the file:

```bash
echo "new text" > file2.txt
```

then the other name sees the same content:

```bash
cat file1.txt
```

Output:

```text
new text
```

> [!NOTE]
> A hard link does not point to another filename. **It points to the same internal filesystem object.**

That internal object is called an **inode**.

You can see inode numbers with:

```bash
ls -li
```

Example output:

```bash
12345 -rw-r--r-- 2 fabio users 9 Jun 17 10:20 file1.txt
12345 -rw-r--r-- 2 fabio users 9 Jun 17 10:20 file2.txt
```

The same inode number means that `file1.txt` and `file2.txt` are two names for the same file.

The number after the permissions is the link count:

```text
-rw-r--r-- 2 fabio users ...
```

Here `2` means that two hard links point to the same inode.

Important properties of hard links:

```text
hard links share the same inode
hard links are equal names for the same file
removing one name does not remove the data if another hard link still exists
hard links usually cannot point to directories
hard links usually cannot cross filesystem boundaries
```

Example:

```bash
rm file1.txt
```

The data is not destroyed if `file2.txt` still exists.

---

## Symbolic links

A **symbolic link**, also called a **soft link**, is a small file that points to a path.

Example:

```bash
ln -s file1.txt link.txt
```

Now `link.txt` points to `file1.txt`.

A long listing may show:

```bash
lrwxrwxrwx 1 fabio users 9 Jun 17 10:25 link.txt -> file1.txt
```

The first character is:

```text
l
```

which means symbolic link.

A symbolic link points to a filename or path, not directly to the file content.

If the target is removed:

```bash
rm file1.txt
```

then the symbolic link becomes broken.

Trying to read it:

```bash
cat link.txt
```

may produce:

```text
No such file or directory
```

Important properties of symbolic links:

```text
symbolic links point to paths
symbolic links can point to directories
symbolic links can cross filesystem boundaries
symbolic links can become broken
symbolic links have their own inode
```

Symbolic links are often used to create shortcuts.

Example:

```bash
ln -s /opt/myprogram/bin/myprogram myprogram
```

Now `myprogram` is a link to the real program elsewhere in the filesystem.

---

## Hard links versus symbolic links

| Feature | Hard link | Symbolic link |
|---|---|---|
| Points to | inode | path |
| Can point to directories | usually no | yes |
| Can cross filesystems | usually no | yes |
| Breaks if original name is deleted | no | yes |
| Has same inode as target | yes | no |
| Created with | `ln file link` | `ln -s file link` |

Example hard link:

```bash
ln file.txt hardlink.txt
```

Example symbolic link:

```bash
ln -s file.txt symlink.txt
```
