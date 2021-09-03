#OBJS = main.o myarea.o mywindow.o sensor.o Valve.o miscparts.o tank.o line.o pid.o
#OBJS = main.o Graph.o GraphWindow.o
PLOTTER_OBJS = main.o GraphWindow.o GraphManager.o MsgReceiver.o LabelParser.o

OBJS = main.o GraphWindow.o

OBJS2 = Daq.o Updater.o main2.o

SERVERMOCK = ServerMock.o

MSGRECEIVER = MsgReceiver.o

#GTKFLAG = `pkg-config gtkmm-3.0 --cflags --libs`
GTKFLAG = `pkg-config --cflags --libs gtkmm-plplot-2.0`

plotter : $(PLOTTER_OBJS)
	g++ -o plotter $(PLOTTER_OBJS) $(GTKFLAG) -ljsoncpp

graphwindow : $(OBJS)
	g++ -o graphwindow $(OBJS) $(GTKFLAG) -ljsoncpp

main.o : main.cpp
	g++ -c -g main.cpp $(GTKFLAG)

Graph.o : Graph.cpp
	g++ -c -g Graph.cpp $(GTKFLAG)

GraphWindow.o : GraphWindow.cpp
	g++ -c -g GraphWindow.cpp $(GTKFLAG)

GraphManager.o : GraphManager.cpp
	g++ -c -g GraphManager.cpp $(GTKFLAG)

MsgReceiver.o : MsgReceiver.cpp
	g++ -c MsgReceiver.cpp $(GTKFLAG)

Base64Tools.o : Base64Tools.cpp
	g++ -c Base64Tools.cpp $(GTKFLAG)

LabelParser.o : LabelParser.cpp
	g++ -c LabelParser.cpp $(GTKFLAG) -ljsoncpp

test2 : $(OBJS2)
	g++ -o test2 $(OBJS2) $(GTKFLAG) -ljsoncpp
Daq.o : Daq.cpp
	g++ -c Daq.cpp $(GTKFLAG)
Updater.o : Updater.cpp
	g++ -c Updater.cpp $(GTKFLAG)
main2.o : main2.cpp
	g++ -c main2.cpp $(GTKFLAG)

server : $(SERVERMOCK)
	g++ -o servermock $(SERVERMOCK)

ServerMock.o : ServerMock.cpp
	g++ -c ServerMock.cpp

msgreceiver : $(MSGRECEIVER)
	g++ -o msgreceiver $(MSGRECEIVER)

#myarea.o : myarea.cpp myarea.h
#	g++ -c myarea.cpp $(GTKFLAG)
#mywindow.o : mywindow.cpp mywindow.h
#	g++ -c mywindow.cpp $(GTKFLAG)
#sensor.o : sensor.cpp sensor.h
#	g++ -c sensor.cpp $(GTKFLAG)
#Valve.o : Valve.cpp Valve.h
#	g++ -c Valve.cpp $(GTKFLAG)
#miscparts.o : miscparts.cpp miscparts.h
#	g++ -c miscparts.cpp $(GTKFLAG)
#tank.o : tank.cpp tank.h
#	g++ -c tank.cpp $(GTKFLAG)
#line.o : line.cpp line.h
#	g++ -c line.cpp $(GTKFLAG)
#pid.o : pid.cpp pid.h
#	g++ -c pid.cpp $(GTKFLAG)

clean:
	rm -f core graphwindow $(OBJS)

clean2:
	rm -f core test2 $(OBJS2)

cleanServer:
	rm -f servermock $(SERVERMOCK)

cleanReceiver:
	rm -f msgreceiver $(MSGRECEIVER)

cleanPlotter:
	rm -f plotter $(PLOTTER_OBJS)