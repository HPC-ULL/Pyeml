# Pyeml

Pyeml is a tool to measure the energy consumed by python functions based on the C library [EML](https://github.com/HPC-ULL/eml)

## Installation

Use the package manager [pip](https://pip.pypa.io/en/stable/) to install Pyeml.

```bash
pip install git+https://github.com/dsuarezl/Pyeml
```

## Usage

```python
from pyeml import measure_decorator

@measure_decorator
def function(arg):
    from time import sleep
    sleep(arg)

print(function(1))

```


## License

[MIT](https://choosealicense.com/licenses/mit/)