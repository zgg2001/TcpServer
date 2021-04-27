#ifndef _CMD_H_
#define _CMD_H_

//ö�����ͼ�¼���� 
enum cmd
{
	CMD_LOGIN,//��¼ 
	CMD_LOGINRESULT,//��¼��� 
	CMD_LOGOUT,//�ǳ� 
	CMD_LOGOUTRESULT,//�ǳ���� 
	CMD_NEW_USER_JOIN,//���û����� 
	CMD_ERROR//���� 
};
//�������ݰ�ͷ 
struct DataHeader
{
	short cmd;//����
	short date_length;//���ݵĳ���	
};
//��1 ��¼ �����˺�������
struct Login : public DataHeader
{
	Login()//��ʼ����ͷ 
	{
		this->cmd = CMD_LOGIN;
		this->date_length = sizeof(Login);
	}
	char UserName[32];//�û��� 
	char PassWord[32];//���� 
};
//��2 ��¼��� ������
struct LoginResult : public DataHeader
{
	LoginResult()//��ʼ����ͷ 
	{
		this->cmd = CMD_LOGINRESULT;
		this->date_length = sizeof(LoginResult);
	}
	int Result;
};
//��3 �ǳ� �����û��� 
struct Logout : public DataHeader
{
	Logout()//��ʼ����ͷ 
	{
		this->cmd = CMD_LOGOUT;
		this->date_length = sizeof(Logout);
	}
	char UserName[32];//�û��� 
};
//��4 �ǳ���� ������
struct LogoutResult : public DataHeader
{
	LogoutResult()//��ʼ����ͷ 
	{
		this->cmd = CMD_LOGOUTRESULT;
		this->date_length = sizeof(LogoutResult);
	}
	int Result;
};
//��5 ���û����� ����ͨ�� 
struct NewUserJoin : public DataHeader
{
	NewUserJoin()//��ʼ����ͷ 
	{
		this->cmd = CMD_NEW_USER_JOIN;
		this->date_length = sizeof(NewUserJoin);
	}
	char UserName[32];//�û��� 
};

#endif
