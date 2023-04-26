# Pyeml

Pyeml is a tool to measure the energy consumed by python functions based on the C library [EML](https://github.com/HPC-ULL/eml)

## Installation

Requirements:
* python3.9

* [EML](https://github.com/HPC-ULL/eml) library


Use the package manager [pip](https://pip.pypa.io/en/stable/) to install Pyeml.

```bash
pip install git+https://github.com/HPC-ULL/Pyeml
```

## Usage

```python
from pyeml import  measure_energy

from pyeml.devices import nvml, rapl

from pyeml.units import cal, uj

@measure_energy(devices = ( rapl(0)), unit = uj)
def function(arg):
    from time import sleep
    sleep(arg)


print(function(1))
#(None, {'rapl0': {'power': 18472015.380859375, 'elapsed': 1.0, 'consumed': 18472015.380859375}}, 1.001293775625527) 


#    Returns a tuple of three elements:
#
#    1: Decorated function output
#
#    2: Energy measurements. Including average power, elapsed time of each 
#    device and energy consumed.
#
#    3: Elapsed time of the full execution

```



## Suported devices
Pyeml supports all of the devices supported by [EML](https://github.com/HPC-ULL/eml).

From [EML](https://github.com/HPC-ULL/eml) [docs](https://hpc-ull.github.io/eml/):

>The following devices are currently supported:
>
>* Dummy devices for testing purposes
>* Intel CPUs starting from Sandy Bridge (through the Running Average Power Limit interface)
>* Recent Nvidia Tesla and Quadro GPUs (through NVIDIA Management Library)
>* Intel Xeon Phi MICs (through the Intel Manycore Platform Software Stack (3.x+), from the host device)
>* Schleifenbauer PDUs (through the Schleifenbauer socket API)
>* Odroid-XU3 integrated sensors (through linux sysfs)
>* Poznań Supercomputing and Networking Center Labee XML >* Interface (through its REST API)
>* Beta support for PMlib
>
>EML automatically discovers necessary libraries and available devices at runtime.

In order to get the data from some devices like rapl it may need privileged execution or "cap_sys_rawio" capabilities.


## License

[MIT](https://choosealicense.com/licenses/mit/)