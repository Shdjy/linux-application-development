CC:=gcc

unnamed_pipe : unnamed_pipe.c
	-$(CC) -o $@ $<
	-./$@ "test"
	-rm ./$@