// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

#ifdef _DEBUG
#define KG_DEBUG_USE_DEBUG
#else
//#define KG_DEBUG_USE_RELEASE
#endif
#include <kg/debug.hpp>
#include <kg/windows/console.hpp>