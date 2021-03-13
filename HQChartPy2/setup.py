from setuptools import setup, find_packages, Distribution
import json

with open('HQChartPy2/pkg_info.json') as fp:
    info = json.load(fp)

setup(
    name = "HQChartPy2",
    version = info["version"],
    author = "jones2000",
    author_email = "jones_2000@163.com",
    description = "HQChartPy2 C++",
    license = "Apache License 2.0",
    keywords = "HQChart HQChartPy2",
    url = "https://github.com/jones2000/hqchartPy2",

    # install_requires=['requests', 'pandas', 'numpy'],

    packages=find_packages(include=["HQChartPy2"]), 

    package_data  = { "HQChartPy2":["*.dll", "HQChartPy2.pyd", "HQChartPy2.so", "pkg_info.json"] },
   
    classifiers=
    [
        'Operating System :: Microsoft :: Windows'
    ],
)