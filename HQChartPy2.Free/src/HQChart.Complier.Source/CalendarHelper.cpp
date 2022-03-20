/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

日期计算 (c++)
*/

#include "CalendarHelper.h"

namespace HQChart { namespace Helper {

//相关变量
unsigned int CalendarHelper::solar_year = 0;
unsigned int CalendarHelper::lunar_year = 0;

unsigned char CalendarHelper::solar_month = 0;
unsigned char CalendarHelper::lunar_month = 0;

unsigned int CalendarHelper::solar_day = 0;
unsigned int CalendarHelper::lunar_day = 0;

//不同农历的数据

/*1900-2051年的农历数据 
数据格式说明： 
5位十六进制数字 例:04bd8 
  1    位: 1表示闰月30天，0表示29天 
2、3、4位: 转换二进制为:0100 1011 1101(1为30天，0为29天) 
           04bd8表示为(13个月):29,30,29,29,30,29,30,30,30(闰8月),30,30,29,30; 
  5    位: 如果有闰月，则为月份，没有则为0*/  
unsigned long int aryLunarInfo[]=  
{  
    0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,  
    0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,  
    0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,  
    0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,  
    0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,  
    0x06ca0,0x0b550,0x15355,0x04da0,0x0a5b0,0x14573,0x052b0,0x0a9a8,0x0e950,0x06aa0,  
    0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,  
    0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b6a0,0x195a6,  
    0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,  //1980   
    0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,  
    0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,  //2004 07552   
    0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,  
    0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530, //2028   
    0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,  
    0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,  
    0x14b63,0x09370  
};  
  
//////////////////////////////////////////////////////////////////////////////////
const unsigned short	MONTH_DAY_COUNT[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };						//每个月份天数



//计算节气的公式[Y*D+C]- L
//年份的后2位*D值+世纪值C - 闰年数L
//定义一个二维数组，第一维数组存储的是20世纪的节气C值，第二维数组存储的是21世纪的节气C值,共24个
const double aryCenturyValue[2][24] = 
{{4.6295,19.4599,6.3826,21.4155,5.59,20.888,6.318,21.86,6.5,22.2,7.928,23.65,8.35, 23.95,8.44,23.822,9.098,24.218,8.218,23.08,7.9,22.6,6.11,20.84},
{3.87,18.73,5.63,20.646,4.81,20.1,5.52,21.04,5.678,21.37,7.108,22.83, 7.5,23.13,7.646,23.042,8.318,23.438,7.438,22.36,7.18,21.94,5.4055,20.12}}; 


//节日结构体
typedef struct tagFestivalItem
{
	long lDate;			//日期 月日
	wchar_t szText[8];	//节日名称
}FESTIVAL_ITEM,*PFESTIVAL_ITEM;


bool CalendarHelper::IsLeapYear(unsigned short wYear)
{ 
	return !(wYear%4)&&(wYear%100) || !(wYear%400); 
}

unsigned short CalendarHelper::GetDayCount(unsigned short wYear, unsigned short wMonth)
{
	if(wMonth > 12 || wMonth < 1) return 0;

	if(wMonth == 2)
	{
		if(IsLeapYear(wYear))
			return 29;
		else
			return 28;
	}

	return MONTH_DAY_COUNT[wMonth];
}

unsigned short CalendarHelper::CaculateWeekDay(unsigned short wYear, unsigned short wMonth, unsigned short wDay)
{
	//基姆拉尔森计算公式
	if(wMonth==1 || wMonth==2) 
	{
		wMonth+=12;
		wYear--;
	}
	
	unsigned short wWeek = (wDay + 2 * wMonth + 3 * (wMonth + 1) / 5 + wYear + wYear / 4 - wYear / 100 + wYear / 400) % 7;
	return wWeek;
}


bool CalendarHelper::IsDateValid(unsigned short wYear, unsigned short wMonth, unsigned short wDay)
{
	if (wYear > 1800 && wYear < 9999 && wMonth >= 1 && wMonth <= 12 && wDay >= 1 && wDay <= 31)
		return true;
	return false;
}


unsigned char CalendarHelper::GetLeapMonth(unsigned int lunar_year)    
{   
	return aryLunarInfo[lunar_year-1900]&0xf;
}  
//---------------------------------------------   
unsigned char CalendarHelper::GetLeapMonthDay(unsigned int lunar_year)   
{   
	if(GetLeapMonth(lunar_year))  
		return(  ( (aryLunarInfo[lunar_year-1900]) & 0x10000 ) ? 30:29 );  
	else  
		return(0);  
}  
//---------------------------------------------   
unsigned char GetLunarMonthTotal(unsigned int lunar_year, unsigned char lunar_month) 
{  
	return( (aryLunarInfo[lunar_year-1900] & (0x10000>>lunar_month) ) ? 30:29 );  
}   
//---------------------------------------------   
unsigned int CalendarHelper::GetLunarYearTotal(unsigned int lunar_year) 
{   
	/*sum=12*29 */   //12个月 29天一月   
	unsigned int sum=348;  
	unsigned int i;  
	for(i=0x8000;i>0x8; i>>=1)   
		sum+=(aryLunarInfo[lunar_year-1900]&i)?1:0;  //把大月的1加进去   
	return(sum+GetLeapMonthDay(lunar_year));    //判断这年是否有闰月   
}  
//---------------------------------------------   
//#############################################   

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   
//---------------------------------------------   
unsigned int CalendarHelper::IsLeapYear(unsigned int year)  //判断是否为闰年   
{   
	if( (year%4==0 &&year%100) || year%400==0 )  
		return 366;   
	else  
		return 365;   
}  

//--------------------------------------   
unsigned char CalendarHelper::GetDay(unsigned int year ,unsigned char month) //判断当年当月天数   
{   
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)  
		return 31;  
	if(month==4||month==6||month==9||month==11)  
		return 30;   
	if(month==2&&IsLeapYear(year)==366)  
		return 29;  
	else  
		return 28;   
}  

unsigned int CalendarHelper::GetSolarTotal(unsigned int solar_year, unsigned char solar_month)  
{   
	unsigned int total;  
	unsigned int temp_num;  
	total=0;  
	for(temp_num=1900;temp_num<solar_year;temp_num++)  
		total+=IsLeapYear(temp_num);  
	for(temp_num=1;temp_num<solar_month;temp_num++)  
		total+=GetDay(solar_year,temp_num);  
	return total;  
}

unsigned char  CalendarHelper::ConvertSolarToLunar(unsigned int kp_year,unsigned char kp_month,unsigned char kp_day) 
{   
	unsigned int total_day=0;/*记录农历1900.1.1日到今天相隔的天数 */  
	unsigned char run_yue_flag=0,run_yue=0,year_flag=0;  

	if(kp_year<1900 || kp_year>2051 || kp_month>12 || kp_month==0 || (kp_year==1900 && kp_month==1) )  
		return 0;  
	if(kp_day>GetDay(kp_year,kp_month) || kp_day==0)  
		return 0;  

	total_day=GetSolarTotal( kp_year, kp_month)+kp_day-30; /* 阳历从1900.1.31(农历1900.1.1)到今天的总天数(减30 实际少了一天)。 */  
	lunar_year=1900;  
	while(total_day>385) //385大于一年  留出一年多的时间用于条件计算   
	{  
		total_day-=GetLunarYearTotal(lunar_year); //   
		lunar_year++;  
	}  
	if(total_day>GetLunarYearTotal(lunar_year))  //排除lunar_year有闰月的情况   
	{  
		total_day-=GetLunarYearTotal(lunar_year);  
		lunar_year++;  

	}  
	run_yue=GetLeapMonth(lunar_year);  //当前闰哪个月   
	if(run_yue)  
		run_yue_flag=1; //有闰月则一年为13个月   
	else  
		run_yue_flag=0;  //没闰月则一年为12个月   

	if(total_day==0)   //刚好一年   
	{  
		lunar_day=GetLunarMonthTotal(lunar_year,12);  
		lunar_month=12;  
	}  
	else  
	{  
		lunar_month=1;  
		while(lunar_month<=12)  
		{  
			if( run_yue_flag==1 && lunar_month==(run_yue+1) )  //闰月处理   
			{  
				if(total_day>GetLeapMonthDay(lunar_year))  
				{  
					total_day-=GetLeapMonthDay(lunar_year);  //该年闰月天数
					run_yue_flag=0;  
					continue;
				}
				else
				{	
					lunar_month = run_yue;
					lunar_day=total_day;
					break;
				}
			}

			if( total_day> GetLunarMonthTotal(lunar_year,lunar_month ) )  
			{  
				total_day=total_day-GetLunarMonthTotal(lunar_year,lunar_month);  //该年该月天数   
				lunar_month++;  
			}  
			else  
			{  
				lunar_day=total_day;  
				break;  
			}  
		}
	}

	return 1;
}

//根据阳历日期获得对应阴历的日期
long CalendarHelper::GetLunarDate(unsigned int kp_year,unsigned char kp_month,unsigned char kp_day)
{
	if (ConvertSolarToLunar(kp_year,kp_month,kp_day) > 0)
		return (int)lunar_day + (int)lunar_month*100 + (int)lunar_year*10000;
	else
		return 0;
}

//获得阴历最后一个月是几日
long CalendarHelper::GetLunarLastMonthDay(unsigned int kp_year) 
{
	return GetLunarMonthTotal(kp_year,12);
}


}}
