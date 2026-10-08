## to-do:
- [x] write-ahead log
- [ ] multiple commands
- [ ] ...

## write-ahead log

it's just gonna be a log of all of the previous commands that were ran in a previous instance.
once a command is run, the server is first going to write the previous command to the log, then apply the changes in memory.
on restart, the server will run each command again before allowing the client to pass any commands.
logically, GET commands will be ommited, as those don't change the state of the memory.

i could've just had the log file contain all of the data as is stored, and somehow slap it back into memory on a restart.
apparently redis supports both, but i think it's a good and simple enough start for this project.

since we're more focused on the speed of retrieving/storing the data, i'm going to use flush() to pass the data from std::ofstream directly to the os buffer. fsync() would be a more reliable alternative, as it writes directly to the disk, but for the same reason it's going to be slower.

#### strategy:

- give the Server class a std::ofstream log object;
- create a new apply() method that simply loads into memory the changes that the command specified. handle_command() will first log the command, then run the newly created apply() method. if, on restart, we would've used handle_command() in it's current state, the modifications will be applied into memory, but the commands in the log would be logged again.

#### end:

i didn't end up using apply(), instead i added two new methods: **log_command()** and **replay_log()**, and i added an extra parameter to handle_command() to let it know whether commands should be logged or not.
the flush() implementation ended up being easier than i thought, as it is an already existing method in fstream.
