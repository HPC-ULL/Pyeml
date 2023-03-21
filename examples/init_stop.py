import pyeml
from pyeml.devices import rapl
from pyeml.units import uj

from time import sleep

pyeml.start(devices = (rapl(0),rapl(1)), unit = uj)

sleep(1)

print(pyeml.stop())