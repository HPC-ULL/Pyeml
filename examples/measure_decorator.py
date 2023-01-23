from pyeml import measure_decorator

from pyeml.devices import nvml, rapl, dummy

from pyeml.units import cal, uj

@measure_decorator(devices = (nvml(0), rapl(0)), unit = uj)
def function(arg):
    from time import sleep
    sleep(arg)


print(function(1))