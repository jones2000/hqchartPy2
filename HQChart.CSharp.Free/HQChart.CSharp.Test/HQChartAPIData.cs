using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using CsvHelper;
using Newtonsoft.Json;
using HQChart.Interface;
using System.Diagnostics;



/*
 *  Copyright (c) 2018 jones

    HQChart前端 开源项目 https://github.com/jones2000/HQChart

    jones_2000@163.com

    hqchart.csharp.dll 数据对接类demo
 * 
*/

namespace HQChart.Data
{
    //数据缓存
    namespace Cache
    {
        public class KLineDayItem
        {
            public int date { get; set; }
            public double yclose { get; set; }
            public double open { get; set; }
            public double high { get; set; }
            public double low { get; set; }
            public double close { get; set; }
            public double vol { get; set; }
            public double amount { get; set; }

        }
        
        class HQChartCache
        {
            //日K缓存
            private Dictionary<string, List<HQCHART_KDATA>> m_mapKLineDay = new Dictionary<string, List<HQCHART_KDATA>>();

            private bool LoadKLineData(string strSymbol)
            {
                try
                {
                    string file = string.Format(@".\{0}.csv", strSymbol);
                    string strPath = Path.Combine(Environment.CurrentDirectory, file);

                    using (var reader = new StreamReader(strPath, Encoding.Default))
                    {
                        using (var csv = new CsvReader(reader, System.Globalization.CultureInfo.CurrentCulture))
                        {
                            var list = csv.GetRecords<KLineDayItem>().ToList();
                            if (list.Count <= 0) return false;

                            List<HQCHART_KDATA> aryKline = new List<HQCHART_KDATA>();
                            foreach (var item in list)
                            {
                                var kItem = new HQCHART_KDATA()
                                {
                                    _nDate = item.date,
                                    _dYClose = item.yclose,
                                    _dOpen = item.open,
                                    _dHigh = item.high,
                                    _dLow = item.low,
                                    _dClose = item.close,
                                    _dVol = item.vol,
                                    _dAmount = item.amount
                                };

                                aryKline.Add(kItem);
                            }

                            m_mapKLineDay[strSymbol] = aryKline;
                        }
                    }
                }
                catch (UnauthorizedAccessException e)
                {
                    //throw new Exception(e.Message);
                    return false;
                }
                catch (FieldValidationException e)
                {
                    //throw new Exception(e.Message);
                    return false; ;
                }
                catch (CsvHelperException e)
                {
                    //throw new Exception(e.Message);
                    return false; ;
                }
                catch (Exception e)
                {
                    //throw new Exception(e.Message);
                    return false;
                }

                return true;
            }

            public bool GetKLineDay(string strSymbol, out List<HQCHART_KDATA> aryData)
            {
                if (!m_mapKLineDay.ContainsKey(strSymbol))
                    LoadKLineData(strSymbol);

                return m_mapKLineDay.TryGetValue(strSymbol, out aryData);
            }
        }
    }

    /// <summary>
    /// 数据对接类(!!!!所有计算需要的数据全部由外部传入,hqchart只负责计算)
    /// </summary>
    class HQChartAPIData : IHQChartData
    {
        private IntPtr m_pPtr = IntPtr.Zero;
        private IntPtr m_pPtrString = IntPtr.Zero;
        //private long m_lPeriod = 0;         //0=日线 1=周线 2=月线 3=年线 9=季线 21=双周 22=半年 4=1分钟 5=5分钟 6=15分钟 7=30分钟 8=60分钟 11=120分钟 12=240分钟
        //private long m_lRight = 0;          //0=不复权 1=前复权 2=后复权
        //private string m_strSymbol;         //代码
        //private string m_strError;

        private static HashSet<string> INDEX_VAR_NAME = new HashSet<string> { "INDEXA", "INDEXC", "INDEXH", "INDEXL", "INDEXO", "INDEXV" };

        private Cache.HQChartCache m_Cache = new Cache.HQChartCache();

        public HQChartAPIData()
        {
            
        }

