
import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__),"lib"))
from eml import measureCode, measureCodeInDevices, getDevices, shutdown

from inspect import currentframe

import __main__ as main_module

from typing import Callable, Iterable, Dict, Union

import atexit
atexit.register(shutdown)

def get_devices():
    return  getDevices()

def measure_code(code : str , devices : Union[Iterable,None] = None, unit : Union[Dict, None] = None):

    if(unit):
        conversion_factor = unit["conv"]
    else:
        conversion_factor = 1

    if(devices is not None):
        return measureCodeInDevices(code, devices=set(devices), conversion_factor = conversion_factor)
    else:
        return measureCode(code, conversion_factor = conversion_factor)


def measure_function(function : Callable, devices : Union[Iterable,None] = None, args : Iterable = (), kwargs : Dict = {}, scope : any = None,  unit : Union[Dict, None] = None):

    scope = scope if scope is not None else currentframe().f_back.f_globals

    main_module.__dict__["__pyeml__"] = {}
    main_module.__dict__["__pyeml__"]["__pyemlscope__"] = scope
    main_module.__dict__["__pyeml__"]["__pyemlargs__"] = args
    main_module.__dict__["__pyeml__"]["__pyemlkwargs__"] = kwargs
    main_module.__dict__["__pyeml__"]["__pyemlfunc__"] = function

    energy = measure_code(f"""
exec('__pyeml__["__pyemloutput__"] = __pyeml__["__pyemlfunc__"](*__pyeml__["__pyemlargs__"],**__pyeml__["__pyemlkwargs__"])', __pyeml__["__pyemlscope__"])
""", devices = devices, unit = unit)
    output = main_module.__dict__["__pyeml__"]["__pyemloutput__"]

    del main_module.__dict__["__pyeml__"]

    return output, energy


def measure_decorator( function = None, devices : Union[Iterable,None] = None, unit : Union[Dict, None] = None):
    def decorator(function : Callable ):
        def wrap(*args, **kwargs):
            return measure_function(function, devices,  args = args, kwargs = kwargs, scope = currentframe().f_back.f_globals, unit = unit)

        return wrap

    if(function is not None):
        return decorator(function)
    else:
        return decorator


