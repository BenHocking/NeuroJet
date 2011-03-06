########################################
###       Makefile for NeuroJet      ###
###    Ben Hocking, 2003 March 21    ###
########################################

# Executable #
#ARCH = -march=athlon -O3 -msse -finline-functions -ffinite-math-only -fconserve-space -finline-limit=1500 
#ARCH = -march=pentium3 -O3 -msse -finline-functions -ffinite-math-only -fconserve-space -finline-limit=1500 
#ARCH = -m64 -O3 -msse -fopenmp -finline-functions -ffinite-math-only -fconserve-space -finline-limit=1500 
RM = rm -f
# debug architectures
ARCH = -g -fPIC -DCHECK_BOUNDS
NAMEA = NeuroJet-aspen
NAMEB = NeuroJet-birch
NAMEC = NeuroJet-sunfire
NAMEPSC = NeuroJet-psc
NAME1 = NeuroJet
NAME2 = NeuroJet-parallel
INSTPATH = /usr/local/bin/
PGCC = pgCC -Wall -DRNG_BUCKET -DMULTIPROC
#CCs = mpiCC -Wall -O3 -march=i686 -DRNG_BUCKET -DMULTIPROC
CCs = mpicxx -Wall -O1 -march=k8 -DRNG_BUCKET -DMULTIPROC -static -DMPICH_IGNORE_CXX_SEEK
#CCPSC = c++ -O3 -lmpi -lelan -lm -DRNG_BUCKET -DMULTIPROC
CCPSC = c++ -O1 -lmpi -lelan -lm -DRNG_BUCKET -DMULTIPROC
CCBlue = mpicxx -O2 -DRNG_BUCKET -DMULTIPROC -DNONUMERICLIMITS -DNO_IOS_OPENMODE
CC2 = mpicc -cc=g++ -O2 -DRNG_BUCKET -DMULTIPROC -DNONUMERICLIMITS
#GCPP = g++ -Wall -Winline -O3 -finline-functions -minline-all-stringops -fconserve-space -finline-limit=1500 $(ARCH) -ffast-math
#GCPP = g++ -Wall -Winline -O3 -finline-functions -minline-all-stringops -fconserve-space -finline-limit=1500 $(ARCH)
#GCPP = g++ -Wall -Winline -O2 -fconserve-space $(ARCH)
#GCPP = g++ -Wall -Os -finline-functions -minline-all-stringops -fconserve-space -finline-limit=1500 $(ARCH)
GCPP = g++ -Wall $(ARCH)
#GCPP = g++ -g

BINDLISTHPPINC = BindList.hpp
DATATYPESHPPINC = DataTypes.hpp
OUTPUTHPPINC = Output.hpp

ARGFUNCTSHPPINC = ArgFuncts.hpp $(DATATYPESHPPINC) $(OUTPUTHPPINC)
FILTERHPPINC = Filter.hpp $(DATATYPESHPPING)

INTERNEURONHPPINC = Interneuron.hpp $(ARGFUNCTSHPPINC) $(DATATYPESHPPINC)
NOISEHPPINC = Noise.hpp $(ARGFUNCTSHPPINC)
SYMBOLSHPPINC = Symbols.hpp $(ARGFUNCTSHPPINC)
SYNAPSETYPEHPPINC = SynapseType.hpp $(FILTERHPPINC)
SYSTEMVARHPPINC = SystemVar.hpp $(ARGFUNCTSHPPINC) $(BINDLISTHPPINC)

CALCHPPINC = Calc.hpp $(NOISEHPPINC) $(SYSTEMVARHPPINC)
NEURONTYPEHPPINC = NeuronType.hpp $(DATATYPESHPPINC) $(FILTERHPPINC) $(SYNAPSETYPEHPPINC) $(SYSTEMVARHPPINC)
PARALLELRANDHPPINC = ParallelRand.hpp $(NOISEHPPINC)
SYNAPSEHPPINC = Synapse.hpp $(ARGFUNCTSHPPINC) $(NOISEHPPINC) $(SYNAPSETYPEHPPINC)
WEIGHTANALYSISHPPINC = WeightAnalysis.hpp  $(SYSTEMVARHPPINC)

