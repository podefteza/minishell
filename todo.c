/*❌ TODO:

❌❌❌❌ SAVE THE LAST EXIT CODE IN THE GLOBAL VARIABLE WHEN EXITING!!! ❌❌❌❌

✔️•Display a prompt when waiting for a new command.

✔️•Have a working history.

•Search and launch the right executable (based on the PATH variable or using a
relative or an absolute path).

•Avoid using more than one global variable to indicate a received signal. Consider
the implications: this approach ensures that your signal handler will not access your
main data structures.

•Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).

•Handle ’ (single quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence.

•Handle " (double quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence except for $ (dollar sign).

•Implement redirections:
	◦< should redirect input.
	◦> should redirect output.
	◦<< should be given a delimiter, then read the input until a line containing the
	delimiter is seen. However, it doesn’t have to update the history!
	◦>> should redirect output in append mode.

•Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe. ✔️ need to handle quotes!! 💡 check multiple pipes (cat | cat | ls)

•Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.

✔️•Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.
	✔️$HOME	/home/user
	✔️$PATH	/usr/bin:/bin:/usr/local/bin
	✔️$USER	username
	✔️$SHELL	/bin/bash (or your shell’s path)
	✔️$?	Last command’s exit status

•Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.

•In interactive mode:
	◦ctrl-C displays a new prompt on a new line.
	◦ctrl-D exits the shell.
	◦ctrl-\ does nothing.

•Your shell must implement the following
	✔️✔️◦echo with option -n ❌ echo with quotes > if command is echo, don't trim the input
	✔️✔️◦cd with only a relative or absolute path ✔️ error when directory does not exist ❌ cd ..\ does not behave like the shell (interactive mode??) ❌ cd ~ (see shorten_path())
	✔️◦pwd with no options
	◦export with no options
	◦unset with no options
	◦env with no options or arguments 💡 why is this necessary? are we gonna unset env?
	✔️◦exit with no options ✔️ also works with an exit code
*/
