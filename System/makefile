CC:= gcc


fork : fork.c
	gcc -o fork fork.c
	./fork
	rm fork

fork_fd_test : fork_fd_test.c
	gcc -o fork_fd_test fork_fd_test.c
	./fork_fd_test
	rm fork_fd_test

execve0: execve0.c
	$(CC) -o $@ $^

execve1: execve1.c execve0
	$(CC) -o $@ $<
	./$@
	rm ./$@ ./$(word 2, $^)

fork_execve : fork_execve.c execve0
	-$(CC) -o $@ $(firstword $^)
	-./$@
	-rm ./$@ ./$(lastword $^)

waitpid : waitpid.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pstree : pstree.c execve0
	-$(CC) -o $@ $(firstword $^)
	-./$@
	-rm ./$@ ./$(lastword $^)

child_process : child_process.c
	-$(CC) -o $@ $^
	
orphan_process : orphan_process.c child_process
	-$(CC) -o $@ $<
	-./$@
	-rm ./$@ ./$(word 2, $^)
