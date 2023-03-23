using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using HQChart.Interface;
using HQChart.Data;

namespace HQChart.CSharp.Test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();


            TextCode.Text = "T:MA(C,10)";
        }

        private void BtnRun_Click(object sender, EventArgs e)
        {
            int nn=HQChartDll.MainVersion();

            RunConfig config = new RunConfig();
            config.Symbol = "600000.sh";
            config.Period = 0;
            config.Right = 0;
            config.Script = TextCode.Text;
            config.JobID = Guid.NewGuid().ToString();
            config.OutCount = -1;        // 输出个数 -1全部
            config.Args = new List<ArgmentItem>()
            {
                new ArgmentItem() { Name = "N", Value = 13 },
                new ArgmentItem() { Name = "M", Value = 28 },
                new ArgmentItem() { Name = "P", Value = 30 },
                new ArgmentItem() { Name = "Q", Value = 48 },
                new ArgmentItem() { Name = "F", Value = 28 },
                new ArgmentItem() { Name = "W", Value = 9 },
            };
            string strJson = JsonConvert.SerializeObject(config);

            HQChartAPIData data = new HQChartAPIData();
            HQChartResult result = new HQChartResult();


            //数据回调委托
            HQCHART_CALLBACK_PTR callback = new HQCHART_CALLBACK_PTR();
            pHQChart_LoadKData pLoadKData = new pHQChart_LoadKData(data.LoadKData);
            pHQChart_LoadKData2 pLoadKData2 = new pHQChart_LoadKData2(data.LoadKData2);
            pHQChart_GetDataByNumber pGetDataByNumber = new pHQChart_GetDataByNumber(data.GetDataByNumber);
            pHQChart_GetDataByNumbers pGetDataByNumbers = new pHQChart_GetDataByNumbers(data.GetDataByNumbers);
            pHQChart_GetDataByName pGetDataByName = new pHQChart_GetDataByName(data.GetDataByName);
            pHQChart_GetIndexScript pGetIndexScript = new pHQChart_GetIndexScript(data.GetIndexScript);
            //数据执行完成通知委托
            pHQChart_Success pSuccess = new pHQChart_Success(result.pHQChart_Success);
            pHQChart_Failed pFailed = new pHQChart_Failed(result.pHQChart_Failed);

            callback._pLoadKData = pLoadKData;
            callback._pLoadKData2 = pLoadKData2;
            callback._pGetDataByNumber = pGetDataByNumber;
            callback._pGetDataByNumbers = pGetDataByNumbers;
            callback._pGetDataByName = pGetDataByName;
            callback._pGetIndexScript = pGetIndexScript;
            callback._pSuccess = pSuccess;
            callback._pFailed = pFailed;


            m_strRunConfig = strJson;
            m_RunCallback = callback;
            m_Result = result;

            this.listResult.Columns.Clear();
            this.listResult.Items.Clear();

            Thread runThread = new Thread(this.Run);
            runThread.Start();
        }

        private string m_strRunConfig;
        private HQCHART_CALLBACK_PTR m_RunCallback;
        private HQChartResult m_Result = new HQChartResult();
        private void Run()
        {
            {
                Action<string> SetLogDelegate = delegate (string strText) { Log.Text = strText; };

                Stopwatch sw = new Stopwatch();
                sw.Start();
                if (!HQChartDll.Run(m_strRunConfig, m_RunCallback))
                {
                    Log.Invoke(SetLogDelegate, new object[] { "执行失败" });
                    return;
                }

                sw.Stop();
                TimeSpan ts3 = sw.Elapsed;
                Log.Invoke(SetLogDelegate, new object[] { string.Format("指标计算完成, 耗时:{0}s", ts3.TotalSeconds) });

                listResult.Invoke(new UpdateResultDataDelegate(UpdateResultData) , new object[] { this.m_Result });
            }
        }

        private delegate void UpdateResultDataDelegate(HQChartResult result); //定义委托

        private void UpdateResultData(HQChartResult result)
        {
            this.listResult.Columns.Clear();
            this.listResult.Items.Clear();
            this.listResult.Columns.Add("Date", 100, HorizontalAlignment.Left);

            if (result.Result.Count <= 0) return;


            JObject jObject = JObject.Parse(result.Result.Values.ToList()[0]);

            var aryDate = jObject["Date"].ToArray();
            foreach (var item in aryDate)
            {
                ListViewItem rowItem = new ListViewItem();
                int nValue = Convert.ToInt32(item);
                rowItem.Text = string.Format("{0:D4}-{1:D2}-{2:D2}", (int)(nValue / 10000), (int)((nValue % 10000) / 100), (int)(nValue % 100));
                this.listResult.Items.Add(rowItem);
            }

            if (jObject.ContainsKey("Time"))
            {
                var aryTime = jObject["Time"].ToArray();
                this.listResult.Columns.Add("Time", 100, HorizontalAlignment.Left);
                for (int i = 0; i < aryTime.Count(); ++i)
                {
                    var item = aryTime[i];
                    ListViewItem rowItem = new ListViewItem();
                    int nValue = Convert.ToInt32(item);
                    string strValue = string.Format("{0:D2}:{1:D2}", (int)(nValue / 100), (int)(nValue % 100));
                    this.listResult.Items[i].SubItems.Add(strValue);
                }
            }

            var aryOutVar = jObject["OutVar"].ToArray();
            foreach (var item in aryOutVar)
            {
                var jsName = item["Name"].ToString();
                var jsData = item["Data"];
                if (jsData == null) continue;
                var aryJsData = jsData.ToArray();
                this.listResult.Columns.Add(jsName, 100, HorizontalAlignment.Left);
                for (var i = 0; i < aryJsData.Count(); ++i)
                {
                    JToken jsItem = aryJsData[i];
                    string strValue = "null";
                    if (jsItem.Type != JTokenType.Null)
                    {
                        strValue = jsItem.ToString();

                        /*
                        double dValue = double.Parse(strValue);
                        if (dValue>0)
                        {
                            Debug.WriteLine(string.Format("[BtnRun_Click] symbol={0}  {1}={2}", strSymbol, jsName, dValue));
                        }
                        */
                    }

                    this.listResult.Items[i].SubItems.Add(strValue);
                }
            }
        }
    }
}
