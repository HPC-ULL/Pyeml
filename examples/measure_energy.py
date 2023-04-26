from pyeml import  measure_energy

from pyeml.devices import nvml, rapl

from pyeml.units import cal, uj

@measure_energy(devices = ( rapl(0)), unit = uj)
def function(arg):
    from time import sleep
    sleep(arg)


print(function(1))
