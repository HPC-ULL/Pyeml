
import sys
import os
sys.path.append(os.path.dirname(__file__))
from eml import measureCode

from inspect import currentframe

import __main__ as main_module

from typing import Callable, Iterable, Dict


def measure_code(code : str , preparation_code : str = ""):
    return measure_code(code=code, preparation_code=preparation_code)


def measure_function(function : Callable, args : Iterable = (), kwargs : Dict = {}, scope : any = None):

    scope = scope if scope is not None else currentframe().f_back.f_globals

    main_module.__dict__["__pyeml__"] = {}
    main_module.__dict__["__pyeml__"]["__pyemlscope__"] = scope
    main_module.__dict__["__pyeml__"]["__pyemlargs__"] = args
    main_module.__dict__["__pyeml__"]["__pyemlkwargs__"] = kwargs
    main_module.__dict__["__pyeml__"]["__pyemlfunc__"] = function

    energy = measureCode(f"""
exec('__pyeml__["__pyemloutput__"] = __pyeml__["__pyemlfunc__"](*__pyeml__["__pyemlargs__"],**__pyeml__["__pyemlkwargs__"])', __pyeml__["__pyemlscope__"])
""")
    output = main_module.__dict__["__pyeml__"]["__pyemloutput__"]

    del main_module.__dict__["__pyeml__"]

    return output, energy


def measure_decorator(function):
    def decorator(*args, **kwargs):
        return measure_function(function, args = args, kwargs = kwargs, scope = currentframe().f_back.f_globals)


    return decorator


