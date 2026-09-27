#ifndef DATA_CPP_DEFINED
#define DATA_CPP_DEFINED

#include "Data.h"

void Data::Init(Cmd::Type cmd, std::vector<void*> data, std::vector<Type> type)
{
	mCmd = cmd;
	m_vType = type;

	int currentData = 0;

	int buf_len = 1 + 4; //cmd + size data
	int var_len = 0;
	char* buf = nullptr;

	for (size_t i = 0; i < data.size(); i++)
	{
		switch (m_vType[currentData])
		{
		case TYPE_INT1:
			var_len += 1;
			break;
		case TYPE_INT4:
			var_len += 4;
			break;
		case TYPE_XMFLOAT3:
			var_len += 12;
			break;
		case TYPE_XMFLOAT4:
			var_len += 16;
			break;
		case TYPE_BOOL:
			var_len += 1;
			break;
		case TYPE_FLOAT:
			var_len += 4;
			break;
		case TYPE_PTR:
			var_len += 4;
			break;
		default:
			break;
		}
		currentData++;
	}

	buf_len += var_len;
	buf = new char[buf_len];
	int offset = 1;
	memcpy(buf + offset, &var_len, 4);
	offset += 4;

	for (size_t i = 0; i < currentData; i++)
	{
		switch (m_vType[i])
		{
		case TYPE_INT1:
			memcpy(buf + offset, data[i], 1);
			offset += 1;
			break;
		case TYPE_INT4:
			memcpy(buf + offset, data[i], 4);
			offset += 4;
			break;
		case TYPE_XMFLOAT3:
			memcpy(buf + offset, data[i], 12);
			offset += 12;
			break;
		case TYPE_XMFLOAT4:
			memcpy(buf + offset, data[i], 16);
			offset += 16;
			break;
		case TYPE_BOOL:
			memcpy(buf + offset, data[i], 1);
			offset += 1;
			break;
		case TYPE_FLOAT:
			memcpy(buf + offset, data[i], 4);
			offset += 4;
			break;
		default:
			break;
		}
	}	

	int cmd8 = cmd;

	memcpy(buf, &cmd8, 1);

	mByte = buf;

	mSize = buf_len;
}

#endif