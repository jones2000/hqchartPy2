import sys
import os
import json
from os.path import dirname

with open(dirname(__file__) + '/pkg_info.json') as fp:
    _info = json.load(fp)

__version__ = _info['version']

from HQChart.hqchartpy2_fast import PERIOD_ID, IHQData, FastHQChart
from HQChart.hqchartpy2_pandas import HQChartPy2Helper



