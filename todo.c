/* 📙 TODO, as stated by the subject:

✔️•Display a prompt when waiting for a new command.

✔️•Have a working history.

✔️•Search and launch the right executable (based on the PATH variable or using a
relative or an absolute path).

⚠️•Avoid using more than one global variable to indicate a received signal. Consider
the implications: this approach ensures that your signal handler will not access your
main data structures.

✔️•Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).
	✔️ prints "minishell: syntax error: unclosed quotes"
	✔️ prints \ and ; as they are

✔️•Handle ’ (single quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence.

✔️•Handle " (double quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence except for $ (dollar sign).

•Implement redirections:
	◦< should redirect input.
	◦> should redirect output.
	◦<< should be given a delimiter, then read the input until a line containing the
	delimiter is seen. However, it doesn’t have to update the history!
	◦>> should redirect output in append mode.

•Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe. ✔️ need to handle quotes!! 💡 check multiple pipes (cat | cat | ls)
❌ so far it assumes that | is a command, not a separator, so we need to sort of create a 2D array separating the commands for each execution after if finds a pipe
❌❌ if it finds a |, create a new command and execute it, then continue with the next command or pipe
💡 if the last character of the input is a pipe or space or tab after a pipe | (ls -l | wc -l | ), it should return a message saying that an argument is missing or invalid pipe, instead it just tries to execute the command ""

❌•Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.
	✔️$HOME	/home/user
	✔️$PATH	/usr/bin:/bin:/usr/local/bin
	✔️$USER	username
	✔️$SHELL	/bin/bash (or your shell’s path)
	✔️$?	Last command’s exit status
	❌$!	PID of the last background command ❌ see if this is only used/updated when a command is run in the background (with & at the end) or evey time we fork
	✔️$$	PID of the shell
	✔️$0	shell name 💡 ./minishell
	✔️$PWD	current working directory
	✔️$OLDPWD	previous working directory

✔️•Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.

✔️•Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.

•In interactive mode:
	✔️◦ctrl-C displays a new prompt on a new line.
	✔️◦ctrl-D exits the shell.
	✔️◦ctrl-\ does nothing.

•Your shell must implement the following
	✔️ ◦echo with option -n
	❌◦cd with only a relative or absolute path ✔️ error when directory does not exist ❌ cd ..\ does not behave like the shell (interactive mode??)
	✔️◦pwd with no options
	◦export with no options
	◦unset with no options
	◦env with no options or arguments 💡 why is this necessary? are we gonna unset env?
	✔️◦exit with no options


❌ when executing a command that is only spaces or tabs, it gives error: "minishell: /home/user/.local/bin/: Is a directory"

*/


