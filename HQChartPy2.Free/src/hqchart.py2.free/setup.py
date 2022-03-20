from distutils.core import setup, Extension


sfc_module = Extension('HQChartPy2', 
                       include_dirs = [('../HQChart.Complier.Source'),
                                       ('../HQChart.Complier.CallbackData') ,
                                       ('../rapidjson')],
                       sources=['hqchart.complier.py.interface.cpp'])

setup(name='HQChartPy2', version='1.0',
      description='Python Package with HQChart C++ extension',
      ext_modules=[sfc_module]
      )
