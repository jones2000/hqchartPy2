import string
from hqchartpy2_fast import FastHQChart
from hqchartpy2_hqchartdata import HQChartData
from hqchartpy2_hqchartresult import HQChartResult, HQSelectResult
from hqchartpy2_klinedata import KLineCache

import json
import time
import uuid


class IHQChartPyRun(object):
    def __init__(self):
        self.m_arySymbol=None  # 股票代码
        self.m_strScript=None
        self.m_nPeriod=0    # 周期
        self.m_nRight=0     # 复权
        self.m_aryArgs=[]   # 参数
        self.m_nOutCount=-1 # 输出数据个数
        self.m_nStartDate=None
        self.m_nEndData=None
        self.m_nCalculateCount=None #指定计算K线个数

        self.m_strJobID=str(uuid.uuid1())   # 任务ID
        self.m_ResponseData=None
        self.m_HQData=None
        self.m_Result=None
        self.m_Error=None        # 错误信息

        self.m_StartTime=time.process_time()    # 起始时间
        self.m_nRunTick=0                       # 执行耗时

###########################################################################
# {
#   "Right":0,
#   "Period":1,
#   "Script":"RSV:=(CLOSE-LLV(LOW,6.6))/(HHV(HIGH,6.6)-LLV(LOW,6.6))*100;\nK:SMA(RSV,M,1);\nD:SMA(K,M,1);\nJ:3*K-2*D;",
#   "Symbol":"000006.sz",
#   "Args":[{"Name":"M","Value":3}],
#   "OutCount":-1
# }
##########################################################################
class HQChartPySimpleRun(IHQChartPyRun):
    def __init__(self):
        super(HQChartPySimpleRun, self).__init__()

    def SetPostData(self, request) :
        if (request.method!="POST"):
            self.m_Error="only support 'Post' method."
            return False

        if (request.mimetype=="application/x-www-form-urlencoded") :
            requestData=request.form
        else :
            requestData = json.loads(request.get_data(as_text=True))

        strSymbol=requestData['Symbol']
        self.m_arySymbol=[strSymbol]
        self.m_strScript=requestData["Script"]

        # 可选参数
        if ("Period" in requestData):
            self.m_nPeriod=requestData["Period"]
        if ("Right" in requestData) :
            self.m_nRight=requestData["Right"]
        if ("Args" in requestData) :
            self.m_aryArgs=requestData["Args"]
        if ("StartDate" in requestData) :
            self.m_nStartDate=requestData["StartDate"]
        if ("EndDate" in requestData) :
            self.m_nEndData=requestData["EndDate"]
        if ("OutCount" in requestData) :
            self.m_nOutCount=requestData["OutCount"]
        if ("CalcCount" in requestData):
            self.m_nCalculateCount=requestData["CalcCount"]

        # todo:检测参数有效性
        # self.m_Error="xxxxx错误提示"
        return True

    def GetRunConfig(self) :
        runConfig={
            # 系统指标名字
            # "Name":"MA",
            "Script":self.m_strScript,
            # 脚本参数
            "Args":self.m_aryArgs,
            # 周期 复权
            "Period":self.m_nPeriod, "Right":self.m_nRight,
            "Symbol":self.m_arySymbol[0],
            # "OutCount":1,
            "OutCount":self.m_nOutCount,
            #jobID (可选)
            "JobID":self.m_strJobID
        }

        return json.dumps(runConfig)    # 运行配置项

    def Run(self):
        jsConfig = self.GetRunConfig()      # 运行配置项
        self.m_HQData=HQChartData()         # 实例化数据类
        self.m_Result=HQChartResult()       # 实例计算结果接收类

        self.m_HQData.m_nCalculateCount=self.m_nCalculateCount

        res=FastHQChart.Run(jsConfig,self.m_HQData,proSuccess=self.m_Result.RunSuccess, procFailed=self.m_Result.RunFailed)

        self.m_nRunTick = (time.process_time() - self.m_StartTime)
        log="[HQChartPySimpleRun::Run] Time used:{0}, {1}".format(self.m_nRunTick, jsConfig)
        print(log)

        if (not res) :
            self.m_Error=self.m_Result.Error

    def GetResponseData(self):
        if (self.m_Error!=None):
            responseData={'Code':1, "Tick":self.m_nRunTick, "Error": self.m_Error, "JobID": self.m_strJobID }
            return responseData

        jsonData=self.m_Result.Result
        responseData=json.loads(jsonData)
        responseData["Code"]=0                  # 成功
        responseData['Tick']=self.m_nRunTick    # 耗时
        responseData['StartDate']=self.m_nStartDate
        responseData["EndDate"]=self.m_nEndData
        responseData["JobID"]=self.m_strJobID

        return responseData


