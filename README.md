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
from pyeml import measure_decorator

@measure_decorator
def function(arg):
    from time import sleep
    sleep(arg)

print(function(1))
#(None, {'rapl1': 18.844284057617188, 'rapl0': 19.9859619140625, 'nvml0': 21.09})

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
>
>EML automatically discovers necessary libraries and available devices at runtime.

In order to get the data from some devices like rapl it may need privileged execution or "cap_sys_rawio" capabilities.


## License

[MIT](https://choosealicense.com/licenses/mit/)