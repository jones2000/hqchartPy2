/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	股票数据对外接口类 (c++)
*/

#include <fstream>
#include "HQChart.BlockScriptResultCache.h"


namespace HQChart { namespace Complier {

BlockScriptResultCache::BlockScriptResultCache()
{

}

BlockScriptResultCache::~BlockScriptResultCache()
{
	Clear();
}

BlockScriptResultCache& BlockScriptResultCache::GetInstance()
{
	static BlockScriptResultCache instance;
	return instance;
}

BlockScriptResult* BlockScriptResultCache::Get(const std::wstring& strKey)
{
	auto iter = m_mapBlockResult.find(strKey);
	if (iter == m_mapBlockResult.end()) return NULL;

	return iter->second;
}

void BlockScriptResultCache::Add(const std::wstring& strKey, BlockScriptResult* pData)
{
	//TODO:如果存在可能会泄漏
	auto iter = m_mapBlockResult.find(strKey);

	m_mapBlockResult[strKey] = pData;
}

void BlockScriptResultCache::Clear()
{
	for (auto iter : m_mapBlockResult)
	{
		DELETE_PTR(iter.second);
	}

	m_mapBlockResult.clear();
}

}
}