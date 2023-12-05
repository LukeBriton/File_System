#include <stdio.h>
#include "filesys.h"
/********************************************************************* 
������creat
���ܣ������ļ��������ҿ�д�򸲸ǣ���������i�ڵ㣬���򿪸��ļ��������ļ�ָ��
**********************************************************************/
int creat(unsigned int user_id, char *filename, unsigned short mode){
	struct inode *inode;
	int dirid,di_ith;
	int i,j;

	dirid = namei(filename);
	if (dirid != -1){//�������ͬ���ļ�/Ŀ¼ 
		inode = iget(dir.direct[dirid].d_ino);
		if(!(inode->di_mode&DIFILE)){//��������ļ�
			printf("����ͬ��Ŀ¼��\n");
		}
		if (access(user_id,inode,WRITE) == 0){
			iput(inode);
			printf("\n No access to creating the file!!!\n");
			return -1;
		}
		j = inode->di_size%512?1:0;
		for (i=0; i<inode->di_size/BLOCKSIZ+j; i++)
			bfree(inode->di_addr[i]);

		for (i=0; i<SYSOPENFILE; i++){
			if (sys_ofile[i].f_count != 0 && sys_ofile[i].f_inode == inode){
				sys_ofile[i].f_off = 0;
			}
		}
		iput(inode);
		return open(user_id,filename,WRITE);
	}else{
		inode = ialloc();
		di_ith = iname(filename);

		dir.size++; 
		dir.direct[di_ith].d_ino = inode->i_ino;
		inode->di_mode = mode;
		inode->di_uid = user[user_id].u_uid;
		inode->di_gid = user[user_id].u_gid; 
		inode->di_size = 0;
		inode->di_number = 1;			//liwen change to 1
		iput(inode);
		return open(user_id,filename,WRITE);
	}
	return 0;
}








