from pyeml import measure_function

def function(arg):
    from time import sleep
    sleep(arg)


print(measure_function(function, args=(1,), devices=("rapl1","rapl0")))