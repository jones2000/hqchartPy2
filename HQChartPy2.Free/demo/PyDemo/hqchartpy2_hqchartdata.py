#
#	Copyright (c) 2021 jones
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
#	开源项目 https://github.com/jones2000/HQChart
#
#	jones_2000@163.com
#
#	hqchartPy2数据对接类

import json
import time
from hqchartpy2_fast import FastHQChart,IHQData,PERIOD_ID
from hqchartpy2_klinedata import KLineCache
from hqchartpy2_klineminutedata import MinuteKLineCache

class HQChartData(IHQData) :
    def __init__(self):
        super(HQChartData, self).__init__() 
        # 指定计算K线日期范围
        self.m_nStartDate=None      
        self.m_nEndDate=None
        self.m_nCalculateCount=None #指定计算K线个数

    def SetDateRange(self, startDate, endDate):
        self.m_nStartDate=startDate
        self.m_nEndDate=endDate
    
    def GetKLineData(self, symbol, period, right, jobID):
        if (MinuteKLineCache.IsMinutePeriod(period)):
            return MinuteKLineCache.GetMinuteKLine(symbol,period,right, self.m_nStartDate, self.m_nEndDate, self.m_nCalculateCount)
            
        return KLineCache.GetDayKLine(symbol, period, right, self.m_nStartDate, self.m_nEndDate, self.m_nCalculateCount)

    def GetFinance(self,symbol, id, period,right,kcount,jobID) :

        if (id==7) :
            pyCacheData=[18653471415, 18653471415,21618279922,21618279922,28103763899, 28103763899,28103763899,28103763899]  # 数据
            pyCacheDate=[20170519, 20170830,20170906, 20180428,20180830, 20190326,20190824,20200425]  # 日期
            data={"type": 2, "date":pyCacheDate, "data":pyCacheData}
            return data
        else :
            pyCacheData=[]
            for i in range(kcount) :    # 生成财务数据
                pyCacheData.append(8976549.994+i)

            data={"type": 1, "data":pyCacheData}
            return data

    def GetDynainfo(self,symbol, id,period,right, kcount,jobID):
        data={"type": 0, "data":5}
        return data

    def GetCapital(self,symbol,period,right, kcount,jobID):
        data={"type": 0, "data":455555555.99}
        return data

    # 历史所有的流通股 
    def GetHisCapital(self,symbol, period, right, kcount,jobID):
        pyCacheData=[]
        for i in range(kcount) :    # 生成流通股数据
            pyCacheData.append(8976549.994+i)

        data={"type": 1, "data":pyCacheData}
        return data

    # 大盘数据
    def GetIndex(self, symbol, varName, period,right, kcount,jobID):
        if (varName==u'INDEXA') :   # 大盘成交额
            pass
        elif (varName==u'INDEXC') : # 大盘收盘价
            pass
        elif (varName==u'INDEXH') : # 大盘最高价
            pass
        elif (varName==u'INDEXL') : # 大盘最低价
            pass
        elif (varName==u'INDEXO') : # 大盘开盘价
            pass
        elif (varName==u'INDEXV') : # 大盘成交量
            pass
        elif (varName==u'INDEXADV') : # 上涨家数
            pass
        elif (varName==u'INDEXDEC') : # 下跌家数
            pass

        # 测试数据
        pyCacheData=[]
        for i in range(kcount) :
            pyCacheData.append(2888.8+i)
        data={"type": 1, "data":pyCacheData}
        return data
