CC:= gcc

create: create.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
	

terminate: terminate.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pthread_detach: pthread_detach.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

race_condition_test: race_condition_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

mutex_test: mutex_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_writer_unlock: rwlock_writer_unlock.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_test: rwlock_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@


rwlock_rw_alternate: rwlock_rw_alternate.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_write_hungry: rwlock_write_hungry.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
	
rwlock_hungry_solveds: rwlock_hungry_solveds.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

condition_var: condition_var.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_bin_thread_condition: unnamed_sem_bin_thread_condition.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_bin_thread: unnamed_sem_bin_thread.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@
	
unnamed_sem_bin_process_condition: unnamed_sem_bin_process_condition.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_bin_process: unnamed_sem_bin_process.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_count_thread: unnamed_sem_count_thread.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_count_process:unnamed_sem_count_process.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

named_sem_bin: named_sem_bin.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

named_sem_count: named_sem_count.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

thread_pool: thread_pool.c
	-$(CC) -o $@ $^ `pkg-config --cflags --libs glib-2.0`
	-./$@
	-rm ./$@
