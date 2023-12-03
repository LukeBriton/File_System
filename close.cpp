#include <stdio.h>
#include "filesys.h"

/****************************************************** 
������close
���ܣ��رմ��ļ�ʱ���޸��û��򿪱��ϵͳ�򿪱��еļ�¼��Ϣ 
*******************************************************/
void close(unsigned int user_id, int cfd){
	struct inode *inode;
	inode = sys_ofile[user[user_id].u_ofile[cfd]].f_inode;
	iput(inode);
	sys_ofile[user[user_id].u_ofile[cfd]].f_count -- ;
	user[user_id].u_ofile[cfd] = SYSOPENFILE+1;
	return;
}
