bin:
	g++ stripParameterSets.cc `root-config --cflags --libs` -I $(SQLITE_FQ_DIR)/include/ -I $(ART_DIR)/source -L $(SQLITE_LIB) -lsqlite3_ups -ostripParameterSets
	mv stripParameterSets $(SRT_PRIVATE_CONTEXT)/bin/$(SRT_SUBDIR)/

clean:
	rm -f $(SRT_PRIVATE_CONTEXT)/bin/$(SRT_SUBDIR)/stripParameterSets

lib:

codegen:

include:

depend:

libobjects:

checkdicts:
