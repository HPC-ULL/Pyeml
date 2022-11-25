# Pyeml

Pyeml is a tool to measure the energy consumed by python functions based on the C library [EML](https://github.com/HPC-ULL/eml)

## Installation

Requirements:
python3.9
[EML](https://github.com/HPC-ULL/eml) library

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
In order to get the data from some devices like rapl it may need privileged execution or "cap_sys_rawio" capabilities.


## License

[MIT](https://choosealicense.com/licenses/mit/)