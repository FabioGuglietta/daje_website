# Introduction to Operating Systems

## What is an Operating System?

An operating system (OS) is the foundational **software** that manages a computer’s hardware and provides services to application programs. It serves as an **intermediary between user** applications and the physical components of the **machine**.

Without an operating system, user programs would have to manage the hardware directly — a complex and error-prone task. The OS handles this complexity, offering a structured and standardized interface to the system's resources.

## Core Functions of an Operating System

1. **Process Management**  
   The OS **handles** the creation, scheduling, and termination of **processes**. It ensures that programs get the CPU time they need and that multiple applications can run concurrently without interfering with each other.

2. **Memory Management**  
   The OS **allocates and deallocates memory** to programs, manages **virtual memory**, and ensures that processes do not overwrite each other's memory.

3. **File System Management**  
   The OS organizes data into **files** and **directories**, manages **permissions**, and abstracts the underlying hardware so users can interact with data consistently.

4. **Device Management**  
   The OS provides **drivers** and **interfaces** to communicate with **hardware devices** such as keyboards, disks, printers, and network interfaces.

5. **User Interface**  
   Most operating systems offer a **user interface** — either **command-line-based** or **graphical** — that allows users to interact with the system.

## Examples of Operating Systems

There are many operating systems, each with its own history and architecture. Two major families that dominate computing environments today are:

### 1. Windows

- Developed by **Microsoft**.
- **Proprietary** and widely used in personal and office environments.
- Known for its **graphical user interface** (GUI) and tight integration with Microsoft software products.
- Typically **not open-source**; users cannot inspect or modify its source code.
- Emphasizes ease of use, extensive driver support, and compatibility with commercial software.

### 2. Unix (and Unix-like systems, such as Linux)

- Originally developed in the 1970s at Bell Labs.
- Emphasizes **modularity**, **portability**, and the use of small composable tools.
- Many modern OSs are Unix-based or inspired by Unix, including:
  - **Linux** distributions (Ubuntu, Fedora, Debian, etc.)
  - **macOS** (a certified Unix OS)
- Often **open-source** (especially **Linux**), allowing users to inspect, modify, and redistribute the code.
- Commonly **used in servers**, **supercomputers**, **academic** environments, and increasingly in desktops and embedded systems.

## Key Differences Between Windows and Unix/Linux

| Feature                  | Windows                         | Unix/Linux                         |
|--------------------------|----------------------------------|------------------------------------|
| Source Code              | Closed source (proprietary)      | Often open source                  |
| Interface                | Primarily graphical              | CLI-based tools plus GUI           |
| Filesystem               | NTFS, drive letters (C:, D:)     | Unified root (`/`), ext4, XFS, etc.|
| User Base                | Personal desktops, businesses    | Servers, HPC, developers, embedded |
| Security Model           | Historically user-friendly focus | Strong multi-user permissions      |
| Customizability          | Limited                          | Highly customizable                |


# Unix and Linux operating systems