/*
💡 HELPERS:
	- check functions used: nm -Ud minishell | grep " U "
	- check global variables used: nm -g minishell | grep " B "
	- find where a function is used: grep -rnw . -e 'function_name'



===================================================================================================================================================================

📝 EVALUATION SHEET:

Mandatory Part
--------------

Compile
- Use "make -n" to see if compilation use "-Wall -Wextra -Werror". If not, select the "invalid compilation" flag.
- minishell compiles without any errors. If not, select the flag.
- The Makefile must not re-link. If not, select the flag.


Simple Command & global variables
- Execute a simple command with an absolute path like /bin/ls, or any other command without any options.
- How many global variables are used? Why? Ask the evaluated student to give you a concrete example of why it feels mandatory or logical.
- Check the global variable. This global variable cannot provide any other information or data access than the number of a received signal.
- Test an empty command.
- Test only spaces or tabs.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


Arguments
- Execute a simple command with an absolute path like /bin/ls, or any other command with arguments but without any quotes or double quotes.
- Repeat multiple times with different commands and arguments.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


echo
- Execute the echo command with or without arguments, or the -n option.
- Repeat multiple times with different arguments.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


exit
- Execute exit command with or without arguments.
- Repeat multiple times with different arguments.
- Don't forget to relaunch the minishell
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


Return value of a process
- Execute a simple command with an absolute path like /bin/ls, or any other command with arguments but without any quotes and double quotes. Then execute echo $?
- Check the printed value. You can do the same in bash in order to compare the results.
- Repeat multiple times with different commands and arguments. Try some wrong commands like '/bin/ls filethatdoesntexist'
- Try anything like expr $? + $?
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


Signals
- ctrl-C in an empty prompt should display a new line with a new prompt.
- ctrl-\ in an empty prompt should not do anything.
- ctrl-D in an empty prompt should quit minishell --> RELAUNCH!
- ctrl-C in a prompt after you wrote some stuff should display a new line with a new prompt.
- The buffer should be clean too. Press "Enter" to make sure nothing from the previous line is executed.
- ctrl-D in a prompt after you wrote some stuff should not do anything.
- ctrl-\ in a prompt after you wrote some stuff should not do anything.
- Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
- Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.
- Try ctrl-D after running a blocking command like cat without arguments or grep “something“.
- Repeat multiple times using different commands.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


Double Quotes
- Execute a simple command with arguments and, this time, use also double quotes (you should try to include whitespaces too).
- Try a command like : echo "cat lol.c | cat > lol.c"
- Try anything except $.
- If something crashes, select the "crash" flag.
- If something doesn't work, select the "incomplete work" flag.


Single Quotes
- Execute commands with single quotes as arguments.
- Try empty arguments.
- Try environment variables, whitespaces, pipes, redirection in the single quotes.
- echo '$USER' must print "$USER".
- Nothing should be interpreted.


env
- Check if env shows you the current environment variables.


export
- Export environment variables, create new ones and replace old ones.
- Check the result with env.


unset
- Export environment variables, create new ones and replace old ones.
- Use unset to remove some of them.
- Check the result with env.


cd
- Use the command cd to move the working directory and check if you are in the right directory with /bin/ls
- Repeat multiple times with working and not working cd
- Also, try '.' and '..' as arguments.


pwd
- Use the command pwd.
- Repeat multiple times in different directories.


Relative Path
- Execute commands but this time use a relative path.
- Repeat multiple times in different directories with a complex relative path (lots of ..).


Environment path
- Execute commands but this time without any path (ls, wc, awk and so forth).
- Unset the $PATH and ensure commands are not working anymore.
- Set the $PATH to a multiple directory value (directory1:directory2) and ensure that directories are checked in order from left to right.


Redirection
- Execute commands with redirections < and/or >
- Repeat multiple times with different commands and arguments and sometimes change > with >>
- Check if multiple tries of the same redirections fail.
- Test << redirection (it doesn't have to update the history).


Pipes
- Execute commands with pipes like 'cat file | grep bla | more'
- Repeat multiple times with different commands and arguments.
- Try some wrong commands like 'ls filethatdoesntexist | grep bla | more'
- Try to mix pipes and redirections.


Go Crazy and history
- Type a command line, then use ctrl-C and press "Enter". The buffer should be clean and there should be nothing left to execute.
- Can we navigate through history using Up and Down? Can we retry some command?
- Execute commands that should not work like 'dsbksdgbksdghsd'. Ensure minishell doesn't crash and prints an error.
- 'cat | cat | ls' should behave in a "normal way".
- Try to execute a long command with a ton of arguments.
- Have fun with that beautiful minishell and enjoy it!


Environment variables
- execute echo with some environment variables ($variable) as arguments.
- Check that $ is interpreted as an environment variable.
- Check that double quotes interpolate $.
- Check that USER exists. Otherwise, set it.
- echo "$USER" should print the value of the USER variable.



Bonus
-----
Evaluate the bonus part if, and only if, the mandatory part has been entirely and perfectly done, and the error management handles unexpected or bad usage. In case all the mandatory points were not passed during the defense, bonus points must be totally ignored.

And, Or
- Use &&, || and parenthesis with commands and ensure minishell behaves the same way bash does.


Wildcard
- Use wildcards in arguments in the current working directory.


Surprise! (or not...)
- Set the USER environment variable.
- echo "'$USER'" should print the value of the USER variable.
- echo '"$USER"' should print "$USER".

*/