PARALLELHPPINC = Parallel.hpp $(PARALLELRANDHPPINC) $(SYNAPSEHPPINC)
PARSERHPPINC = Parser.hpp $(CALCHPPINC) $(SYSTEMVARHPPINC)
POPULATIONHPPINC = Population.hpp $(DATATYPESHPPINC) $(INTERNEURONHPPINC) $(NEURONTYPEHPPINC)
PROGRAMHPPINC = Program.hpp $(ARGFUNCTSHPPINC) $(BINDLISTHPPINC) $(FILTERHPPINC) $(INTERNEURONHPPINC) $(OUTPUTHPPINC) $(NOISEHPPINC) $(PARALLELRANDHPPINC)

STATEHPPINC = State.hpp $(DATATYPESHPPINC) $(POPULATIONHPPINC) $(SYNAPSEHPPINC)
USERHPPINC = User.hpp $(WEIGHTANALYSISHPPINC)

NEUROJETHPPINC = NeuroJet.hpp $(NEURONTYPEHPPINC) $(PARALLELHPPINC) $(USERHPPINC) $(SYNAPSEHPPINC)

OBJ1 = NeuroJet.o ArgFuncts.o Calc.o Interneuron.o NeuronType.o Noise.o Output.o Parser.o Population.o Program.o rdtsc.o Synapse.o SynapseType.o SystemVar.o
OBJBUCKA = NeuroJet-aspen.o ArgFuncts-aspen.o Calc-aspen.o Interneuron-aspen.o NeuronType-aspen.o Noise-aspen.o Output-aspen.o Parallel-aspen.o ParallelRand-aspen.o Parser-aspen.o Population-aspen.o Program-aspen.o rdtsc-aspen.o Synapse-aspen.o SynapseType-aspen.o SystemVar-aspen.o
OBJBUCKB = NeuroJet-birch.o ArgFuncts-birch.o Calc-birch.o Interneuron-birch.o NeuronType-birch.o Noise-birch.o Output-birch.o Parallel-birch.o ParallelRand-birch.o Parser-birch.o Population-birch.o Program-birch.o rdtsc-birch.o Synapse-birch.o SynapseType-birch.o SystemVar-birch.o
OBJBUCKC = NeuroJet-sunfire.o ArgFuncts-sunfire.o Calc-sunfire.o Interneuron-sunfire.o NeuronType-sunfire.o Noise-sunfire.o Output-sunfire.o Parallel-sunfire.o ParallelRand-sunfire.o Parser-sunfire.o Population-sunfire.o Program-sunfire.o rdtsc-sunfire.o Synapse-sunfire.o SynapseType-sunfire.o SystemVar-sunfire.o
OBJBUCKPSC = NeuroJet-psc.o ArgFuncts-psc.o Calc-psc.o Interneuron-psc.o NeuronType-psc.o Noise-psc.o Output-psc.o Parallel-psc.o ParallelRand-psc.o Parser-psc.o Population-psc.o Program-psc.o Synapse-psc.o SynapseType-psc.o SystemVar-psc.o
OBJBUCK2 = NeuroJet-parallel.o ArgFuncts-parallel.o Calc-parallel.o Interneuron-parallel.o NeuronType-parallel.o Noise-parallel.o Output-parallel.o Parallel-parallel.o ParallelRand-parallel.o Parser-parallel.o Population-parallel.o Program-parallel.o rdtsc-parallel.o Synapse-parallel.o SynapseType-parallel.o SystemVar-parallel.o

#	g++ -Wall $(ARCH) -fdefer-pop -fmerge-constants -fthread-jumps -floop-optimize -fcrossjumping -fif-conversion -fif-conversion2 -fdelayed-branch -fguess-branch-probability -fcprop-registers -fomit-frame-pointer NeuroJet.cpp $(OBJ1) -o $(NAME1) -lm

default: $(NAME1) NeuroJet-depends.ps

checkin: updateInfo test
	./NJchkIn

updateInfo:
	./updateLastUpdate; sleep 0.5

test: $(NAME1)
	/home/levylab/test_scripts/testNJ `perl -e "use Cwd; print getcwd . \"/NeuroJet\";"`
#	./launchParallelTest

pTest-sunfire: $(NAMEC)
	cd test_scripts;./testSunfire ../NeuroJet-sunfire;cd ..


pTest-birch: $(NAMEB)
	cd test_scripts;./testBirch ../NeuroJet-birch;cd ..

pTest-dogwood: $(NAMEB)
	cd test_scripts;./testDogwood ../NeuroJet-birch;cd ..

