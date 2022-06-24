-include ../../run-make-fulldeps/tools.mk

# only-linux

all:
	$(DUSTC) test.rs --test --target $(TARGET)
	$(shell ulimit -p 0 && $(call RUN,test))
