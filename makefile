#
#MakeFile
#
#Brett Binnersley, V00776751
#Csc 360, Assignment #3
#


all: makeDiskInfo makeDiskList makeDiskGet makeDiskPut

clean:
	cleanItems


#Make the 4 programs that are required
makeDiskInfo:
	gcc diskinfo.c shared.c -o diskinfo

makeDiskList:
	gcc disklist.c shared.c -o disklist

makeDiskGet:
	gcc diskget.c shared.c -o diskget

makeDiskPut:
	gcc diskput.c shared.c -o diskput

#Remove binary files left over
cleanItems:
	-rm -rf *.o *.exe