Unix and Linux are, loosely speaking, two families of [operating systems](https://en.wikipedia.org/wiki/Operating_system) (OSs) That's the software that sits between the computer hardware and the usual programs.

*   Unix is rather old (1969) and used to be proprietary software. Linux [was born](https://www.cs.cmu.edu/~awb/linux.history.html) as a free ([as in free speech](https://en.wikipedia.org/wiki/Gratis_versus_libre)) implementation of Unix for the [Intel 80386](https://en.wikipedia.org/wiki/I386), a popular processor used in [IBM PC clones](https://en.wikipedia.org/wiki/IBM_PC_compatible), that [paved the way to their ubiquity](https://www.tomshardware.com/reviews/history-of-computers,4518-32.html).
*   Historically, Unix is tied with the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)), in which it was written (the first time for an OS! back then OSs were written in [assembly](https://en.wikipedia.org/wiki/Assembly_language)). Unix was also instrumental to the birth of Internet (and later of the www).
*   Today, Unix and Linux are playing a huge role in the [Internet infrastructure](https://en.wikipedia.org/wiki/Internet_backbone), [mobile computing](https://en.wikipedia.org/wiki/Mobile_computing) and [High Performance Computing (HPC)](https://en.wikipedia.org/wiki/High-performance_computing). Most of [Internet's servers run on Unix or Linux](https://en.wikipedia.org/wiki/Usage_share_of_operating_systems#Public_servers_on_the_Internet), and the same is true for [routers and firewalls](https://en.wikipedia.org/wiki/List_of_router_and_firewall_distributions), not to mention [Android](https://en.wikipedia.org/wiki/Android_(operating_system)) (another Linux-based OS) phones! [Mac OS X](https://en.wikipedia.org/wiki/MacOS) is also a flavor of unix.
*   Regarding HPC systems, the plot on the side that shows the share of [top500 supercomputers](https://en.wikipedia.org/wiki/TOP500) by OS leaves no doubt that if you want to do computer simulations, you will have to deal with a variant of Linux.

```{image} linux-share.png
:width: "80%"

 Share of TOP500 Supercomputers by OS: Linux dominates.
```
##  Why Unix/Linux Rocks

1.  **Stability & Longevity**  
   The basic structure hasn’t changed much in 50+ years. Terminal commands and system libraries from the 1970s still work.  
   Software you write today might still run in 2065!

2.  **Portability**  
   Programs you write can run on everything from your laptop to a giant supercomputer — with little or no change.

3.  **Massive Community & Free Tools**  
   Tons of free, open-source tools are available — and you can read, modify, and share the code.  
   Online guides, forums, and tutorials are everywhere.

> [!NOTE]
> Linux is technically just the [kernel](https://en.wikipedia.org/wiki/Kernel_(operating_system)) — the core engine that:
> - Manages memory
> - Schedules CPU tasks
> - Talks to your devices (keyboard, disk, network…)
> 
> To make a complete OS, we also need:
> - System libraries (like the [GNU C Library (glibc)](https://en.wikipedia.org/wiki/Glibc)) to open files, allocate memory, etc.
> - User programs: terminal, editor, compiler, etc.
> 
> This bundle (**kernel + libraries + programs**) is what we call a [Linux distribution](https://en.wikipedia.org/wiki/Linux_distribution) — like Ubuntu, Fedora, Arch, etc.

# Command-Line Interface (CLI) vs Graphical User Interface (GUI)

Modern operating systems offer users different ways to interact with the system: the **command-line interface** (CLI) and the **graphical user interface** (GUI). Understanding the strengths and limitations of each interface is crucial for using computers effectively, especially in technical and scientific computing.

### Command-Line Interface (CLI)

A command-line interface allows users to interact with the system by **typing text commands**. You enter these commands in a **terminal** or console window.

Examples of command-line environments:
- **Bash** (Linux, macOS)
- Command Prompt or PowerShell (Windows)
- **Zsh** or Fish (Unix-like systems)

### Example:

```bash
cd Documents
ls -l
mkdir new_folder
```

This sequence changes the current directory to `Documents`, lists the files in long format, and creates a new folder called `new_folder`.

**CLI** tools are especially **useful** for **developers**, **system administrators**, and **scientists** who need to automate tasks, run **remote jobs**, or perform bulk operations efficiently.

### Graphical User Interface (GUI)

A graphical user interface provides a **visual and interactive way** of using a computer. It uses elements like **windows**, **icons**, **buttons**, and **menus**. Users **interact using a mouse**, touchpad, or **touchscreen**.

Common GUI environments:
- **Windows Desktop** (Windows OS)
- **Finder** (macOS)
- **GNOME**, KDE (Linux desktop environments)

Example:

To create a folder using a GUI:
	1.	Open the file manager
	2.	Navigate to the “Documents” folder
	3.	Right-click and select “New Folder”
	4.	Type the folder name and press Enter

GUIs are **intuitive** and **beginner-friendly**, ideal for tasks like **browsing the web**, **managing files**, or using **visual applications** like text editors, media players, or image editors.

### Key Differences

| Feature               | Command-Line Interface (CLI)        | Graphical User Interface (GUI)        |
|-----------------------|--------------------------------------|----------------------------------------|
| Interaction Method    | Text-based commands                 | Visual elements (windows, icons, menus) |
| Learning Curve        | Steeper (requires memorization)     | Gentler (intuitive for beginners)      |
| Speed for Experts     | Very fast for repetitive tasks      | Slower for complex or repeated tasks   |
| Automation            | Easily scriptable                   | Harder to automate                     |
| System Resource Usage | Low (no graphics required)          | Higher (requires graphical system)     |
| Remote Access         | Ideal (via terminal/SSH)            | Often unavailable or limited           |
| Accessibility         | Usable on minimal hardware          | Requires full desktop environment      |
| Typical Use Cases     | Programming, servers, HPC, scripting| General desktop use, browsing, media   |

### When to Use CLI vs GUI

Use CLI when:
- Performing **repetitive tasks** (e.g., batch renaming files)
- Managing **servers** or working **remotely**
- **Automating workflows** with shell scripts
- **Developing** and **compiling code**

Use GUI when:
- Doing **visual tasks** (e.g., drawing, web browsing)
- Managing files occasionally or casually
- Using applications designed for **graphical interaction**
- You’re **new to computing** and prefer visual feedback
- You're bored and want to **play Solitaire** — or just click around aimlessly until something happens :D

### Real-World Examples

**CLI Example:** Copying all `.txt` files from one folder to another

```cp ~/Documents/*.txt ~/Backup/```

This single command copies all `.txt` files from the `Documents` directory to `Backup`.

**GUI Alternative:**
	1.	Open the file manager
	2.	Go to Documents
	3.	Select all .txt files
	4.	Drag them into the Backup folder

The **CLI is faster and scriptable**. The GUI is easier for small tasks but harder to automate.

### Complementary Use

CLI and GUI are **NOT mutually exclusive** — many advanced users combine both:
- Use GUI to **read documentation** or **preview results**
- Use CLI to **run simulations**, **scripts**, or **manage code**
- Many IDEs integrate terminals, so users can switch seamlessly

Being comfortable with both interfaces allows for **flexibility** and **efficiency**.