############################################################################################
# {
#   "Right":0,
#   "Period":1,
#   "Script":"RSV:=(CLOSE-LLV(LOW,6.6))/(HHV(HIGH,6.6)-LLV(LOW,6.6))*100;\nK:SMA(RSV,M,1);\nD:SMA(K,M,1);\nJ:3*K-2*D;",
#   "Symbol":["000006.sz","000001.sz"],
#   "Args":[{"Name":"M","Value":3}],
#   "OutCount":1
# }
#############################################################################################

class HQChartPyMultiRun(IHQChartPyRun):
    def __init__(self):
        super(HQChartPyMultiRun, self).__init__()

    def SetPostData(self, request) :
        if (request.method!="POST"):
            self.m_Error="only support 'Post' method."
            return False

        if (request.mimetype=="application/x-www-form-urlencoded") :
            requestData=request.form
        else :
            requestData = json.loads(request.get_data(as_text=True))

        strSymbol=requestData['Symbol']
        self.m_arySymbol=strSymbol
        self.m_strScript=requestData["Script"]

        # 可选参数
        if ("Period" in requestData):
            self.m_nPeriod=requestData["Period"]
        if ("Right" in requestData) :
            self.m_nRight=requestData["Right"]
        if ("Args" in requestData) :
            self.m_aryArgs=requestData["Args"]
        if ("StartDate" in requestData) :
            self.m_nStartDate=requestData["StartDate"]
        if ("EndDate" in requestData) :
            self.m_nEndData=requestData["EndDate"]
        if ("OutCount" in requestData) :
            self.m_nOutCount=requestData["OutCount"]
        if ("CalcCount" in requestData):
            self.m_nCalculateCount=requestData["CalcCount"]

        # todo:检测参数有效性
        # self.m_Error="xxxxx错误提示"
        return True

    def GetRunConfig(self) :
        runConfig={
            # 系统指标名字
            # "Name":"MA",
            "Script":self.m_strScript,
            # 脚本参数
            "Args":self.m_aryArgs,
            # 周期 复权
            "Period":self.m_nPeriod, "Right":self.m_nRight,
            "Symbol":self.m_arySymbol,
            # "OutCount":1,
            "OutCount":self.m_nOutCount,
            #jobID (可选)
            "JobID":self.m_strJobID
        }

        return json.dumps(runConfig)    # 运行配置项

    def Run(self):
        jsConfig = self.GetRunConfig()      # 运行配置项
        self.m_HQData=HQChartData()         # 实例化数据类
        self.m_Result=HQSelectResult()      # 实例计算结果接收类

        count=len(self.m_arySymbol)
        self.m_HQData.m_nCalculateCount=self.m_nCalculateCount
        self.m_Result.TotalCount=count

        res=FastHQChart.Run(jsConfig,self.m_HQData,proSuccess=self.m_Result.RunSuccess, procFailed=self.m_Result.RunFailed)

        self.m_nRunTick = (time.process_time() - self.m_StartTime)
    
        log="""
----------------------------[HQChartPyMultiRun::Run] 指标计算完成 -------------------------------------------------\n
指标脚本:{0}

耗时(s):{1}
股票个数:{2}
--------------------------------------------------------------------------------------------------------------------
""".format(self.m_strScript, self.m_nRunTick, count)

        print(log)

        if (not res) :
            self.m_Error=self.m_Result.Error

    def GetResponseData(self):
        if (self.m_Error!=None):
            responseData={'Code':1, "Tick":self.m_nRunTick, "Error": self.m_Error, "JobID": self.m_strJobID }
            return responseData

        responseData={}
        responseData["Data"]=[]
        for item in self.m_Result.Result:
            itemData=json.loads(item['Data'])
            responseData["Data"].append(itemData)

        responseData["Code"]=0                  # 成功
        responseData['Tick']=self.m_nRunTick    # 耗时
        responseData['StartDate']=self.m_nStartDate
        responseData["EndDate"]=self.m_nEndData
        responseData["JobID"]=self.m_strJobID

        return responseData





# 选股指标结果
class HQPolicyResult(object):
    def __init__(self):
        self.Result = { }    # 保存所有的执行结果
        self.Error=[]
        self.TotalCount=0   # 一共的股票个数

    # 执行成功回调
    def RunSuccess(self, symbol, jsData, jobID):
        if (symbol not in self.Result) :
            self.Result[symbol]=[{"Data":jsData, "ID": jobID }]
        else:
            self.Result[symbol].append({"Data":jsData, "ID": jobID })

        log="{0} success. job={1}, {2}/{3}".format(symbol,jobID, len(self.Result), self.TotalCount)
        print (log)
        # print (jsData)

    # 执行失败回调
    def RunFailed(self, code, symbol, error,jobID) :
        log="{0}\n{1} failed\n{2}".format(code, symbol,error)
        self.Error.append( {"Symbol":symbol, "Error":error, "ID":jobID} )
        print(log)

