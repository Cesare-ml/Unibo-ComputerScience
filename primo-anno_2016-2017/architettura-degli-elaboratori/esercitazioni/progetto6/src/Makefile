main: main.o utils.o vmtranslator.o bootstrap_template.o arithmetic_templates.o memory_access_templates.o flow_templates.o function_templates.o
	gcc -o VMtranslator main.o utils.o vmtranslator.o bootstrap_template.o arithmetic_templates.o memory_access_templates.o flow_templates.o function_templates.o

main.o: main.c
	gcc -c main.c

vmtranslator.o: vmtranslator.c
	gcc -c vmtranslator.c

utils.o: utils.c
	gcc -c utils.c

bootstrap_template.o: bootstrap_template.c
	gcc -c bootstrap_template.c

arithmetic_templates.o: arithmetic_templates.c
	gcc -c arithmetic_templates.c

memory_access_templates.o: memory_access_templates.c
	gcc -c memory_access_templates.c

flow_templates.o: flow_templates.c
	gcc -c flow_templates.c

function_templates.o: function_templates.c
	gcc -c function_templates.c

clean:
	rm *.o main
