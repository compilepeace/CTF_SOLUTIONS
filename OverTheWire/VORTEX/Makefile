

.PHONY: clean

T1= client
TARGETS= $(T1) 

all: $(TARGETS)


$(T1): $(T1).c
	gcc $< -o $@


clean:
	rm $(TARGETS)
