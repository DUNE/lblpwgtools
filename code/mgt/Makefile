
#comment out to disable profiling
#enablegp=-pg


customChis.o: customChis.c customChis.h
	gcc -std=c99 -static -g -Wall -g -O2 -c -o customChis.o customChis.c ${enablegp}
	
mgt_helper.o: mgt_helper.c mgt_helper.h
	gcc -std=c99 -static -g -Wall -g -O2 -c -o mgt_helper.o mgt_helper.c ${enablegp}

sigdcp.o: sigdcp.c sigdcp.h
	gcc -std=c99 -static -g -Wall -g -O2 -c -o sigdcp.o sigdcp.c ${enablegp}

senseVlum.o: senseVlum.c senseVlum.h
	gcc -std=c99 -static -g -Wall -g -O2 -c -o senseVlum.o senseVlum.c ${enablegp}

dcpVsTheta.o: dcpVsTheta.c dcpVsTheta.h
	gcc -std=c99 -static -g -Wall -g -O2 -c -o dcpVsTheta.o dcpVsTheta.c ${enablegp}
	
mgt: mgt.c sigdcp.o customChis.o mgt_helper.o senseVlum.o dcpVsTheta.o
	gcc  -std='c99'  -o mgt ${GLBT_LIB_PATH}/glb_tools_parser.o ${GLBT_LIB_PATH}/glb_tools_header.o ${GLBT_LIB_PATH}/glb_tools_eightfold.o sigdcp.o customChis.o mgt_helper.o senseVlum.o dcpVsTheta.o -lglobes -lgsl -lgslcblas -lm -L${GLBT_LIB_PATH}/lib -L${INSTALLATION}/lib mgt.c ${enablegp}

all: mgt
