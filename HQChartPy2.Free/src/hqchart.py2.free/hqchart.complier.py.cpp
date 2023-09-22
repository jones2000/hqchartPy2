/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

*/

#include "hqchart.complier.py.h"
#include <Python.h>
#include "HQChart.OutVarToJson.h"

//json 读取
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filereadstream.h"
using namespace rapidjson;

#include "hqchart.complier.py.authorizeInfo.h"


namespace HQChart { namespace Complier { namespace Py {


bool LoadAuthorizeInfo(const std::string& strBase64Encrypted)
{
	return true;
}

bool AddCustomVariable(const std::string& strValue)
{
	if (strValue.empty()) return 0;

	std::wstring strName=UTF8ToWString(strValue);
	auto& customVar = CustomVariant::GetInstance();
	customVar.Add(strName);

	return 1;
}

void GetAuthorizeInfo(std::wstring& strValue)
{
	strValue = L"HQChartPy2社区版本";
}


bool AddSystemIndex(PyObject* pConfig)
{
	if (!pConfig || !PyUnicode_Check(pConfig))
	{
		TRACE_WARNING(L"[AddSystemIndex] PyUnicode_Check(pConfig) error.");
		return false;
	}

	const char* pStrConfig = PyUnicode_AsUTF8AndSize(pConfig, NULL);

	std::string strValue;
	rapidjson::Document root;
	root.Parse(pStrConfig);
	if (root.IsNull() || !root.IsArray()) return false;

	std::wstring strScript, strName, strDescription, strArgName;

	for (int i=0;i<(int)root.Size();++i)
	{
		const auto& index = root[i];
		if (!index.HasMember("Script") || !index["Script"].IsString())	//脚本
		{
			TRACE_WARNING(L"[AddSystemIndex] config[%d] 'Script' error.", i);
			continue;
		}

		if (!index.HasMember("Name") || !index["Name"].IsString())
		{
			TRACE_WARNING(L"[AddSystemIndex] config[%d] 'Name' error.", i);
			continue;
		}

		strValue = index["Script"].GetString();
		strScript = UTF8ToWString(strValue);

		strValue = index["Name"].GetString();
		strName = UTF8ToWString(strValue);

		strDescription.clear();
		if (index.HasMember("Description") && index["Description"].IsString())
		{
			strValue = index["Description"].GetString();
			strDescription = UTF8ToWString(strValue);
		}

		ScriptLibrary::ARRAY_ARGUMENT args;
		bool bArgsError = false;
		if (index.HasMember("Args") || index["Args"].IsArray())
		{
			const Value& jsArgs = index["Args"];
			double dValue = 0;
			for (int j = 0; j < (int)jsArgs.Size(); ++j)
			{
				const auto& item = jsArgs[j];
				strArgName = UTF8ToWString(item["Name"].GetString());
				dValue = item["Value"].GetDouble();

				HQChart::Complier::ARGUMENT_ITEM argItem;
				argItem._dValue = dValue;
				argItem._strName = strArgName;

				args.push_back(argItem);
			}
		}

		if (bArgsError)
		{
			continue;
		}

		if (args.empty())
		{
			ScriptLibrary::GetInstance().Add(strName, strScript, strDescription);
		}
		else
		{
			ScriptLibrary::GetInstance().Add(strName, strScript, args, strDescription);
		}
	}

	return true;
}

}
}
}