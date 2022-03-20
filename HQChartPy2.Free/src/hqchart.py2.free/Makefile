#############################################################   
# Makefile for shared library.  
# 编译动态链接库  
#############################################################  
#set your own environment option  
CC = g++  
CC_FLAG = -D_NOMNG -D_FILELINE  -std=c++11 -fPIC -finput-charset=GBK
  
#set your inc and lib  
INC = -I../rapidjson -I./  -I../HQChart.Complier.Source -I../HQChart.Complier.JsonData -I ../HQChart.Complier.ToJson -I/usr/python3/include/python3.6m
LIB = -lpthread -lssl -lcrypto -L./ 
  
#make target lib and relevant obj   
PRG = HQChartPy2.so  
SOURCE := $(wildcard *.c) $(wildcard *.cc) $(wildcard *.cpp) $(wildcard ../HQChart.Complier.Source/*.cpp) $(wildcard ../HQChart.Complier.ToJson/*.cpp)
OBJS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
  
#all target  
all:$(PRG)  
  
$(PRG):$(OBJS)  
	$(CC) -shared -o $@ $(OBJS) $(LIB)  
  
.SUFFIXES: .c .o .cpp  
.cpp.o:  
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o  
  
.PRONY:clean  
clean:  
	@echo "Removing linked and compiled files......";  
	rm -f $(OBJ) $(PRG)