debugversions:
	g++ -Wall $(ARCH) -c Noise.cpp -o Noise.o
	g++ -Wall $(ARCH) -c rdtsc.s -o rdtsc.o
	g++ -Wall $(ARCH) NeuroJet.cpp $(OBJ1) -o $(NAME1) -lm
	cp -p NeuroJet ~/TraceSeries/NonOpt2/NeuroJet.nonOpt
	$(GCPP) $(ARCH) -c Noise.cpp -o Noise.o
	$(GCPP) $(ARCH) -c rdtsc.s -o rdtsc.o
	$(GCPP) NeuroJet.cpp $(OBJ1) -o $(NAME1) -lm
	cp -p NeuroJet ~/TraceSeries/Opt3/NeuroJet.opt3

install: $(NAME1)
	su -c"install -p $(NAME1) $(INSTPATH)"

clean:
	$(RM) *.o $(NAMEA) $(NAMEB) $(NAMEC) $(NAMEPSC) $(NAME1)

$(NAME1): $(OBJ1) Makefile
	$(GCPP) $(OBJ1) -o $(NAME1) -lm

$(NAMEA): $(OBJBUCKA) Makefile
	$(CCBlue) $(OBJBUCKA) -o $(NAMEA)

$(NAMEB): $(OBJBUCKB) Makefile
	$(CCBlue) $(OBJBUCKB) -o $(NAMEB)

$(NAMEC): $(OBJBUCKC) Makefile
	$(CCs) $(OBJBUCKC) -o $(NAMEC)

$(NAMEPSC): $(OBJBUCKPSC) Makefile
	$(CCPSC) $(OBJBUCKPSC) -o $(NAMEPSC)

$(NAME2): $(OBJBUCK2) Makefile
	$(CC2) $(OBJBUCK2) -o $(NAME2)

NEURODEPEND = NeuroJet.cpp $(CALCHPPINC) $(NEUROJETHPPINC) $(POPULATIONHPPINC) $(PARSERHPPINC) $(STATEHPPINC) Makefile
NeuroJet.o: $(NEURODEPEND)
	$(GCPP) -c NeuroJet.cpp -o NeuroJet.o

NeuroJet-aspen.o: $(NEURODEPEND)
	$(CCBlue) -c NeuroJet.cpp -o NeuroJet-aspen.o

NeuroJet-birch.o: $(NEURODEPEND)
	$(CCBlue) -c NeuroJet.cpp -o NeuroJet-birch.o

NeuroJet-psc.o: $(NEURODEPEND)
	$(CCPSC) -c NeuroJet.cpp -o NeuroJet-psc.o

NeuroJet-sunfire.o: $(NEURODEPEND)
	$(CCs) -c NeuroJet.cpp -o NeuroJet-sunfire.o

NeuroJet-parallel.o: $(NEURODEPEND)
	$(CC2) -c NeuroJet.cpp -o NeuroJet-parallel.o

ARGDEPEND = ArgFuncts.cpp $(ARGFUNCTSHPPINC) Makefile
ArgFuncts.o: $(ARGDEPEND)
	$(GCPP) -c ArgFuncts.cpp -o ArgFuncts.o

ArgFuncts-aspen.o: $(ARGDEPEND)
	$(CCBlue) -c ArgFuncts.cpp -o ArgFuncts-aspen.o

ArgFuncts-birch.o: $(ARGDEPEND)
	$(CCBlue) -c ArgFuncts.cpp -o ArgFuncts-birch.o

ArgFuncts-psc.o: $(ARGDEPEND)
	$(CCPSC) -c ArgFuncts.cpp -o ArgFuncts-psc.o

ArgFuncts-sunfire.o: $(ARGDEPEND)
	$(CCs) -c ArgFuncts.cpp -o ArgFuncts-sunfire.o

ArgFuncts-parallel.o: $(ARGDEPEND)
	$(CC2) -c ArgFuncts.cpp -o ArgFuncts-parallel.o

CALCDEPEND = Calc.cpp $(CALCHPPINC) Makefile
Calc.o: $(CALCDEPEND)
	$(GCPP) -c Calc.cpp -o Calc.o

Calc-aspen.o: $(CALCDEPEND)
	$(CCBlue) -c Calc.cpp -o Calc-aspen.o

