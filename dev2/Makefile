CC	= gcc
LD	= gcc

#CFLAGS	= -Wall -I/usr/include/shift -I/usr/include/mysql
#LFLAGS	= -Wall -L/usr/lib -lshift -L/usr/lib/mysql -lmysqlclient -lz

CFLAGS  = -Wall -I/usr/include/shift -I/usr/include/mysql
LFLAGS = -Wall -lshift -L/usr/lib/mysql -lmysqlclient -lz #-R/afs/cern.ch/user/t/tilecom/public/www/WIS/

COPY_CASTOR       = copyFromCastor.o
CREATE_FILE       = wisCreateFile.o
DELETE_FILE       = wisDeleteFile.o wisFunctions.o
WGET_URL          = wisWgetURL.o wisFunctions.o
EXIST_FILE        = wisExistFile.o wisFunctions.o
EXIST_SMALL_NTUPLE= wicomExistSmallNtuple.o  wisFunctions.o
CRC_AND_BCID      = wisCheckCRCandBCID.o
PULSE_HEIGHT      = wisPulseHeight.o
DEAD_CHANNELS     = wisDeadChannels.o
BASELINE          = wisBaseline.o
CNOISE            = wisCnoise.o
CALIB             = wisCheckCalibrationData.o
IS_RUNNING        = wisIsRunning.o
ERASE_FIRST_LINE  = wisEraseFirstLine.o
ERASE_LINE        = wisEraseLines.o
INTPED            = wisIntped.o
HISTO             = wicomExistHisto.o  wisFunctions.o
SHOWDATE          = wicomShowNtupleDate.o wisFunctions.o
CALLMACRO         = wisCallMacro.o
READDIR           = wisReadCastorDir.o
RECONSTRUCTION    = wisReconstruction.o wisFunctions.o
ALL               = copyFromCastor wisCreateFile wisDeleteFile wisWgetURL wisExistFile wisIsRunning wisEraseFirstLine wisEraseLines wisReconstruction wisCallMacro wicomExistSmallNtuple wicomExistHisto wicomShowNtupleDate  wisReadCastorDir
#wisCheckCRCandBCID wisPulseHeight wisDeadChannels wisBaseline wisCnoise wisCheckCalibrationData wisIsRunning wisEraseFirstLine wisEraseLines wisReconstruction wisIntped wisCallMacro

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(ALL)

copyFromCastor: $(COPY_CASTOR)
	$(LD) $(LFLAGS) -o $@ $(COPY_CASTOR)
#	chmod ug+s $@

wisCreateFile: $(CREATE_FILE)
	$(LD) $(LFLAGS) -o $@ $(CREATE_FILE)
#	chmod ug+s $@

wisDeleteFile: $(DELETE_FILE)
	$(LD) $(LFLAGS) -o $@ $(DELETE_FILE)
#	chmod ug+s $@

wisWgetURL: $(WGET_URL)
	$(LD) $(LFLAGS) -o $@ $(WGET_URL)
#	chmod ug+s $@

wisExistFile: $(EXIST_FILE)
	$(LD) $(LFLAGS) -o $@ $(EXIST_FILE)
#	chmod ug+s $@

wicomExistSmallNtuple: $(EXIST_SMALL_NTUPLE)
	$(LD) $(LFLAGS) -o $@ $(EXIST_SMALL_NTUPLE)
wisCheckCRCandBCID: $(CRC_AND_BCID)
	$(LD) $(LFLAGS) -o $@ $(CRC_AND_BCID)
#	chmod ug+s $@

wisPulseHeight: $(PULSE_HEIGHT)
	$(LD) $(LFLAGS) -o $@ $(PULSE_HEIGHT)
#	chmod ug+s $@

wisDeadChannels: $(DEAD_CHANNELS)
	$(LD) $(LFLAGS) -o $@ $(DEAD_CHANNELS)
#	chmod ug+s $@

wisBaseline: $(BASELINE)
	$(LD) $(LFLAGS) -o $@ $(BASELINE)
#	chmod ug+s $@

wisCnoise: $(CNOISE)
	$(LD) $(LFLAGS) -o $@ $(CNOISE)
#	chmod ug+s $@

wisCheckCalibrationData: $(CALIB)
	$(LD) $(LFLAGS) -o $@ $(CALIB)
#	chmod ug+s $@

wisIsRunning: $(IS_RUNNING)
	$(LD) $(LFLAGS) -o $@ $(IS_RUNNING)
#	chmod ug+s $@

wisEraseFirstLine: $(ERASE_FIRST_LINE)
	$(LD) $(LFLAGS) -o $@ $(ERASE_FIRST_LINE)
#	chmod ug+s $@

wisEraseLines: $(ERASE_LINE)
	$(LD) $(LFLAGS) -o $@ $(ERASE_LINE)
#	chmod ug+s $@

wisReconstruction: $(RECONSTRUCTION)
	$(LD) $(LFLAGS) -o $@ $(RECONSTRUCTION)
#	chmod ug+s $@

wisIntped: $(INTPED)
	$(LD) $(LFLAGS) -o $@ $(INTPED)
#	chmod ug+s $@
wisCallMacro: $(CALLMACRO)
	$(LD) $(LFLAGS) -o $@ $(CALLMACRO) 
wicomExistHisto: $(HISTO)
	$(LD) $(LFLAGS) -o $@ $(HISTO) 
wicomShowNtupleDate: $(SHOWDATE)
	$(LD) $(LFLAGS) -o $@ $(SHOWDATE) 
wisReadCastorDir: $(READDIR)
	$(LD) $(LFLAGS) -o $@ $(READDIR) 

clean:
	rm -f *.o $(ALL)

