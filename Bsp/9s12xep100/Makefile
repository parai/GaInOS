#P_RUN = wine
#P_SW  =/opt/SW
#mkfile = ./mkfile/Makefile_Posix
P_RUN =
P_SW  = D:/usr/opt/SW
mkfile = ./mkfile/Makefile_Win

all:
	$(P_RUN) $(P_SW)/Prog/piper.exe $(P_SW)/Prog/maker.exe $(mkfile)
	rm -f *.o
	echo ">>> Done! <<<"
	$(P_RUN) $(P_SW)/Prog/hiwave.exe 
clean:
	rm -f *.o *.map *.abs

tbdml:
	$(P_RUN) $(P_SW)/Prog/hiwave.exe -W -Prod=TBDML.ini -instance=tbdml Project.abs -CMD="Go"

sim:
	$(P_RUN) $(P_SW)/Prog/hiwave.exe 
#-W -Prod=./Project/Full_Chip_Simulation.ini -instance=sim Project.abs