Calc-birch.o: $(CALCDEPEND)
	$(CCBlue) -c Calc.cpp -o Calc-birch.o

Calc-psc.o: $(CALCDEPEND)
	$(CCPSC) -c Calc.cpp -o Calc-psc.o

Calc-sunfire.o: $(CALCDEPEND)
	$(CCs) -c Calc.cpp -o Calc-sunfire.o

Calc-parallel.o: $(CALCDEPEND)
	$(CC2) -c Calc.cpp -o Calc-parallel.o

INTERNRNDEPEND = Interneuron.cpp $(INTERNEURONHPPINC) $(FILTERHPPINC) $(OUTPUTHPPINC) $(PARSERHPPINC) Makefile
Interneuron.o: $(INTERNRNDEPEND)
	$(GCPP) -c Interneuron.cpp -o Interneuron.o

Interneuron-aspen.o: $(INTERNRNDEPEND)
	$(CCBlue) -c Interneuron.cpp -o Interneuron-aspen.o

Interneuron-birch.o: $(INTERNRNDEPEND)
	$(CCBlue) -c Interneuron.cpp -o Interneuron-birch.o

Interneuron-psc.o: $(INTERNRNDEPEND)
	$(CCPSC) -c Interneuron.cpp -o Interneuron-psc.o

Interneuron-sunfire.o: $(INTERNRNDEPEND)
	$(CCs) -c Interneuron.cpp -o Interneuron-sunfire.o

Interneuron-parallel.o: $(INTERNRNDEPEND)
	$(CC2) -c Interneuron.cpp -o Interneuron-parallel.o

NEURONTYPEDEPEND = NeuronType.cpp $(CALCHPPINC) $(NEURONTYPEHPPINC) $(OUTPUTHPPINC) $(SYSTEMVARHPPINC) Makefile
NeuronType.o: $(NEURONTYPEDEPEND)
	$(GCPP) -c NeuronType.cpp -o NeuronType.o

NeuronType-aspen.o: $(NEURONTYPEDEPEND)
	$(CCBlue) -c NeuronType.cpp -o NeuronType-aspen.o

NeuronType-birch.o: $(NEURONTYPEDEPEND)
	$(CCBlue) -c NeuronType.cpp -o NeuronType-birch.o

NeuronType-psc.o: $(NEURONTYPEDEPEND)
	$(CCPSC) -c NeuronType.cpp -o NeuronType-psc.o

NeuronType-sunfire.o: $(NEURONTYPEDEPEND)
	$(CCs) -c NeuronType.cpp -o NeuronType-sunfire.o

NeuronType-parallel.o: $(NEURONTYPEDEPEND)
	$(CC2) -c NeuronType.cpp -o NeuronType-parallel.o

NOISEDEPEND = Noise.cpp $(NOISEHPPINC) Makefile
Noise.o: $(NOISEDEPEND)
	$(GCPP) -c Noise.cpp -o Noise.o

Noise-aspen.o: $(NOISEDEPEND)
	$(CCBlue) -c Noise.cpp -o Noise-aspen.o

Noise-birch.o: $(NOISEDEPEND)
	$(CCBlue) -c Noise.cpp -o Noise-birch.o

Noise-psc.o: $(NOISEDEPEND)
	$(CCPSC) -c Noise.cpp -o Noise-psc.o

Noise-sunfire.o: $(NOISEDEPEND)
	$(CCs) -c Noise.cpp -o Noise-sunfire.o

Noise-parallel.o: $(NOISEDEPEND)
	$(CC2) -c Noise.cpp -o Noise-parallel.o

OUTPUTDEPEND = Output.cpp $(OUTPUTHPPINC) $(DATATYPESHPPINC) Makefile
Output.o: $(OUTPUTDEPEND)
	$(GCPP) -c Output.cpp -o Output.o

Output-aspen.o: $(OUTPUTDEPEND)
	$(CCBlue) -c Output.cpp -o Output-aspen.o

Output-birch.o: $(OUTPUTDEPEND)
	$(CCBlue) -c Output.cpp -o Output-birch.o

Output-psc.o: $(OUTPUTDEPEND)
	$(CCPSC) -c Output.cpp -o Output-psc.o

Output-sunfire.o: $(OUTPUTDEPEND)
	$(CCs) -c Output.cpp -o Output-sunfire.o

