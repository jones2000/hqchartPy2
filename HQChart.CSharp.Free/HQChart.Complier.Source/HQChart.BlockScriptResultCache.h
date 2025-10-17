/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	批量指标计算缓存 (c++)
*/

#pragma once

#include "HQChart.data.h"


namespace HQChart { namespace Complier {



class BlockScriptResultCache
{
public:
	BlockScriptResultCache();
	~BlockScriptResultCache();
	
	static BlockScriptResultCache& GetInstance();

	BlockScriptResult* Get(const std::wstring& strKey);
	void Add(const std::wstring& strKey, BlockScriptResult* pData);

	void Clear();

private:
	std::map<std::wstring, BlockScriptResult*> m_mapBlockResult;

};


}
}