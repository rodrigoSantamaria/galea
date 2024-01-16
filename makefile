# Analgam makefile
CXX = c++ 
# tambien compila bien con g++
PROJ = Galea
# Mozart tiene un compilador antiguo que da problemas con las variables
# definidas en bloques internos de codigo. 
# Para evitar errores estupidos de compilacion hay que desactivar los warnings: -w
# Para hacer limpieza de warnings: -Wall
CXXFLAGS =
# en Mozart hay que incluir la ruta hacia las librerias de Motif: /usr/dt/include
# o bien hacer un enlace dinamico del directorio Xm (dentro del anterior
# directorio a /usr/include
# Tambien hay que incluir la ruta hacia las librerias de X11: /usr/X11R6.3/include
# o bien hacer un enlace dinamico del directorio X11 (dentro del anterior
# directorio a /usr/include
INCLUDES =
# en Mozart hay que incluir la ruta de las librerias de Motif: /usr/dt/lib
# o bien hacer un enlace dinamico del fichero libXm.so.3 a /usr/lib
# Tampoco vendria mal hacerle el enlace dinamico con sobrenombre libXm.so y libXm.so.1.2
# en jsbach hay que indicarle que las librerías de X11 se encuentran en -L/usr/X11R6/lib
LIBRARIES = -L/usr/X11R6/lib
# en Mozart hay que incluir ademas las librerias -lICE y -lSM
LDFLAGS = -lXm -lXt -lX11 
OBJECTS = Analgam.o VisCalib.o VisConti.o ajustes.o calibracion.o \
			Graficas.o espectros.o VisGraf.o math1.o trabajo.o \
			continuo.o avisos.o picos.o BSpline.o fuentes.o \
			analisis.o ficheros.o cursores.o lara/cLaraBD.o \
			VisIdent.o VisAnal.o VisActiv.o InitFile.o preferences.o strings1.o \
			GA/Node.o GA/Individuo.o GA/LinkedList.o \
			GA/GeneticAlgorithm.o GA/GeneticAlgorithm2.o
			
all: $(PROJ)
$(PROJ): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(PROJ) $(INCLUDES) $(LIBRARIES) $(LDFLAGS) $(OBJECTS)
ajustes.o: ajustes.h ajustes.C math1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c ajustes.C
analisis.o: analisis.h analisis.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c analisis.C
Analgam.o: Analgam.C constDef.h globales.h trabajoEst.h avisos.o analisis.o \
		calibracion.o continuo.o cursores.o ficheros.o \
		fuentes.o VisGraf.o math1.o picos.o trabajo.o VisAnal.o VisCalib.o \
		VisConti.o VisActiv.o preferences.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Analgam.C
avisos.o: avisos.h avisos.C globales.h constDef.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c avisos.C
BSpline.o: BSpline.h BSpline.C math1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c BSpline.C
calibracion.o: calibracion.h calibracion.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c calibracion.C
continuo.o: continuo.h continuo.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c continuo.C
cursores.o: cursores.h cursores.C globales.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c cursores.C
espectros.o: espectros.h espectros.C ajusteEsp.h ajustes.o analisis.o \
		BSpline.o calibracion.o continuo.o math1.o picos.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c espectros.C
ficheros.o: ficheros.h ficheros.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c ficheros.C
fuentes.o: fuentes.h fuentes.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c fuentes.C
Graficas.o : Graficas.h Graficas.C globales.h trabajoEst.h espectros.o math1.o \
		strings1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c Graficas.C
InitFile.o: InitFile.h InitFile.C strings1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c InitFile.C
math1.o: math1.h math1.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c math1.C
picos.o: picos.h picos.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c picos.C
preferences.o: preferences.h preferences.C InitFile.o strings1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c preferences.C
strings1.o: strings1.h strings1.C
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c strings1.C
trabajo.o: trabajo.h trabajo.C constDef.h trabajoEst.h calibracion.o \
		fuentes.o VisGraf.h VisAnal.o VisCalib.h VisConti.o VisActiv.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c trabajo.C