Output-parallel.o: $(OUTPUTDEPEND)
	$(CC2) -c Output.cpp -o Output-parallel.o

PARALLELDEPEND = Parallel.cpp $(PARALLELHPPINC) Makefile
Parallel-aspen.o: $(PARALLELDEPEND)
	$(CCBlue) -c Parallel.cpp -o Parallel-aspen.o

Parallel-birch.o: $(PARALLELDEPEND)
	$(CCBlue) -c Parallel.cpp -o Parallel-birch.o

Parallel-psc.o: $(PARALLELDEPEND)
	$(CCPSC) -c Parallel.cpp -o Parallel-psc.o

Parallel-sunfire.o: $(PARALLELDEPEND)
	$(CCs) -c Parallel.cpp -o Parallel-sunfire.o

Parallel-parallel.o: $(PARALLELDEPEND)
	$(CC2) -c Parallel.cpp -o Parallel-parallel.o

PRDEPEND = ParallelRand.cpp $(PARALLELRANDHPPINC) Makefile
ParallelRand-aspen.o: $(PRDEPEND)
	$(CCBlue) -c ParallelRand.cpp -o ParallelRand-aspen.o

ParallelRand-birch.o: $(PRDEPEND)
	$(CCBlue) -c ParallelRand.cpp -o ParallelRand-birch.o

ParallelRand-psc.o: $(PRDEPEND)
	$(CCPSC) -c ParallelRand.cpp -o ParallelRand-psc.o

ParallelRand-sunfire.o: $(PRDEPEND)
	$(CCs) -c ParallelRand.cpp -o ParallelRand-sunfire.o

ParallelRand-parallel.o: $(PRDEPEND)
	$(CC2) -c ParallelRand.cpp -o ParallelRand-parallel.o

PARSERDEPEND = Parser.cpp $(PARSERHPPINC) $(CALCHPPINC) $(OUTPUTHPPINC) $(PARALLELHPPINC) $(SYMBOLSHPPINC) Makefile
Parser.o: $(PARSERDEPEND)
	$(GCPP) -c Parser.cpp -o Parser.o

Parser-aspen.o: $(PARSERDEPEND)
	$(CCBlue) -c Parser.cpp -o Parser-aspen.o

Parser-birch.o: $(PARSERDEPEND)
	$(CCBlue) -c Parser.cpp -o Parser-birch.o

Parser-psc.o: $(PARSERDEPEND)
	$(CCPSC) -c Parser.cpp -o Parser-psc.o

Parser-sunfire.o: $(PARSERDEPEND)
	$(CCs) -c Parser.cpp -o Parser-sunfire.o

Parser-parallel.o: $(PARSERDEPEND)
	$(CC2) -c Parser.cpp -o Parser-parallel.o

POPULATIONDEPEND = Population.cpp $(POPULATIONHPPINC) $(SYSTEMVARHPPINC) Makefile
Population.o: $(POPULATIONDEPEND)
	$(GCPP) -c Population.cpp -o Population.o

Population-aspen.o: $(POPULATIONDEPEND)
	$(CCBlue) -c Population.cpp -o Population-aspen.o

Population-birch.o: $(POPULATIONDEPEND)
	$(CCBlue) -c Population.cpp -o Population-birch.o

Population-psc.o: $(POPULATIONDEPEND)
	$(CCPSC) -c Population.cpp -o Population-psc.o

Population-sunfire.o: $(POPULATIONDEPEND)
	$(CCs) -c Population.cpp -o Population-sunfire.o

Population-parallel.o: $(POPULATIONDEPEND)
	$(CC2) -c Population.cpp -o Population-parallel.o

PGMDEPEND = Program.cpp $(CALCHPPINC) $(PROGRAMHPPINC) $(ALGORITHMHPPINC) $(NEURONTYPEHPPINC) $(PARALLELHPPINC) $(SYMBOLSHPPINC) $(SYNAPSEHPPINC) $(SYSTEMVARHPPINC) Makefile
Program.o: $(PGMDEPEND)
	$(GCPP) -c Program.cpp -o Program.o

Program-aspen.o: $(PGMDEPEND)
	$(CCBlue) -c Program.cpp -o Program-aspen.o

Program-birch.o: $(PGMDEPEND)
	$(CCBlue) -c Program.cpp -o Program-birch.o

Program-psc.o: $(PGMDEPEND)
	$(CCPSC) -c Program.cpp -o Program-psc.o

