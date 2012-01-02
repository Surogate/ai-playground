# Makefile
# project : Archiver
# author : Cedric Creusot

NAME	    = server
SOURCES	    = 	./Logique/Board.cpp 					\
		./Logique/Callback_Environnement.cpp 			\
		./Logique/Coord.cpp 					\
		./Logique/DecisionTree.cpp				\
		./Logique/Entity.cpp 					\
		./Logique/Environnement.cpp 				\
		./Logique/Logger.cpp 					\
		./Logique/Sheep.cpp 					\
		./Logique/Wolf.cpp					\
		./IANetwork/Serveur/NetworkEnvironnement.cpp		\
		./IANetwork/Serveur/Package.cpp				\
		./IANetwork/Serveur/Server.cpp				\
		./IANetwork/Serveur/main.cpp		

HEADERS	    = ./Logique
INCLUDES    = -I./IANetwork/Serveur -I/usr/local/include -I/home/calimeraw/fann-2.1.0/src/include

LIBS	    = -L/usr/local/lib -L/home/calimeraw/fann-2.1.0/src/libs -lfann -lboost_thread -lboost_random -lboost_chrono

SRCS	    = $(SOURCES)

OBJS	    = $(SRCS:.cpp=.o)


CXXFLAGS    += $(INCLUDES) -I$(HEADERS)
LDXXFLAGS   += $(LIBS)

RM	= rm -rf
CXX	= g++

all: $(NAME)

$(NAME) : $(OBJS)
	$(CXX) -o $(NAME) $(OBJS) $(LDXXFLAGS)
	
clean	:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re	: fclean all

.cpp.o :
	$(CXX) -c $(CXXFLAGS)	-o $@ $<
