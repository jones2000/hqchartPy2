/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

日期计算 (c++)
*/

#pragma once

namespace HQChart { namespace Helper {


class CalendarHelper
{
public:

	static unsigned short GetDayCount(unsigned short wYear, unsigned short wMonth);				//得到某一个月的天数
	static bool IsLeapYear(unsigned short wYear);								//是否为润年
	static unsigned short CaculateWeekDay(unsigned short wYear, unsigned short wMonth, unsigned short wDay);	//得到某一天是星期几
	static bool IsDateValid(unsigned short wYear, unsigned short wMonth, unsigned short wDay); // 日期是否有效
	
	//农历算法相关
	static long GetLunarLastMonthDay(unsigned int kp_year);//获得农历最后一个月天数
	static unsigned char GetLeapMonth(unsigned int lunar_year);	//确定是否存在农历的闰月 并返回闰月
	static unsigned char GetLeapMonthDay(unsigned int lunar_year); //若存在闰月,返回闰月的天数,30?29  
	static unsigned int GetLunarYearTotal(unsigned int lunar_year); // 农历当年总天数,354?355 384 383
	static unsigned int IsLeapYear(unsigned int year);				//是否为闰年
	static unsigned char GetDay(unsigned int year ,unsigned char month); //判断当年当月天数
	static unsigned int GetSolarTotal(unsigned int solar_year, unsigned char solar_month); //计算1900.1.1 到  输入年月的天数   
	static unsigned char  ConvertSolarToLunar(unsigned int kp_year,unsigned char kp_month,unsigned char kp_day); /* 输入阳历时期 年  月  日 */ 
	static long GetLunarDate(unsigned int kp_year,unsigned char kp_month,unsigned char kp_day);

protected:

	//相关变量
	static unsigned int solar_year,lunar_year;  
	static unsigned char solar_month,lunar_month;  
	static unsigned int solar_day,lunar_day;
};

}}