        ~HQChartAPIData()
        {
            ClearCachePtr();
        }

        private void ClearCachePtr()
        {
            if (m_pPtr != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(m_pPtr);
                m_pPtr = IntPtr.Zero;
            }

            if (m_pPtrString != IntPtr.Zero)
            {
                Marshal.FreeBSTR(m_pPtrString);
                m_pPtrString = IntPtr.Zero;
            }
        }

        

        private bool DownloadKData(string lpSymbol, int lPeriod, int lRight, ref List<HQCHART_KDATA> aryData)
        {
            /*
            ClearCachePtr();
            try
            {
                var strInterSymbol = ToInternalSymbol(lpSymbol);
                //if(lPeriod==0 && (lRight==1 || lRight==2))
                //{
                //    return LoadKDataRightData(lpSymbol, lPeriod, lRight, ref aryData);
                //}

                long lType = ToInternalPeriod(lPeriod, lRight);
                if (lType < 0) return false;

                //日线前后复权用新域名
                string domain = "hqwiapi.hexun.com";
                if (lType == 18 || lType == 19)
                    domain = "stocksquote.hexun.com";

                var url = string.Format("https://{0}/a/kline?code={1}&start={2}&number={3}&type={4}", domain, strInterSymbol, m_lStartDateTime, m_lCount, lType);

                var task = m_HttpClient.GetAsync(url);
                var response = task.Result;
                response.EnsureSuccessStatusCode();

                var task2 = response.Content.ReadAsStringAsync();
                var responseBody = task2.Result;
                var strJson = responseBody.Substring(1, responseBody.Length - 3);

                JObject root = JObject.Parse(strJson);

                //TODO: 检测json数据格式
                var aryJsData = root["Data"];
                var aryJsKData = aryJsData[0];
                long lStartDate = long.Parse(aryJsData[1].ToString());
                long lEndDate = long.Parse(aryJsData[2].ToString());
                double lPriceNum = long.Parse(aryJsData[4].ToString());
                aryData = new List<HQCHART_KDATA>();
                foreach (var item in aryJsKData)
                {
                    //TODO: 检测json数据格式
                    HQCHART_KDATA kItem = new HQCHART_KDATA();
                    long lDateTime = long.Parse(item[0].ToString());
                    long lYClose = long.Parse(item[1].ToString());
                    long lOpen = long.Parse(item[2].ToString());
                    long lClose = long.Parse(item[3].ToString());
                    long lHigh = long.Parse(item[4].ToString());
                    long lLow = long.Parse(item[5].ToString());
                    long lVol = long.Parse(item[6].ToString());
                    long lAmount = long.Parse(item[7].ToString());

                    kItem._nDate = (int)(lDateTime / 1000000);
                    kItem._nTime = (int)((lDateTime % 1000000) / 100);
                    kItem._dYClose = lYClose / lPriceNum;
                    kItem._dOpen = lOpen / lPriceNum;
                    kItem._dClose = lClose / lPriceNum;
                    kItem._dHigh = lHigh / lPriceNum;
                    kItem._dLow = lLow / lPriceNum;
                    kItem._dVol = lVol;
                    kItem._dAmount = lAmount;

                    aryData.Add(kItem);
                }

                return true;
            }
            catch (Exception ex)
            {
                m_strError = ex.Message;
                return false;
            }
            */
            return false;
        }

