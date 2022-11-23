from pyeml import measure_decorator


@measure_decorator
def function(arg):
    from time import sleep
    sleep(arg)


print(function(1))