Program-sunfire.o: $(PGMDEPEND)
	$(CCs) -c Program.cpp -o Program-sunfire.o

Program-parallel.o: $(PGMDEPEND)
	$(CC2) -c Program.cpp -o Program-parallel.o

SYNAPSEDEPEND = Synapse.cpp $(SYNAPSEHPPINC) Makefile
Synapse.o: $(SYNAPSEDEPEND)
	$(GCPP) -c Synapse.cpp -o Synapse.o

Synapse-aspen.o: $(SYNAPSEDEPEND)
	$(CCBlue) -c Synapse.cpp -o Synapse-aspen.o

Synapse-birch.o: $(SYNAPSEDEPEND)
	$(CCBlue) -c Synapse.cpp -o Synapse-birch.o

Synapse-psc.o: $(SYNAPSEDEPEND)
	$(CCPSC) -c Synapse.cpp -o Synapse-psc.o

Synapse-sunfire.o: $(SYNAPSEDEPEND)
	$(CCs) -c Synapse.cpp -o Synapse-sunfire.o

Synapse-parallel.o: $(SYNAPSEDEPEND)
	$(CC2) -c Synapse.cpp -o Synapse-parallel.o

SYNAPSETYPEDEPEND = SynapseType.cpp $(SYNAPSETYPEHPPINC) $(OUTPUTHPPINC) Makefile
SynapseType.o: $(SYNAPSETYPEDEPEND)
	$(GCPP) -c SynapseType.cpp -o SynapseType.o

SynapseType-aspen.o: $(SYNAPSETYPEDEPEND)
	$(CCBlue) -c SynapseType.cpp -o SynapseType-aspen.o

SynapseType-birch.o: $(SYNAPSETYPEDEPEND)
	$(CCBlue) -c SynapseType.cpp -o SynapseType-birch.o

SynapseType-psc.o: $(SYNAPSETYPEDEPEND)
	$(CCPSC) -c SynapseType.cpp -o SynapseType-psc.o

SynapseType-sunfire.o: $(SYNAPSETYPEDEPEND)
	$(CCs) -c SynapseType.cpp -o SynapseType-sunfire.o

SynapseType-parallel.o: $(SYNAPSETYPEDEPEND)
	$(CC2) -c SynapseType.cpp -o SynapseType-parallel.o

rdtsc.o: rdtsc.s
	$(GCPP) -c rdtsc.s -o rdtsc.o

rdtsc-aspen.o: rdtsc.s
	$(CCBlue) -c rdtsc.s -o rdtsc-aspen.o

rdtsc-birch.o: rdtsc.s
	$(CCBlue) -c rdtsc.s -o rdtsc-birch.o

rdtsc-psc.o: rdtsc.s
	$(GCPP) -c rdtsc.s -o rdtsc-psc.o

rdtsc-sunfire.o: rdtsc.s
	$(CCs) -c rdtsc.s -o rdtsc-sunfire.o

rdtsc-parallel.o: rdtsc.s
	$(CC2) -c rdtsc.s -o rdtsc-parallel.o

SYSVARDEPEND = SystemVar.cpp $(OUTPUTHPPINC) $(SYSTEMVARHPPINC) Makefile
SystemVar.o: $(SYSVARDEPEND)
	$(GCPP) -c SystemVar.cpp -o SystemVar.o

SystemVar-aspen.o: $(SYSVARDEPEND)
	$(CCBlue) -c SystemVar.cpp -o SystemVar-aspen.o

SystemVar-birch.o: $(SYSVARDEPEND)
	$(CCBlue) -c SystemVar.cpp -o SystemVar-birch.o

SystemVar-psc.o: $(SYSVARDEPEND)
	$(CCPSC) -c SystemVar.cpp -o SystemVar-psc.o

SystemVar-sunfire.o: $(SYSVARDEPEND)
	$(CCs) -c SystemVar.cpp -o SystemVar-sunfire.o

SystemVar-parallel.o: $(SYSVARDEPEND)
	$(CC2) -c SystemVar.cpp -o SystemVar-parallel.o

NeuroJet-depends.ps: NeuroJet-depends.dvi
	dvips -Ppdf -t landscape NeuroJet-depends.dvi

NeuroJet-depends.dvi: NeuroJet-depends.tex
	latex NeuroJet-depends