###########################################################################
#
#{
#  "Period":1,
#  "Right":0,
#  "CalcCount":200, 计算K线的个数
#  "OutCount":1,    指标返回数据的个数 1个就是返回最新的1条
#  "Policy":
#  [
#    { 
#      "Script" :"T:MA(C,5);",
#      "Args":[ {"Name":"M","Value":3} ],
#      "ID":"1" 指标id
#    },
#    { 
#      "Script" :"T2:MA(C,M);",
#      "Args":[ {"Name":"M","Value":15} ],
#      "ID":"2"
#    }
#  ],
#
#  "Symbol":[ "000001.sh", "000001.sz", "600000.sh" ]
#}
##########################################################################

class HQChartPyPolicyRun(IHQChartPyRun) :
    def __init__(self):
        super(HQChartPyPolicyRun, self).__init__()
        self.AryIndex=None  # 指标数组

    def SetPostData(self, request) :
        if (request.method!="POST"):
            self.m_Error="only support 'Post' method."
            return False

        if (request.mimetype=="application/x-www-form-urlencoded") :
            requestData=request.form
        else :
            requestData = json.loads(request.get_data(as_text=True))

        strSymbol=requestData['Symbol'] # 股票池
        self.m_arySymbol=strSymbol

        self.AryIndex=requestData["Policy"] # 策略

        # 可选参数
        if ("Period" in requestData):
            self.m_nPeriod=requestData["Period"]
        if ("Right" in requestData) :
            self.m_nRight=requestData["Right"]
        if ("StartDate" in requestData) :
            self.m_nStartDate=requestData["StartDate"]
        if ("EndDate" in requestData) :
            self.m_nEndData=requestData["EndDate"]
        if ("OutCount" in requestData) :
            self.m_nOutCount=requestData["OutCount"]
        if ("CalcCount" in requestData):
            self.m_nCalculateCount=requestData["CalcCount"]

        # todo:检测参数有效性
        # self.m_Error="xxxxx错误提示"
        return True

    def GetRunConfig(self, item, id) :

        if ("Script" not in item):
            return None

        runConfig={
            # 系统指标名字
            # "Name":"MA",
            "Script":item["Script"],
            # 脚本参数
            "Args":None,
            # 周期 复权
            "Period":self.m_nPeriod, 
            "Right":self.m_nRight,

            "Symbol":self.m_arySymbol,
            # "OutCount":1,
            "OutCount":self.m_nOutCount,
            #jobID (可选)
            "JobID":str(id)
        }

        if ("Args" in item):
            runConfig["Args"]=item["Args"]

        if ("ID" in item):
            runConfig["ID"]=item["ID"]

        return json.dumps(runConfig)    # 运行配置项

    def Run(self):

        self.m_HQData=HQChartData()         # 实例化数据类
        self.m_Result=HQPolicyResult()      # 实例计算结果接收类
        self.m_HQData.m_nCalculateCount=self.m_nCalculateCount
        count=len(self.m_arySymbol)
        self.m_Result.TotalCount=count

        for i in range(len(self.AryIndex)) :
            item=self.AryIndex[i]
            jsConfig = self.GetRunConfig(item, i)      # 运行配置项
            if (jsConfig==None):
                continue
            runStart=time.process_time()
            res=FastHQChart.Run(jsConfig,self.m_HQData,proSuccess=self.m_Result.RunSuccess, procFailed=self.m_Result.RunFailed)    
            self.m_nRunTick = (time.process_time() - runStart)

            log="""
----------------------------[HQChartPyPolicyRun::Run] 指标计算完成 -------------------------------------------------\n
指标脚本:{0}

耗时(s):{1}
股票个数:{2}
--------------------------------------------------------------------------------------------------------------------
""".format(item["Script"], self.m_nRunTick, count)

            print(log)

        self.m_nRunTick=(time.process_time() - self.m_StartTime)
        print("Job:{0}, 一共耗时(s):{1}".format(self.m_strJobID, self.m_nRunTick))


    def GetResponseData(self):
        if (self.m_Error!=None):
            responseData={'Code':1, "Tick":self.m_nRunTick, "Error": self.m_Error, "JobID": self.m_strJobID }
            return responseData

        responseData={}
        responseData["Data"]=self.m_Result.Result

        responseData["Code"]=0                  # 成功
        responseData['Tick']=self.m_nRunTick    # 耗时
        responseData['StartDate']=self.m_nStartDate
        responseData["EndDate"]=self.m_nEndData
        responseData["JobID"]=self.m_strJobID

        return responseData

        
