-include ../tools.mk

all:
	$(DUSTDOC) -Z unstable-options --print unversioned-files | sort | diff - unversioned-files.txt