        /// <summary>
        /// 获取系统指标脚本 (脚本内调用系统函数会触发这个回调)
        /// </summary>
        /// <param name="lpName"></param>
        /// <param name="lpCallInfo"></param>
        /// <param name="pstrGuid"></param>
        /// <returns></returns>
        public IntPtr GetIndexScript(string lpName, string lpCallInfo, string pstrGuid)
        {
            string strJson = "";
            if (lpName == "MACD")
            {
                IndexScript script = new IndexScript
                {
                    Name = lpName,
                    Script = "DIF:EMA(CLOSE,SHORT)-EMA(CLOSE,LONG);\r\n"+
                            "DEA:EMA(DIF, MID);\r\n"+
                            "MACD: (DIF - DEA) * 2,COLORSTICK;",
                    Args = new List<ArgmentItem>
                    {
                        new ArgmentItem { Name="SHORT", Value=12 },
                        new ArgmentItem{Name="LONG", Value=26 },
                        new ArgmentItem{Name="MID", Value=9 }
                    }
                };
                strJson = JsonConvert.SerializeObject(script);
            }
            else if (lpName=="TEXT")
            {
                IndexScript script = new IndexScript
                {
                    Name = lpName,
                    Script = "T1:MA(C,M);\r\nT2:MA(C,N);T3:(H+L)/2;",
                    Args = new List<ArgmentItem>
                    {
                        new ArgmentItem { Name="M", Value=5 },
                        new ArgmentItem{Name="N", Value=10 }
                    }
                };

                strJson = JsonConvert.SerializeObject(script);
            }

            ClearCachePtr();

            m_pPtrString = Marshal.StringToBSTR(strJson);
            return m_pPtrString;
        }

        public bool LoadKData(string lpSymbol, int lPeriod, int lRight, ref HQCHART_KDATA_RESULT pResult, string pstrGuid)
        {
            ClearCachePtr();

            List<HQCHART_KDATA> aryKData = null;
            if (!m_Cache.GetKLineDay(lpSymbol, out aryKData))
                return false;
            
            int nSize = Marshal.SizeOf(typeof(HQCHART_KDATA)) * aryKData.Count;
            m_pPtr = Marshal.AllocHGlobal(nSize);

            for (int i = 0; i < aryKData.Count; ++i)
            {
                HQCHART_KDATA kItem = aryKData[i];
                IntPtr pPtr = new IntPtr(m_pPtr.ToInt64() + Marshal.SizeOf(typeof(HQCHART_KDATA)) * i);
                Marshal.StructureToPtr(kItem, pPtr, true);
            }

            pResult._pData = m_pPtr;
            pResult._lCount = aryKData.Count;
            pResult._lPeriod = lPeriod;
            pResult._lRight = lRight;

            pResult._pszName = Marshal.StringToBSTR(lpSymbol);  //TODO: 目前数据没有股票名称
            return true;
        }

        public bool LoadKData2(string lpSymbol, int lPeriod, int lRight, string lpCallInfo, ref HQCHART_KDATA_INFO kDataInfo, ref HQCHART_KDATA_RESULT pResult, string pstrGuid)
        {
            ClearCachePtr();

            List<HQCHART_KDATA> aryKData = null;
            if (!m_Cache.GetKLineDay(lpSymbol, out aryKData))
                return false;

            int nSize = Marshal.SizeOf(typeof(HQCHART_KDATA)) * aryKData.Count;
            m_pPtr = Marshal.AllocHGlobal(nSize);

            for (int i = 0; i < aryKData.Count; ++i)
            {
                HQCHART_KDATA kItem = aryKData[i];
                IntPtr pPtr = new IntPtr(m_pPtr.ToInt64() + Marshal.SizeOf(typeof(HQCHART_KDATA)) * i);
                Marshal.StructureToPtr(kItem, pPtr, true);
            }

            pResult._pData = m_pPtr;
            pResult._lCount = aryKData.Count;
            pResult._lPeriod = lPeriod;
            pResult._lRight = lRight;

            pResult._pszName = Marshal.StringToBSTR(lpSymbol);  //TODO: 目前数据没有股票名称
            return true;
        }

        public bool GetDataByNumber(string lpSymbol, string pFunctionName, int lParam, int lPeriod, int lRight, ref HQCHART_KDATA_INFO kDataInfo, ref HQCHART_VALUE_RESULT pResult, string pstrGuid)
        {
            if (pFunctionName == "DYNAINFO") //最新行情
            {
                //DYNAINFO(lParam)
                return false;
            }
            else if (pFunctionName == "FINANCE")
            {
                //FINANCE(lParam)
                return false;
            }
            else
            {
                return false;
            }
        }

