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
