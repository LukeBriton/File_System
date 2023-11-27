#include <stdio.h>
#include "filesys.h"

static unsigned int block_buf[BLOCKSIZ];
/********************************************************** 
������balloc 
���ܣ�ά���������еĿ�������ջ������������ݿ飬���������� 
***********************************************************/
unsigned int balloc(){
	unsigned int free_block;
	int i;

	//���û�п����̿�
	if (filsys.s_nfree == 0){
		printf("\nDisk Full!!!\n");
		return DISKFULL;
	}
	free_block = filsys.s_free[filsys.s_pfree]; //ȡ��ջ�е��̿��
	if (filsys.s_pfree == NICFREE-1){		    //�����ջֻʣһ����
		memcpy(block_buf,disk+DATASTART+(free_block)*BLOCKSIZ,BLOCKSIZ);
		//���ж�ȡ��һ����
		for (i=0; i<NICFREE; i++)
			filsys.s_free[i] = block_buf[i];
		filsys.s_pfree = 0;				//���ö�ջָ��
	}else{//�����ջ�д���һ���̿�
		filsys.s_pfree++;				//�޸Ķ�ջָ��
	}
	filsys.s_nfree --;					//�޸��ܿ���
	filsys.s_fmod = SUPDATE;
	return free_block;	
}


void bfree(unsigned int block_num){
	int i;

	if (filsys.s_pfree == 0){//�����ջ����
		/*����ǰ��ջ�ڿ��д�뵱ǰ���*/
		for(i=0; i<NICFREE; i++)
			block_buf[i] = filsys.s_free[NICFREE-1-i];
		memcpy(disk+DATASTART+block_num*BLOCKSIZ,block_buf,BLOCKSIZ);
		filsys.s_pfree = NICFREE;//��ն�ջ
	}
	/*�޸Ķ�ջָ�룬������ǰ���ѹ���ջ*/
	filsys.s_pfree --;
	filsys.s_nfree++;
	filsys.s_free[filsys.s_pfree] = block_num;
	filsys.s_fmod = SUPDATE;
	return;
}
