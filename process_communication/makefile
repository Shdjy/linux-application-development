CC:=gcc

.PHONY: named_fifo named_fifo_clean p_c_mq p_c_mq_clean

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
	o-rm ./$@

mqueue_producer: mqueue_producer.c
	-$(CC) -o $@ $^

mqueue_consumer: mqueue_consumer.c
	-$(CC) -o $@ $^

p_c_mq: mqueue_producer mqueue_consumer

p_c_mq_clean:
	-rm ./mqueue_producer ./mqueue_consumer

signal: signal.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
