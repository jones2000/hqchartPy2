from flask import Flask, render_template
from flask import request, jsonify
from flask_cors import CORS

import json
import time
import uuid

from hqchartpy2_fast import FastHQChart
from hqchartpy2_hqchartdata import HQChartData
from hqchartpy2_hqchartresult import HQChartResult, HQSelectResult
from hqchartpy2_klinedata import KLineCache
from hqchartpy2_klineminutedata import MinuteKLineCache
from hqchartpy2_run import HQChartPySimpleRun, HQChartPyMultiRun, HQChartPyPolicyRun
from hqchartpy2_hqapidata import DayKLineController, MinuteKLineController

app = Flask(__name__)
CORS(app) # 跨域 


@app.route('/', methods=['GET'])
def home():
    version=FastHQChart.GetVersion()
    return '''<h1>HQChartPy2</h1>
<p>版本号:{0}</p>
<p>授权信息:{1}</p>'''.format(version, FastHQChart.Authorize)


# 单指标
@app.route('/api/Run', methods=['POST', "GET"])
def Single_Run():
    pyRun=HQChartPySimpleRun()
    if (pyRun.SetPostData(request)) :
        pyRun.Run()
    
    responseData=pyRun.GetResponseData()
    return jsonify(responseData)

# 批量指标计算
@app.route('/api/RunAll', methods=['POST', "GET"])
def Multiple_Run():
    pyRun=HQChartPyMultiRun()
    if (pyRun.SetPostData(request)) :
        pyRun.Run()
    
    responseData=pyRun.GetResponseData()
    return jsonify(responseData)

# 策略计算
@app.route('/api/Policy', methods=['POST', "GET"])
def Policy_Run():
    pyRun=HQChartPyPolicyRun()
    if (pyRun.SetPostData(request)) :
        pyRun.Run()
    
    responseData=pyRun.GetResponseData()
    return jsonify(responseData)

# 前端K线数据
@app.route('/api/DayKLine', methods=['POST', "GET"])
def DayKLine():
    controller=DayKLineController()
    if (controller.SetPostData(request)):
        controller.Run()

    responseData=controller.GetResponseData()
    return jsonify(responseData)

@app.route('/api/MinuteKLine', methods=['POST', "GET"])
def MinuteKLine():
    controller=MinuteKLineController()
    if (controller.SetPostData(request)):
        controller.Run()

    responseData=controller.GetResponseData()
    return jsonify(responseData)


if __name__ == '__main__':
    FastHQChart.Initialization(None)    #初始化

    # 初始化缓存
    # 分钟K线加载
    MinuteKLineCache.SetCachePath("../data/kdata/4")
    MinuteKLineCache.LoadCache()
    # test=MinuteKLineCache.GetMinuteKLine("600000.sh")
    # test2=MinuteKLineCache.GetMinuteKLine("000001.sz", 5, 0)

    # 日线数据加载
    KLineCache.SetCachePath("../data/kdata/0") 
    # KLineCache.LoadCache()
    app.run(host='127.0.0.1', port=8712, debug=True)



