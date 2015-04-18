
#comment out to disable profiling
#enablegp=-pg
copts="-fgnu89-inline"

globes-REdiscovery.o: globes-REdiscovery.c globes-REdiscovery.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o globes-REdiscovery.o globes-REdiscovery.c ${enablegp}
	
customChis.o: customChis.c customChis.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o customChis.o customChis.c ${enablegp}
	
priors.o: priors.c priors.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o priors.o priors.c ${enablegp}
	
mctools.o: mctools.c mctools.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o mctools.o mctools.c ${enablegp}
	
mgt_helper.o: mgt_helper.c mgt_helper.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o mgt_helper.o mgt_helper.c ${enablegp}

sigdcp.o: sigdcp.c sigdcp.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o sigdcp.o sigdcp.c ${enablegp}

senseVlum.o: senseVlum.c senseVlum.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o senseVlum.o senseVlum.c ${enablegp}

dcpVsTheta.o: dcpVsTheta.c dcpVsTheta.h
	gcc -std=c99 ${copts} -static -g -Wall -g -O2 -c -o dcpVsTheta.o dcpVsTheta.c ${enablegp}
	
mgt: mgt.c sigdcp.o customChis.o priors.o mgt_helper.o senseVlum.o dcpVsTheta.o mctools.o globes-REdiscovery.o
	gcc  -std='c99' ${copts} -o mgt ${GLBT_LIB_PATH}/glb_tools_parser.o ${GLBT_LIB_PATH}/glb_tools_header.o ${GLBT_LIB_PATH}/glb_tools_eightfold.o priors.o mctools.o sigdcp.o customChis.o mgt_helper.o senseVlum.o dcpVsTheta.o globes-REdiscovery.o -lglobes -lgsl -lgslcblas -lm -L${GLBT_LIB_PATH}/lib -L${INSTALLATION}/lib mgt.c ${enablegp}

all: mgt
