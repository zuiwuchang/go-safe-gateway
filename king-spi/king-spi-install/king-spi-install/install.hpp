#ifndef KING_INSTALL_HEADER_HPP
#define KING_INSTALL_HEADER_HPP


#ifdef _WIN64
#define KING_SPI_DLL		"king-spi64.dll"
#define KING_INSTALL_KEY	L"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\King_Winsock_Spi64"
#define	KING_CATALOG_KEY	L"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries64"
#else
#define KING_SPI_DLL		"king-spi.dll"
#define KING_INSTALL_KEY	L"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\King_Winsock_Spi"
#define	KING_CATALOG_KEY	L"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries"
#endif
#define KING_INSTALL_NAME	L"Name"
#define KING_INSTALL_ITEM	L"Item"
#define KING_CATALOG_ITEM	L"PackedCatalogItem"


CREATE_CATEGORY(install,"install");

//安裝信息
struct install_info
{
	//原 基礎服務提供者 在註冊表中的 key 名稱
	std::wstring name;
	//原 基礎服務提供者 dll 全路徑
	std::wstring path;
};
struct PackedCatalogItem
{
	char Path[MAX_PATH];
	WSAPROTOCOL_INFO Info;
};
struct packed_catalog_item_t
{
	//在註冊表中的 key值
	std::wstring key;

	//PackedCatalogItem 值
	PackedCatalogItem item;
};
typedef boost::shared_ptr<packed_catalog_item_t> packed_catalog_item_spt;
typedef std::vector<packed_catalog_item_spt> packed_catalog_items_t;
//註冊表 安裝 卸載 封裝
class install_t
{
private:
	install_t();
	~install_t();
	class scoped_hkey
		:boost::noncopyable
	{
	private:
		HKEY _key;
	public:
		scoped_hkey(HKEY key):_key(key)
		{
		}
		~scoped_hkey()
		{
			if(_key)
			{
				RegCloseKey(_key);
			}
		}
	};
	//返回 原 基礎服務 提供者 key 和 PackedCatalogItem
	static void basic_provider(packed_catalog_items_t& items)
	{
		try
		{
			HKEY key;
			if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,KING_CATALOG_KEY,0,KEY_READ,&key))
			{
				BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open catalog key")));
			}
			scoped_hkey lKey(key);

			//enum
			DWORD pos = 0;
			wchar_t keyName[MAX_PATH];
			while(RegEnumKey(key,pos,keyName, MAX_PATH) == ERROR_SUCCESS)
			{
				packed_catalog_item_spt item = boost::make_shared<packed_catalog_item_t>();
				if(basic_provider(key,keyName,item->item))
				{
					item->key = keyName;
					items.push_back(item);
				}
				pos++;
			}
			if(items.empty())
			{
				BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad find basic_provider")));
			}
		}
		catch(const std::bad_alloc&)
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad alloc")));
		}
	}
	static bool basic_provider(HKEY key,wchar_t* keyName,PackedCatalogItem& item)
	{
		HKEY subKey; 
		if(ERROR_SUCCESS != RegOpenKeyEx(key,keyName,0,KEY_READ,&subKey))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open basic_provider")));
		}
		scoped_hkey lKey(subKey);

		DWORD len = sizeof(item);
		if(ERROR_SUCCESS != RegQueryValueEx(subKey,KING_CATALOG_ITEM,0,NULL,(LPBYTE)&item,&len) || len != sizeof(item))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad query basic_provider")));
		}

		WSAPROTOCOL_INFO& info = item.Info;
		if(info.ProtocolChain.ChainLen != 1 //基礎服務提供者
			|| info.iAddressFamily != AF_INET	//驗證 ip 
			//|| info.iSocketType != SOCK_STREAM	//tcp
			)
		{
			return false;
		}
		return true;
	}
	static void replace_spi(packed_catalog_items_t& items)
	{
		char fileName[MAX_PATH];
		GetModuleFileNameA(NULL,fileName,MAX_PATH);
		boost::filesystem::path path(fileName);
		path = path.parent_path();
		path /= KING_SPI_DLL;

		if(!boost::filesystem::exists(path))
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get(std::string(KING_SPI_DLL) + " not found")));
		}

		std::string str = path.string();

		try
		{
			BOOST_FOREACH(const packed_catalog_item_spt& node,items)
			{
				std::wstring keyName(KING_CATALOG_KEY);
				keyName += L"\\" + node->key;
				HKEY key;
				if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,keyName.c_str(),0,KEY_WRITE,&key))
				{
					RegCloseKey(key);
					BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open provider item")));
				}

				strcpy(node->item.Path,str.c_str());
				if(ERROR_SUCCESS != RegSetValueEx(key,KING_CATALOG_ITEM,0,REG_BINARY,(const BYTE*)&(node->item),sizeof(PackedCatalogItem)))
				{
					RegCloseKey(key);
					BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad replace provider item")));
				}

				RegCloseKey(key);
			}
		}
		catch(const boost::system::system_error& e)
		{
			uninstall();
			BOOST_THROW_EXCEPTION(e);
		}
	}