VisActiv.o: VisActiv.h VisActiv.C globales.h constDef.h trabajoEst.h analisis.o \
		avisos.o cursores.o fuentes.o VisGraf.h  picos.o VisCalib.h VisAnal.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c VisActiv.C
VisAnal.o: VisAnal.h VisAnal.C globales.h constDef.h trabajoEst.h analisis.o \
		avisos.o cursores.o fuentes.o VisGraf.h  picos.o VisCalib.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c VisAnal.C
VisCalib.o: VisCalib.h VisCalib.C constDef.h globales.h trabajoEst.h avisos.o \
		calibracion.o cursores.o fuentes.o VisGraf.h math1.o picos.o \
		trabajo.C trabajo.h VisAnal.h VisAnal.C VisIdent.o HelpIcon.xbm \
		Ecuation.xbm
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c VisCalib.C
VisConti.o: VisConti.h VisConti.C constDef.h globales.h trabajoEst.h \
		avisos.o BSpline.o calibracion.o continuo.o cursores.o fuentes.o \
		VisGraf.h trabajo.C trabajo.h VisAnal.o VisCalib.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c VisConti.C
VisGraf.o: VisGraf.h VisGraf.C constDef.h globales.h trabajoEst.h \
		espectros.o fuentes.o math1.o preferences.o VisCalib.h VisConti.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c VisGraf.C
VisIdent.o: VisIdent.h VisIdent.C globales.h math1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c VisIdent.C


#lara/cLaraBD.o: lara/cLaraBD.H lara/cLaraBD.C lara/math1.o lara/ficheros.o
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -o lara/cLaraBD.o -c lara/cLaraBD.C


GA/Individuo.o : GA/Individuo.h GA/Individuo.cc GA/Individuo.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o GA/Individuo.o -c GA/Individuo.cc
GA/Node.o : GA/Node.h GA/Node.cc GA/Node.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o GA/Node.o -c GA/Node.cc
GA/LinkedList.o : GA/LinkedList.h GA/LinkedList.cc GA/Node.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o GA/LinkedList.o -c GA/LinkedList.cc

GA/GeneticAlgorithm.o : GA/GeneticAlgorithm.h GA/GeneticAlgorithm.cc GA/LinkedList.o \
						GA/Individuo.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o GA/GeneticAlgorithm.o -c GA/GeneticAlgorithm.cc
GA/GeneticAlgorithm2.o : GA/GeneticAlgorithm2.h GA/GeneticAlgorithm2.cc GA/GeneticAlgorithm.o \
						GA/LinkedList.o GA/Individuo.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o GA/GeneticAlgorithm2.o -c GA/GeneticAlgorithm2.cc

cleanAll:
	-rm -f *.o core $(PROJ) *.third *.3log
	-rm -f lara/*.o core
clean:
	-rm -f *.o core
	-rm -f lara/*.o core
editAll:
	-nedit *.C *.h *.txt lara/cLaraBD.C lara/cLaraBD.H
editVis:
	-nedit VisCalib.h VisCalib.C VisAnal.h VisAnal.C VisConti.h VisConti.C VisActiv.h VisActiv.C \
		calibracion.C calibracion.h analisis.h analisis.C continuo.h continuo.C \
		picos.h picos.C
editLara:
	-nedit lara/cLaraBD.C lara/cLaraBD.H
editGraf:
	-nedit VisGraf.h VisGraf.C Graficas.C Graficas.h \
		cursores.C cursores.h fuentes.C fuentes.h
editKer:
	-nedit espectros.h espectros.C
editIden:
	-nedit VisIdent.C VisIdent.h
editIni:
	-nedit InitFile.h InitFile.C preferences.h preferences.C
backup:
	-rm -f *.o core $(PROJ) *.third *.3log
	-rm -f lara/*.o core
	-cp -a * ../../seguridad2
third:
	third $(PROJ)
