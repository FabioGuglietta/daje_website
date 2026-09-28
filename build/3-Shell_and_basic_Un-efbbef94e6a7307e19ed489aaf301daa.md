# Example of Basic Unix Commands
The following example combines the basic shell commands introduced in this lecture into a single workflow.

The goal is to: 
* create a small working directory,
* generate a few text and data files,
* inspect their content,
* copy and move them,
* select files using wildcards,
* combine commands with pipes,
* redirect output to files,
* and finally clean everything up.

In the code block below, each command is followed by a short comment explaining what it does. The symbol `#` starts a comment in Bash: everything after `#` on the same line is ignored by the shell.

The example is meant to be executed step by step in a terminal.
```bash
pwd                                      # print the current directory
ls                                       # list the content of the current directory
echo "Starting shell practice"           # print a message to the screen

cd ~                                     # move to the home directory
pwd                                      # check that we are in the home directory
ls                                       # list files and directories in the home directory

mkdir shell_practice                     # create a new directory
cd shell_practice                        # enter the new directory
pwd                                      # print the absolute path of the current directory
ls                                       # list the content of the current directory; it should be empty

touch empty_file.txt                     # create an empty file
ls                                       # check that the file exists
ls -l empty_file.txt                     # show detailed information about the file

echo "This is the first line" > notes.txt # create a file and write one line into it
echo "This is the second line" >> notes.txt # append a second line to the same file
echo "This is the third line" >> notes.txt # append a third line to the same file

cat notes.txt                            # print the whole content of notes.txt
head notes.txt                           # print the first 10 lines of notes.txt
head -n 2 notes.txt                      # print the first 2 lines of notes.txt
tail notes.txt                           # print the last 10 lines of notes.txt
tail -n 2 notes.txt                      # print the last 2 lines of notes.txt
more notes.txt                           # inspect notes.txt page by page
less notes.txt                           # inspect notes.txt interactively; press q to quit

cat > parameters.txt                     # create a file by typing text from the keyboard; finish with Ctrl+D
cat parameters.txt                       # print the content of parameters.txt

mkdir data                               # create a directory called data
mkdir backup                             # create a directory called backup
mkdir results                            # create a directory called results
ls                                       # list the current directory

touch data/data.1.dat                    # create an empty data file inside data
touch data/data.2.dat                    # create another empty data file inside data
touch data/data.3.dat                    # create another empty data file inside data
touch data/test.txt                      # create a text file inside data
ls data                                  # list the content of the data directory

cp notes.txt backup/                     # copy notes.txt into backup
cp parameters.txt backup/                # copy parameters.txt into backup
ls backup                                # list the content of backup

cp notes.txt notes_copy.txt              # copy notes.txt to a new file called notes_copy.txt
ls                                       # check that notes_copy.txt exists

mv notes_copy.txt notes_renamed.txt      # rename notes_copy.txt to notes_renamed.txt
ls                                       # check the new file name

mv notes_renamed.txt results/            # move notes_renamed.txt into results
ls                                       # check that notes_renamed.txt is no longer here
ls results                               # check that notes_renamed.txt is inside results

ls data/*.dat                            # list all files in data ending with .dat
ls data/data.?.dat                       # list files where ? matches one character
echo data/*.dat                          # show how the wildcard expands
cp data/*.dat backup/                    # copy all .dat files from data into backup
ls backup                                # check that the .dat files were copied

wc notes.txt                             # count lines, words, and bytes in notes.txt
wc -l notes.txt                          # count only the number of lines in notes.txt
wc -w notes.txt                          # count only the number of words in notes.txt
wc -c notes.txt                          # count only the number of bytes in notes.txt

wc < notes.txt                           # send notes.txt to wc through standard input
cat notes.txt | wc                       # send the output of cat to wc through a pipe
cat notes.txt | wc -l                    # count the number of lines using a pipe

ls -1                                    # list one entry per line
ls -1 | wc -l                            # count the number of entries in the current directory
ls -1 data                               # list one entry per line inside data
ls -1 data | grep data                   # keep only entries containing the word data
ls -1 data | grep data | wc -l           # count entries inside data containing the word data

ls -1 data > data_files.txt              # redirect the list of files in data to data_files.txt
cat data_files.txt                       # inspect data_files.txt
ls -1 data | grep ".dat" > dat_files.txt # save only .dat file names into dat_files.txt
cat dat_files.txt                        # inspect dat_files.txt
ls -1 data | grep ".dat" | wc -l > count.txt # count .dat files and save the result
cat count.txt                            # inspect the count

cd data                                  # move into the data directory using a relative path
pwd                                      # check the current directory
ls                                       # list the content of data

cd ..                                    # move to the parent directory
pwd                                      # check that we are back in shell_practice

ls ./data                                # list data using a path starting from the current directory
ls ../shell_practice                     # list shell_practice using a path through the parent directory
ls ~/shell_practice                      # list shell_practice using the home-directory shortcut

cd results                               # move into results
pwd                                      # check the current directory
cd -                                     # return to the previous directory
pwd                                      # check the current directory again

rm empty_file.txt                        # remove an empty file
rm results/notes_renamed.txt             # remove a file inside results
ls results                               # check that results is now empty

rmdir results                            # remove the empty results directory
ls                                       # check that results was removed

rm data/test.txt                         # remove one file from data
rm data/*.dat                            # remove all .dat files from data
ls data                                  # check that data is now empty

rmdir data                               # remove the empty data directory
rm backup/*                              # remove all files inside backup
rmdir backup                             # remove the empty backup directory

rm notes.txt                             # remove notes.txt
rm parameters.txt                        # remove parameters.txt
rm data_files.txt                        # remove data_files.txt
rm dat_files.txt                         # remove dat_files.txt
rm count.txt                             # remove count.txt

cd ..                                    # move out of shell_practice
rmdir shell_practice                     # remove the now-empty shell_practice directory
pwd                                      # print the current directory
ls                                       # list the current directory
```
