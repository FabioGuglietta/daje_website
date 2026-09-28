This page will show you how to stop/resume jobs, or put them in background / foreground.

Now that you know how to start some commands on bash, it's time to learn how to handle the long ones. You could of course open a new terminal if a command takes too long to execute. But this is often cumbersome and might not be what you want (suppose you are chain-connecting through three servers to reach your target. If you open a new terminal on your machine you will have to repeat the whole sequence)

  

A typical use case is when you want to launch a graphical user interface (GUI) program from the terminal, and be able to still using the terminal (to launch other programs, check outputs and so on). Suppose the GUI program is called `myprogram` , then by putting an ampersand ( `&` ) symbol at the end, it will go directly to background, letting you able to use the terminal.

```bash
> myprogram &  # this will open a window
> ls           # we can still use the terminal
Applications    Documents
```
  

Mind that if `myprogram` is sending output to the terminal, it will overlap with what you see in your shell. If it keeps producing output, you might not be able to see what you are typing! The way to go around this is to redirect the output either to a file

```bash
> myprogram > log &  # this will place all output in the file called log
> ls           # we can still use the terminal
Applications    Documents
```
  

or to the special file `/dev/null` , which discards the output

```bash
> myprogram > /dev/null &  # this will discard all output
> ls           # we can still use the terminal
Applications    Documents
```
  

Note that unix systems have a different type of output channel, besides `stdout` , which is called the standard error ( `stderr` ). It will still end up in your terminal's output. So, you might want to catch or get rid of it as well. The way to do it is to first redirect `stdout` , and then `stderr` to `stdout`. It's easier than it sounds:

```bash
> ( myprogram > log 2>&1 ) &  # this will put both sdtout (1) and stderr (2) channels into log
> ls           # we can still use the terminal
Applications    Documents
```
  

To stop a running program, simply hit `ctrl+z`. An easy example with the sleep command (sleep just waits and does nothing, so there won't be much to see...):

```bash
> sleep 100000
^Z
[1]+  Stopped          sleep 100000
>
```
  

You can put it into background, so that it will keep running (sleep does nothing, but this is just an example...) while you are free to use the terminal

```bash
> bg 1
[1]+ sleep 100000 &
> ls
Applications   Documents
>
```
  

And you can put it back in foreground with `fg`

```bash
> fg 1
sleep 100000
```
  

To cancel a job in foreground, hit `ctrl+c`

```bash
> fg 1
sleep 100000
^C
>
```
  

You can see the list of jobs currently handled by your shell using the `job` command :

```bash
> jobs
[1]-  Stopped                 vim test.c
[2]+  Stopped                 vim nothe_test.c
[3]   Running                 sleep 100000 &
[4]   Running                 vmd > /dev/null 2>&1 &
```  

In the example above I have two (character interface) text editors open ( `vim` ) that have been stopped, as well as two commands running in background ( `sleep` and `vmd` )

I can call back into foreground a process by using the `fg` command, stopping it with `ctrl+z` and then putting it back to background with `bg` :

```bash
> fg 3
sleep 100000
^Z
[3]+  Stopped                 sleep 100000
 
> bg 3
[3]+ sleep 100000 &
 
>
```
  

I can go back to one of the stopped editors by putting it into foreground

```bash
> fg 2 # will show the editor interface in the terminal. I can continue writing in the editor now
```