public:
	//安裝
	static void install()
	{
		//如果 已經 安裝 卸載
		uninstall();

		//返回 原 服務 提供者
		packed_catalog_items_t items;
		basic_provider(items);

		//創建 安裝 記錄信息
		HKEY key;
		if(ERROR_SUCCESS !=RegCreateKeyEx(HKEY_LOCAL_MACHINE
			,KING_INSTALL_KEY
			,0
			,NULL
			,REG_OPTION_VOLATILE
			,KEY_ALL_ACCESS
			,NULL
			,&key
			,NULL
			)
			)
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad create install key")));
		}
		scoped_hkey lKey(key);

		BOOST_FOREACH(const packed_catalog_item_spt& node,items)
		{
			const std::wstring& name = node->key;
			const PackedCatalogItem& item = node->item;

			//save item
			if(ERROR_SUCCESS != RegSetValueEx(key,name.c_str(),0,REG_BINARY,(const BYTE*)&item,sizeof(item)))
			{
				BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad save install path")));
			}
		}

		//替換 原始 spi dll
		replace_spi(items);
	}
	//卸載
	static void uninstall()
	{
		//沒有安裝 直接 返回
		packed_catalog_items_t items;
		if(!is_install(items,false))
		{
			return;
		}
		
		BOOST_FOREACH(const packed_catalog_item_spt& node,items)
		{
			
			std::string path = node->item.Path;
			if(path.empty())
			{
				BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad install record")));
			}

			//打開 原key
			std::wstring keyName = KING_CATALOG_KEY;
			keyName += L"\\" + node->key;
			HKEY key;
			if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,keyName.c_str(),0,KEY_ALL_ACCESS,&key))
			{
				BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad open provider key")));
			}
			scoped_hkey lKey(key);

			//恢復 key值
			if(ERROR_SUCCESS != RegSetValueEx(key,KING_CATALOG_ITEM,0,REG_BINARY,(const BYTE*)&(node->item),sizeof(PackedCatalogItem)))
			{
				BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad save provider item")));
			}
		}
		//刪除 安裝 記錄
		RegDeleteKey(HKEY_LOCAL_MACHINE,KING_INSTALL_KEY);
	}
	//返回 是否已經 安裝
	static bool is_install(packed_catalog_items_t& items,bool one = true)
	{
		bool ok = false;
		HKEY key;
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,KING_INSTALL_KEY,0,KEY_READ,&key))
		{
			return ok;
		}
		scoped_hkey lKey(key);
		try
		{
			//enum
			DWORD pos = 0;
			wchar_t keyName[MAX_PATH];
			while(true)
			{
				packed_catalog_item_spt item = boost::make_shared<packed_catalog_item_t>();
				DWORD len = MAX_PATH;
				DWORD type;
				DWORD size = sizeof(PackedCatalogItem);
				if(RegEnumValue(key,pos,keyName, &len,0,&type,(LPBYTE)&(item->item),&size) != ERROR_SUCCESS)
				{
					break;
				}
				if(type == REG_BINARY 
					&& sizeof(PackedCatalogItem) == size
					)
				{
					item->key = keyName;
					items.push_back(item);
					if(one)
					{
						break;
					}
				}
				pos++;
			}
			ok = !items.empty();
		}
		catch(const std::bad_alloc&)
		{
			BOOST_THROW_EXCEPTION(boost::system::system_error(1,install::get("bad alloc")));
		}
		return ok;
	}

};

#endif	//KING_INSTALL_HEADER_HPP