from pyeml import measure_function

def function(arg):
    from time import sleep
    sleep(arg)
    return 1


print(measure_function(function, args=(1,)))