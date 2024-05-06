void 
freeargv(char *argv[])
{ 
    for (int i = 0; i < MAX_NUM_ARGS; i++) {
	myfree(argv[i], (MAX_LINE_SIZE+1) * sizeof(char));
    } 

} 

void
testrunner() 
{ 
    int argc, npipes;
    char *argv[MAX_NUM_ARGS];
    Pipeline pipe[MAX_NUM_ARGS];
    char readbuf[1000] = {0};

    mystrcpy(readbuf, "command1 argument1 argument2       argument3 | command2 a1 a2 a3 a4 |             command3      a1 a2 a3");
    argc = parse_commandline(argv, readbuf);
    npipes = parse_pipeline(pipe, argc, argv);
    for (int i = 0; i < npipes; i++) { 
	for (int j = 0; j < pipe[i].argc; j++) {
	    myfprintf(sys_stdout, "pipe[%d].argv[%d] = %s\n",i , j, pipe[i].argv[j]);
	}
	puts("\n");
    }
} 