        public bool GetDataByNumbers(string lpSymbol, string pFunctionName, ref CUSTOM_FUNCTION_ARGUMENT argument, int lPeriod, int lRight, ref HQCHART_KDATA_INFO kDataInfo, ref HQCHART_VALUE_RESULT pResult, string pstrGuid)
        {
            return false;
        }

        public bool GetDataByName(string lpSymbol, string pFunctionName, int lPeriod, int lRight, ref HQCHART_KDATA_INFO kDataInfo, ref HQCHART_VALUE_RESULT pResult, string pstrGuid)
        {
            if (INDEX_VAR_NAME.Contains(pFunctionName)) //大盘数据
            {
                return false;
            }
            else if (pFunctionName== "GetHisCapital")   //历史流通股本 FINANCE(7)
            {
                return Finance(7, lpSymbol, ref pResult);
            }
            else if (pFunctionName== "CAPITAL") //当前流通股本
            {
                return false;
            }
            else if (pFunctionName== "TOTALCAPITAL")    //当前总股本
            {
                return false;
            }

            return false;
        }

        //财务数据
        protected bool Finance(int id, string strSymbol, ref HQCHART_VALUE_RESULT pResult)
        {
            return false;
        }

        /// <summary>
        /// 取大盘指数
        /// </summary>
        /// <param name="strSymbol"></param>
        /// <param name="strDataName"></param>
        /// <param name="lPeriod"></param>
        /// <param name="pResult"></param>
        /// <returns></returns>
        private bool IndexData(string strSymbol, string strDataName, int lPeriod, ref HQCHART_VALUE_RESULT pResult)
        {
            ClearCachePtr();

            List<HQCHART_KDATA> aryKData = null;
            if (!m_Cache.GetKLineDay("000001.sh", out aryKData))
                return false;

            int nSize = Marshal.SizeOf(typeof(VALUE_ITEM)) * aryKData.Count;
            m_pPtr = Marshal.AllocHGlobal(nSize);

            for (int i = 0; i < aryKData.Count; ++i)
            {
                HQCHART_KDATA kItem = aryKData[i];
                VALUE_ITEM fItem = new VALUE_ITEM();
                fItem._lDate = kItem._nDate;
                fItem._lTime = kItem._nTime;

                if (strDataName == "INDEXA") fItem._dValue = kItem._dAmount;
                else if (strDataName == "INDEXC") fItem._dValue = kItem._dClose;
                else if (strDataName == "INDEXH") fItem._dValue = kItem._dHigh;
                else if (strDataName == "INDEXL") fItem._dValue = kItem._dLow;
                else if (strDataName == "INDEXO") fItem._dValue = kItem._dOpen;
                else if (strDataName == "INDEXV") fItem._dValue = kItem._dVol;

                IntPtr pPtr = new IntPtr(m_pPtr.ToInt64() + Marshal.SizeOf(typeof(VALUE_ITEM)) * i);
                Marshal.StructureToPtr(fItem, pPtr, true);
            }

            pResult._lType = 3;
            pResult._pData = m_pPtr;
            pResult._lCount = aryKData.Count;
            return true;
        }
    }

    //指标计算结果保存
    public class HQChartResult : IHQChartResult
    {
        private Dictionary<string, string> m_mapResult = new Dictionary<string, string>();

        public Dictionary<string, string> Result { get { return m_mapResult; } }
        public void pHQChart_Success(string lpSymbol, string lpJSResult, string pstrGuid)
        {
            m_mapResult[lpSymbol] = lpJSResult;
            Debug.WriteLine(string.Format("[HQChartResult::pHQChart_Success] {0} finish.", lpSymbol));
        }

        public void pHQChart_Failed(string lpCode, string lpSymbol, string lpError, string pstrGuid)
        {
            string strLog = string.Format("symbol={0}, error={1}\r\n-----------------------\r\n{2}\r\n----------------", lpSymbol, lpError, lpCode);
            Debug.WriteLine(strLog);
        }
    }
}

