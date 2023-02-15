#include "stdafx.h"
#include "DataParser.h"


CDataParser::CDataParser()
{
}


CDataParser::~CDataParser()
{
}


int CDataParser::ParseInt(CString str)
{
	return StrToInt(str);
}


double CDataParser::ParseDouble(CString str)
{
	return _tcstod(str,0);
}


void CDataParser::ParseIntArray(CArray<int>& arr, CString str)
{
	arr.RemoveAll();
	int length = str.GetLength(); 
	CString temp;
	for (int i = 0; i < length; i++)
	{
		if (str[i] >= '0'&&str[i] <= '9')
		{
			temp+=str[i];
		}
		else
		{
			if (!temp.IsEmpty())
			{
				arr.Add(ParseInt(temp));
				temp.Empty();
			}
		}
	}
	if (!temp.IsEmpty())
	{
		arr.Add(ParseInt(temp));
		temp.Empty();
	}
}


void CDataParser::ParseDoubleArray(CArray<double>& arr, CString str)
{
	arr.RemoveAll();
	int length = str.GetLength();
	CString temp;
	for (int i = 0; i < length; i++)
	{
		if (str[i] >= '0'&&str[i] <= '9'||str[i]=='.')
		{
			temp += str[i];
		}
		else
		{
			if (!temp.IsEmpty())
			{
				arr.Add(ParseDouble(temp));
				temp.Empty();
			}
		}
	}
	if (!temp.IsEmpty())
	{
		arr.Add(ParseDouble(temp));
		temp.Empty();
	}
}


void CDataParser::ParseIntList(CList<int>& list, CString str)
{
	list.RemoveAll();
	int length = str.GetLength();
	CString temp;
	for (int i = 0; i < length; i++)
	{
		if (str[i] >= '0'&&str[i] <= '9')
		{
			temp += str[i];
		}
		else
		{
			if (!temp.IsEmpty())
			{
				list.AddTail(ParseInt(temp));
				temp.Empty();
			}
		}
	}
	if (!temp.IsEmpty())
	{
		list.AddTail(ParseInt(temp));
		temp.Empty();
	}
}


void CDataParser::ParseDoubleList(CList<double>& list, CString str)
{
	list.RemoveAll();
	int length = str.GetLength();
	CString temp;
	for (int i = 0; i < length; i++)
	{
		if (str[i] >= '0'&&str[i] <= '9' || str[i] == '.')
		{
			temp += str[i];
		}
		else
		{
			if (!temp.IsEmpty())
			{
				list.AddTail(ParseDouble(temp));
				temp.Empty();
			}
		}
	}
	if (!temp.IsEmpty())
	{
		list.AddTail(ParseDouble(temp));
		temp.Empty();
	}
}