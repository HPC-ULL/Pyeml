CC = c++
NAME = eml
# Assues you have a directory ezdraw at the top of your directory structure
# that contains the ezdraw.h and libezdraw.a files
# Libraries to load
LIBS =  
CFLAGS= $(shell python3-config --embed --includes --cflags)  -O3 -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) -L/usr/local/lib  -Wl,--unresolved-symbols=ignore-all
LDFLAGS=  $(shell python3-config --embed  --ldflags --includes) -leml -L/opt/soft/Modules/modulefiles/cuda-nvml-header -L/opt/soft/Modules/modulefiles/confuse/3.2  -lconfuse -Wl,--as-needed -lnvidia-ml -Wl,--no-as-needed

OBJS = $(NAME).o

%.o: %.cpp;
	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME): $(OBJS);
	$(CC) $(CFLAGS) -o $@.cpython-39-x86_64-linux-gnu.so $(OBJS)  $(LIBS) $(LDFLAGS)

clean:
	rm -f *.o
	rm -f *~
	rm -f $(NAME).cpython-39-x86_64-linux-gnu.so

run:
	make
	./$(NAME).cpython-39-x86_64-linux-gnu.so