import sys
import os
import json
from os.path import dirname

with open(dirname(__file__) + '/pkg_info.json') as fp:
    _info = json.load(fp)

__version__ = _info['version']

from tushare_hqchart.hqchartpy2_tushare import TushareConfig, TushareHQChartData, TushareKLocalHQChartData, HQResultTest



