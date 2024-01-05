#include "filesys.h" 

/**********************************************************************************
������access
�������û�ID�ţ��ڴ�ڵ㣻Ҫ�жϵ�Ȩ�� 
���ܣ�ͨ���û�Ȩ���ж��û��Ƿ��ܶԸ��ļ�������Ӧ�Ĳ�����Ȩ�޹�����1��Ȩ�޲�������0��
***********************************************************************************/
unsigned int access(unsigned int user_id,  struct inode *inode,  unsigned short mode){
	switch (mode){
		case READ:
			if (inode->di_mode & ODIREAD)												//�ļ��������û��ɶ���ͨ����
				return 1;
			if ((inode->di_mode & GDIREAD) && (user[user_id].u_gid == inode->di_gid))	//�ļ�����ɶ����û�����ļ���һ��ͨ����
				return 1;
			if ((inode->di_mode & UDIREAD) && (user[user_id].u_uid == inode->di_uid))	//�ļ�ָ���û��ɶ����Ǹ��û�ͨ����
				return 1;
			return 0;				//������ͨ����
		case WRITE:
			if (inode->di_mode & ODIWRITE)
				return 1;
			if ((inode->di_mode & GDIWRITE) && (user[user_id].u_gid == inode->di_gid))
				return 1;
			if ((inode->di_mode & UDIWRITE) && (user[user_id].u_uid == inode->di_uid))
				return 1;
			return 0;
		case EXECUTE:
			if (inode->di_mode & ODIEXECUTE)
				return 1;
			if ((inode->di_mode & GDIEXECUTE) && (user[user_id].u_gid == inode->di_gid))
				return 1;
			if ((inode->di_mode & UDIEXECUTE) && (user[user_id].u_uid == inode->di_uid))
				return 1;
			return 0;		
	}//swith
	return 0;
}




































