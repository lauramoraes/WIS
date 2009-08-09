CC	= gcc
CXX	= g++
LD	= gcc

CFLAGS	= -Wall -I/usr/include/shift -I/usr/include/mysql
LFLAGS	= -Wall -L/afs/cern.ch/user/t/tilecom/public/www/WIS/ -lshift -lz -L/usr/lib/mysql -lmysqlclient -L/usr/local/lib

#CFLAGS = -Wall -I/usr/include/mysql
#LFLAGS = -Wall -L/usr/lib/mysql -lmysqlclient -L/usr/local/lib

#ROOTSYS = /afs/cern.ch/sw/lcg/external/root/5.10.00/slc3_gcc323/root

LIBS =	-L$(ROOTSYS)/lib  -lNew  -lCint  \
        -lGraf -lGraf3d -lHist -lHtml -lMatrix  -lMinuit  \
        -lPostscript -lTree -lCore -lGX11 \
        -lGpad -lGui  \
        -L/usr/X11R6/lib -lm -ldl
OPTCOMP =  -g
#OPTCOMP =  -O
CXXFLAGS = --exceptions  -I$(ROOTSYS)/include -L$(ROOTSYS)/lib 

CRC_DMU_BCID  = TWISMacroResult.o checkCRC_DMU_BCID.o wisMacroFunctions.o
DEAD_CHANNELS = wis2Tilecomm.o TWISMacroResult.o checkDeadChannels.o wisMacroFunctions.o
PULSE_HEIGHT  = wis2Tilecomm.o TWISMacroResult.o checkPulseHeight.o wisMacroFunctions.o
BASELINE      = wis2Tilecomm.o TWISMacroResult.o checkBaseline.o wisMacroFunctions.o
CNOISE        = wis2Tilecomm.o TWISMacroResult.o checkCnoise.o wisMacroFunctions.o
CCALIBRATION  = wis2Tilecomm.o TWISMacroResult.o monocis.o pedestal.o checkCalibrationData.o 
INTPED        = wisIntegrator2Tilecomm.o wisMacroFunctions.o TWISMacroResult.o intped.o  
ALL  = checkCRC_DMU_BCID checkDeadChannels checkPulseHeight checkBaseline checkCnoise checkCalibrationData intped

.c.o:
	$(CC) $(OPTCOMP) $(CXXFLAGS) $(CFLAGS) -c $<

.cpp.o:
	$(CXX) $(OPTCOMP) $(CXXFLAGS) $(CFLAGS) -c $<

all: $(ALL)

checkCRC_DMU_BCID: $(CRC_DMU_BCID)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(CRC_DMU_BCID) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@

checkDeadChannels: $(DEAD_CHANNELS)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(DEAD_CHANNELS) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@

checkPulseHeight: $(PULSE_HEIGHT)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(PULSE_HEIGHT) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@

checkBaseline: $(BASELINE)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(BASELINE) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@

checkCnoise: $(CNOISE)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(CNOISE) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@

checkCalibrationData: $(CCALIBRATION)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(CCALIBRATION) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@
intped: $(INTPED)
	$(LD) $(OPTCOMP) $(CXXFLAGS) -o $@ $(OPTCOMP) $(INTPED) $(LIBS) $(LFLAGS)
	chmod 755 $@
#	chmod ug+s $@
clean:
	rm -f *.o
	rm -f *~;
	rm -f $(ALL)
