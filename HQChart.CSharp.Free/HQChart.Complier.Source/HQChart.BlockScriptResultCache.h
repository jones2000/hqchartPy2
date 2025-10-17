/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	��Դ��Ŀ https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	����ָ����㻺�� (c++)
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