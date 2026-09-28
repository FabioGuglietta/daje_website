# Git - Version Control System

## Why You Should Use Git (and Stop Naming Folders final_final_version_REALLY_FINAL2)
Let’s face it. At some point, we’ve all done it:
```bash
my_code/
├── first_try/
├── version2/
├── version2_final/
├── version2_final_revised/
├── version2_final_revised_NEW/
├── version2_final_revised_NEW_copy/
```
You don’t know where your actual latest code is. Your teammate Ubaldo (or even worse, YOU!) made changes in their own folder called `my_code_Ubaldo_final_v2/` and now your simulations mysteriously produce different results. Welcome to **folder-based version control**! It’s like doing Carbonara with bacon and cream.

### What is version control
What is “version control”, and why should you care? **Version control is a system that records changes to a file or set of files over time** so that you can recall specific versions later. The type of files is not restricted: source code, text files, images, etc. **It allows you to revert selected files back to a previous state**, revert the entire project back to a previous state, compare changes over time, see who last modified something that might be causing a problem, who introduced an issue and when, and more. **Git** is the most famous and currently used version control system. We'll see some basic git functionalities in this lecture.

### Why Git in summary
- **Track changes**: Know what changed, when, and by whom. Even if it was past-you (who can’t be trusted).
- **Undo mistakes**: Broke your code? Git is a time machine. Go back to when things worked.
- **Collaborate** without chaos: No more emailing .zip files named project_final_fixed_really.zip.
- **Parallel development**: Work on new features in isolation using branches, and then merge without fear.
- **One source of truth**: Everyone sees the same history. No more "which folder did you run that from?"

This lecture expects you to know how to open Terminal in macOS or Linux ~~or Command Prompt or PowerShell in Windows~~. There are a lot of different ways to use Git. There are the original command-line tools, and there are many GUIs. The command line is the only place you can run all Git commands -- most of the GUIs implement only a partial subset of Git functionality for simplicity. If you know how to run the command-line version, you can probably also figure out how to run the GUI version, while the opposite is not necessarily true.

> [!Installation and configuration]
>
> The installation of Git will not be covered here. However, Git is so easy to install (e.g. with `apt install git`) and famous that it is probably already installed in you machine. Just open a terminal and type `git --version` to check if it is available.
>
> Additionally, before using Git, it is good practice to configure a couple of things: your name, email, default text editor and default branch name. This steps will need to be done only once.
> ```bash
> > git config --global user.name "John Doe" # username
> > git config --global user.email johndoe@example.com # useremail
> > git config --global core.editor vim # default text editor
> > git config --global init.defaultBranch main # default branch name
> ```
