import json
import time
import numpy as np 
import pandas as pd


class HQChartPy2Helper:
    @staticmethod
    def JsonDataToPandas(jsData, symbol=None) :  # 指标json结果数据转pandas
        indexData = json.loads(jsData)
        date=indexData["Date"]
        data={ "Date":date }
        if ("Time" in indexData.keys()) :   
            data["Time"]=indexData["Time"]

        if (symbol!=None):
            symbols=[symbol]*len(date)
            data["symbols"]=symbols

        outVar=indexData["OutVar"]
        for item in outVar:
            if (item["Type"]!=0):
                continue
            if ("Data" in item.keys()) :
                data[item["Name"]]=item["Data"]
            else :
                data[item["Name"]]=None
        df = pd.DataFrame(data)
        return df
