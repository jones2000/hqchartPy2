
#
#	Copyright (c) 2021 jones
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
#	开源项目 https://github.com/jones2000/HQChart
#
#	jones_2000@163.com
#
#	分钟K数据类 



import os
import json
import datetime
from enum import Enum, auto

from sqlalchemy import true

class MINUTE_KLINE_DATA_INDEX(Enum):
    DATE_ID=0
    YCLOSE_ID=1
    OPEN_ID=2
    HIGH_ID=3
    LOW_ID=4
    CLOSE_ID=5
    VOL_ID=6
    AMOUNT_ID=7
    TIME_ID=8

    MAX_COUNT=auto()


class MinuteKLineCache(object) :
    # 日K缓存
    m_Cache={ }             # 1分钟K线缓存
    m_bLoadFinished=False   # 缓存是否加载完成
    m_CachePath=None

    def SetCachePath(path):
        MinuteKLineCache.m_CachePath=path

    # 加载缓存
    def LoadCache() :
        MinuteKLineCache.m_bLoadFinished=False
        path=MinuteKLineCache.m_CachePath
        log="[MinuteKLineCache::LoadCache] start load day kline data. path='{0}' .....".format(path)
        print(log)

        aryFiles= os.listdir(path) 
        for item in aryFiles: 
            if not os.path.isdir(item): 
                strFile=path+"/"+item
                with open(strFile,encoding='utf-8') as f:
                    kData = json.load(f)
                    symbol=kData['symbol']
                    name=kData["name"]
                    dataCount=len(kData['data'])

                    log="[MinuteKLineCache::LoadCache] symbol={0}, name={1}, dataCount={2}".format(symbol,name,dataCount)
                    print(log)
                    MinuteKLineCache.m_Cache[symbol.lower()]=kData

        MinuteKLineCache.m_bLoadFinished=True
        log="[MinuteKLineCache::LoadCache] finish load day kline data. count={0}".format(len(MinuteKLineCache.m_Cache))
        print(log)

    # 查找K线数据
    def GetMinuteKLineCache(symbol):
        if (MinuteKLineCache.m_bLoadFinished==False):
            return None
        if ( symbol not in MinuteKLineCache.m_Cache):
            return None
        return MinuteKLineCache.m_Cache[symbol]

    # 读取K线文件
    def ReadMinuteKLineFile(symbol):
        kdataFile="{0}/{1}.json".format(MinuteKLineCache.m_CachePath,symbol)    # 日线数据
        try:
            with open(kdataFile,encoding='utf-8') as f:
                kData = json.load(f)
                symbol=kData["symbol"]
                if (symbol!=None) :
                    return kData
        except :
            log="[MinuteKLineCache::ReadDayKLineFile] Error: read kline data failed.{0}".format(symbol)
            print(log)
            return None
        return None

    # 转换成HQChartPy数据格式
    def ConvertToHQChartData(kData, result):
        dataCount=len(kData['data'])
        aryDate=[]
        aryClose=[]
        aryYClose=[]
        aryOpen=[]
        aryHigh=[]
        aryLow=[]
        aryVol=[]
        aryAmount=[]
        aryTime=[]

        for i in range(dataCount) :
            item=kData['data'][i]
            aryDate.append(item[0])
            aryYClose.append(item[1])
            aryOpen.append(item[2])
            aryHigh.append(item[3])
            aryLow.append(item[4])
            aryClose.append(item[5])
            aryVol.append(item[6])
            aryAmount.append(item[7])
            aryTime.append(item[8])

        result["count"]=dataCount
        result["date"]=aryDate
        # cacheData["time"]=aryTime
        result["yclose"]=aryYClose
        result["open"]=aryOpen
        result["high"]=aryHigh
        result["low"]=aryLow
        result["close"]=aryClose
        result["vol"]=aryVol
        result["amount"]=aryAmount
        result["time"]=aryTime


    # 拷贝数据
    def CopyKItem(kItem):   
        newItem=[0]*MINUTE_KLINE_DATA_INDEX.MAX_COUNT.value
        newItem[MINUTE_KLINE_DATA_INDEX.DATE_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.DATE_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.YCLOSE_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.YCLOSE_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.OPEN_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.OPEN_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.VOL_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.VOL_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.AMOUNT_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.AMOUNT_ID.value]
        newItem[MINUTE_KLINE_DATA_INDEX.TIME_ID.value]=kItem[MINUTE_KLINE_DATA_INDEX.TIME_ID.value]

        return newItem

    def IsMinutePeriod(period):
        if (period==4 or period==5 or period==6 or period==7 or period==8 or period==11 or period==12):
            return True
        return False

    # 计算周期  5=5分钟 6=15分钟 7=30分钟 8=60分钟 11=120分钟 12=240
    def CalculatePeriod(aryData, period):
        result = []
        periodDataCount = 5
        if period == 5 :
            periodDataCount = 5
        elif period == 6 :
            periodDataCount = 15
        elif period == 7 :
            periodDataCount = 30
        elif period == 8 :
            periodDataCount = 60
        elif period==11 :
            periodDataCount=120
        elif period==12 :
            periodDataCount=240

        bFirstPeriodData = False
        newData = None
        dataCount=len(aryData)
        i=-1
        while i<dataCount :    # for (var i = 0; i < this.Data.length; )
            bFirstPeriodData = True

            # for (var j = 0; j < periodDataCount && i < this.Data.length; ++i)
            j=0 
            while j<periodDataCount and i<dataCount :
                i+=1
                if i>=dataCount :
                    break
            
                if bFirstPeriodData :
                    newData = [0]*MINUTE_KLINE_DATA_INDEX.MAX_COUNT.value
                    result.append(newData)
                    bFirstPeriodData = False

                minData = aryData[i]
                if minData == None :
                    j+=1
                    continue

                ## if minData.Time in(925 ,930 ,1300) :
                ##    pass
                ## else :
                ##    j+=1

                j+=1
                newData[MINUTE_KLINE_DATA_INDEX.DATE_ID.value] = minData[MINUTE_KLINE_DATA_INDEX.DATE_ID.value]
                newData[MINUTE_KLINE_DATA_INDEX.TIME_ID.value] = minData[MINUTE_KLINE_DATA_INDEX.TIME_ID.value]
                if minData[MINUTE_KLINE_DATA_INDEX.OPEN_ID.value]==None or minData[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]==None :
                    continue
                if newData[MINUTE_KLINE_DATA_INDEX.OPEN_ID.value]==0 or newData[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]==0 :
                    newData[MINUTE_KLINE_DATA_INDEX.OPEN_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.OPEN_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.YCLOSE_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.YCLOSE_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.VOL_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.VOL_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.AMOUNT_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.AMOUNT_ID.value]    
                else :
                    if newData[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]<minData[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value] :
                        newData[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.HIGH_ID.value]
                    if newData[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]>minData[MINUTE_KLINE_DATA_INDEX.LOW_ID.value] :
                        newData[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.LOW_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]=minData[MINUTE_KLINE_DATA_INDEX.CLOSE_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.VOL_ID.value]+=minData[MINUTE_KLINE_DATA_INDEX.VOL_ID.value]
                    newData[MINUTE_KLINE_DATA_INDEX.AMOUNT_ID.value]+=minData[MINUTE_KLINE_DATA_INDEX.AMOUNT_ID.value]

        return result

    # right: 1=前复权 2=后复权 0=不复权 分钟数据支持复权
    # period: 4=1分钟 5=5分钟 6=15分钟 7=30分钟 8=60分钟 11=120分钟 12=240
    def GetMinuteKLine(symbol, period=4, right=0, startDate=None, endDate=None, nCalculateCount=None) :
        srcKData=MinuteKLineCache.GetMinuteKLineCache(symbol)
        if (srcKData ==None) :
            srcKData=MinuteKLineCache.ReadMinuteKLineFile(symbol)
        if (srcKData==None):
            log="[MinuteKLineCache::GetMinuteKLine] Error: get kline data failed.{0}".format(symbol)
            print(log)
            return None

        dataCount=len(srcKData["data"])
        result={}
        result['symbol']=srcKData['symbol']
        result['name']=srcKData['name']
        result['period']=period
        result['right']=right

        # 指定计算个数
        if (nCalculateCount!=None and nCalculateCount>0 and dataCount>nCalculateCount):
            aryData=srcKData["data"][-nCalculateCount:]
            if (period==4):
                kData={ 'data':aryData }
                MinuteKLineCache.ConvertToHQChartData(kData, result)
                return result

        if (period==4) :
            MinuteKLineCache.ConvertToHQChartData(srcKData, result)
            return result

        kData={}
        aryData=srcKData["data"]

        # 计算周期
        if (period==4 or period==5 or period==6 or period==7 or period==8 or period==11 or period==12 ):
            aryData=MinuteKLineCache.CalculatePeriod(aryData, period)

        kData['data']=aryData
        MinuteKLineCache.ConvertToHQChartData(kData, result)
        return result

        
    def GetMinuteKLineAPIData(symbol, period=0, right=0, startDate=None, endDate=None):
        srcKData=MinuteKLineCache.GetMinuteKLineCache(symbol)
        if (srcKData ==None) :
            srcKData=MinuteKLineCache.ReadMinuteKLineFile(symbol)
        if (srcKData==None):
            log="[MinuteKLineCache::GetMinuteKLineAPIData] Error: get kline data failed.{0}".format(symbol)
            print(log)
            return None

        kData={ "name": srcKData["name"], "symbol":srcKData["symbol"] }
        if (period==4) :
            aryData=srcKData["data"]
            kData['data']=aryData
            return kData

        
        aryData=srcKData["data"]

        # 计算周期
        if (period==5 or period==6 or period==7 or period==8 or period==11 or period==12):
            aryData=MinuteKLineCache.CalculatePeriod(aryData, period)

        kData['data']=aryData
        return kData

def TestCase():
    MinuteKLineCache.SetCachePath("../data/kdata/4")
    MinuteKLineCache.LoadCache()
    test=MinuteKLineCache.GetMinuteKLine("600000.sh")
    test2=MinuteKLineCache.GetMinuteKLine("000001.sz", 5, 0)

# TestCase()
           