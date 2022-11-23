from pyeml import measure_function

def function(arg):
    from time import sleep
    sleep(1)
    print(arg)

print(measure_function(function, args=("Hello world",)))