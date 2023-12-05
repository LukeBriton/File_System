#include <stdio.h>
#include "filesys.h" 

void delete(char *filename){
	unsigned int dirid;
	struct inode *inode;

	dirid = namei(filename);
	if (dirid == -1){
		printf("�ļ������ڣ�����!\n"); 
		return;
	}
	inode = iget(dir.direct[dirid].d_ino);
	if(!(inode->di_mode &DIFILE)){
		printf("���ж������Ͳ����ļ������飡\n");
		iput(inode);
		return;
	}
	dir.direct[dirid].d_ino = DIEMPTY;
	dir.size --;
	inode->di_number--;
	iput(inode);
	return;
}











