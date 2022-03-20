#
#	Copyright (c) 2021 jones
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
#	开源项目 https://github.com/jones2000/HQChart
#
#	jones_2000@163.com
#
#	指标计算结果类


# 单个指标
class HQChartResult(object):
    def __init__(self):
        self.Result=None    # 保存所有的执行结果
        self.Error=None
        self.Symbol=None
    
    # 执行成功回调
    def RunSuccess(self, symbol, jsData, jobID):
        self.Result=jsData # 保存结果
        self.Symbol=symbol
        log="{0} success".format(symbol)
        print (log)
        # print (jsData)

    # 执行失败回调
    def RunFailed(self, code, symbol, error,jobID) :
        self.Symbol=symbol
        log="{0}\n{1} failed\n{2}".format(code, symbol,error)
        self.Error=error
        print(log)

# 选股指标结果
class HQSelectResult(object):
    def __init__(self):
        self.Result = []    # 保存所有的执行结果
        self.Error=[]
        self.TotalCount=0 # 一共的股票个数

    # 执行成功回调
    def RunSuccess(self, symbol, jsData, jobID):
        self.Result.append({"Symbol":symbol, "Data":jsData})  # 保存结果
        log="{0} success. {1}/{2}".format(symbol,len(self.Result), self.TotalCount)
        print (log)
        # print (jsData)

    # 执行失败回调
    def RunFailed(self, code, symbol, error,jobID) :
        log="{0}\n{1} failed\n{2}".format(code, symbol,error)
        self.Error.append( {"Symbol":symbol, "Error":error} )
        print(log)