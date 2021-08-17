# mysh - Project to create a shell program in C.

Running with no arguments is interactive, running with one argument is batch.

Takes long lines of text by using getline function.

Multiple commands may be run via & operator.

Any parallel commands are all run at the same time. The shell does not wait for one to end before launching the next.

Tokenizes strings to seperates out commands.

Limited by requirements to execv, not execvp.

Must have own path variable.

Must have the cd, exit, and path commands as part of program rather than calling them.

Must support redirect operator > for all called commands. The built in commands are not supported by this.

Do not support selecting a binary directly via full path.

Verify that the path + command works via access before runing it.
