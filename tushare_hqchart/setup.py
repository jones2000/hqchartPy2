from setuptools import setup, find_packages, Distribution
import json

with open('tushare_hqchart/pkg_info.json') as fp:
    info = json.load(fp)

setup(
    name = "tushare_hqchart",
    version = info["version"],
    author = "jones2000",
    author_email = "jones_2000@163.com",
    description = "HQChartPy2 C++",
    license = "Apache License 2.0",
    keywords = "HQChart HQChartPy2",
    url = "https://github.com/jones2000/hqchartPy2",

    install_requires=['requests', 'pandas', 'numpy', "hqchartPy2", "hqchart", "tushare"],

    packages=find_packages(include=["tushare_hqchart"]), 

    package_data  = { "tushare_hqchart":["pkg_info.json"] },
   
    classifiers=
    [
        'Operating System :: Microsoft :: Windows'
    ],
)