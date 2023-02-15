#pragma once
class CDataParser
{
public:
	CDataParser();
	~CDataParser();
	static int ParseInt(CString str);
	static double ParseDouble(CString str);
	static void ParseIntArray(CArray<int>& arr, CString str);
	static void ParseDoubleArray(CArray<double>& arr, CString str);
	static void ParseIntList(CList<int>& list, CString str);
	static void ParseDoubleList(CList<double>& list, CString str);
};

