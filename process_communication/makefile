CC:=gcc

.PHONY: named_fifo named_fifo_clean

unnamed_pipe : unnamed_pipe.c
	-$(CC) -o $@ $<
	-./$@ "test"
	-rm ./$@

fifo_write: fifo_write.c
	-$(CC) -o $@ $<

fifo_read: fifo_read.c
	-$(CC) -o $@ $<

named_fifo: fifo_write fifo_read

named_fifo_clean:
	-rm ./fifo_write ./fifo_read

shared_memory: shared_memory.c
	-$(CC) -o $@ $<
	-./$@
	-rm ./$@

mqueue: mqueue.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
