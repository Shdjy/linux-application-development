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


