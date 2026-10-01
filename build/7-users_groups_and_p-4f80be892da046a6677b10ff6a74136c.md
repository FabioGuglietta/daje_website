# Users, Groups, and Privileges

Unix is a **multiuser** system: several people and services can use the same
computer while keeping their resources and responsibilities separate.
This matters on a shared teaching server or computing cluster, but also on a
laptop running background services.

## Users and accounts

A **user account** represents an identity recognized by the system. It usually
has a readable name and a numeric **user identifier**, or **UID**.
The operating system uses numeric identities when making access decisions.

An account need not represent a person. Services can run under dedicated
accounts so that each service receives only the access it needs.

A **home directory** is an account's usual personal workspace. It is a location
in the filesystem, not the account itself. Its access permissions determine
who else can read or modify its contents; being a home directory does not
by itself guarantee privacy.

## Groups

A **group** collects accounts that need common access. Groups have names and
numeric **group identifiers**, or **GIDs**. A user can belong to several groups.

For example, Alice and Bruno work on the same simulation project. Each keeps
an individual account, while both belong to a project group. Shared files can
then grant access to that group without becoming writable by every user on
the machine.

Group membership and the permissions of the resource work together. Joining
a group does not automatically grant access to every file on the system.

## Processes act with an identity

When an account starts an ordinary application, its process normally runs
with that account's identity and group memberships. Access checks are made
for the process requesting the operation.

For example, an editor saving a file does not receive special access merely
because it is an editor. The operating system checks whether its identity is
allowed to perform the requested operation.

A user can therefore be allowed to read one dataset, modify a second, and
have no access to a third. Permission is about both a resource and an operation.

## Authentication and authorization

These concepts answer different questions:

| Concept | Question | Example |
| --- | --- | --- |
| Authentication | Who are you? | Proving control of an account when logging in |
| Authorization | What may you do? | Deciding whether that account may modify a file |

Successfully logging in does not grant unrestricted access. Likewise, knowing
a file's path does not imply permission to read it.

## The administrator and root

The traditional Unix administrator account is **root**, with UID 0. It has
broad privileges, including the ability to perform many operations that
ordinary users cannot.

The **root user** and the **root directory** are different concepts:

- root is an account with administrative privileges;
- `/` is the top of the directory tree.

Modern systems can impose additional restrictions or delegate particular
privileges. For introductory work, the important distinction is that ordinary
applications should run with ordinary user access, while administrative tasks
require explicitly granted privileges.

## Least privilege

The **principle of least privilege** means granting only the access needed for
a task. Editing a personal report should not require permission to replace
system software or change another student's files.

This limits the consequences of mistakes as well as faulty programs. A shared
project group is usually a better fit for collaboration than giving every
participant administrator access.

## Example: a shared research machine

Alice and Bruno need to read a common dataset and write their own results.
A sensible arrangement could provide:

- individual accounts and personal output directories;
- a project group with read access to the common dataset;
- a shared results directory with group write access when collaboration requires it;
- separate administrative access for managing the machine.

The intended access should guide the permissions, rather than making every
file writable simply to avoid access errors.

## Summary

Accounts identify users and services; groups organize shared access. Processes
request operations under an identity, and the operating system enforces the
applicable access rules. Administrative privileges should be reserved for
operations that need them.

The practical details of ownership, permission bits, and the commands used to
change them are covered in
[File Ownership and Permissions](../2-Working_with_the_Unix_Shell_Basic/92-file_ownership_and_permissions.md).
