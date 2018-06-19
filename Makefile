TARGETS = 3ds switch
.PHONY: 3ds switch clean all

all:
	@for dir in $(TARGETS); do $(MAKE) -C $$dir; done

3ds:
	@$(MAKE) -C $(CURDIR)/3ds

switch:
	@$(MAKE) -C $(CURDIR)/switch

clean:
	@for dir in $(TARGETS); do $(MAKE) clean -C $$dir; done