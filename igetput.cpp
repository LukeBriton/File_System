#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesys.h" 

/* ��������	iget 							*/
/* ���ã�	Ϊ����i�������Ӧ���ڴ�i���	*/
/* ����:	������Ĵ���i����				*/
/* ����ֵ��	ָ���Ӧ���ڴ�i����ָ��		*/ 
struct inode * iget(unsigned int dinodeid)
{
	int existed = 0, inodeid;
	long addr;
	struct inode *temp, *newinode;


	int i;


	inodeid = dinodeid % NHINO;//�����ڴ���Ӧ���ڵڼ�����ϣ������
	if (hinode[inodeid].i_forw == NULL)//���ù�ϣ����Ϊ�գ��ڴ���һ��δ������
		existed = 0;
	else//����Ϊ�գ��Ӹù�ϣ����ͷ��ʼ����
	{
		temp = hinode[inodeid].i_forw; 
		while (temp)
		{
			if (temp->i_ino == dinodeid)//���ҵ�
			{
				existed = 1;
				temp->i_count ++;
				return temp;//���ظ��ڴ���ָ��
			}
			else
				temp = temp->i_forw;

		}
	}

	/* ��û���ҵ� */   
	/* 1. ����ô���i������ļ����е�λ�� */
	addr = DINODESTART + dinodeid * DINODESIZ;

	/* 2. ����һ���ڴ�i��� */
	newinode = (struct inode *)malloc(sizeof(struct inode));

	/* 3. �ô���i����ʼ���ڴ�i��� */
	memcpy(&(newinode->di_number), disk+addr, DINODESIZ);

	/* 4. ���ڴ�i���������Ӧ�Ĺ�ϣ������*/
	newinode->i_forw = hinode[inodeid].i_forw;
	hinode[inodeid].i_forw = newinode; 
	newinode->i_back = newinode;
	if (newinode->i_forw)
		newinode->i_forw->i_back = newinode;

	/*5. ��ʼ���ڴ�i�������������� */
	newinode->i_count = 1;
	newinode->i_flag = 0;   /* ��ʾδ���� */
	newinode->i_ino = dinodeid;

	return newinode;
}

/* ��������	iput							*/
/* ���ã�	�����ڴ�i���					*/
/* ����:	ָ������յ��ڴ�i���ָ��		*/
/* ����ֵ��	��								*/ 
void iput(struct inode *pinode)
{
	long addr;
	unsigned int block_num;
	int i;

	if (pinode->i_count > 1)//�����ü���>1
	{
		pinode->i_count --;

		return;
	}
	else
	{
		if (pinode->di_number != 0)//�����������Ϊ0
		{
			/* ���ڴ�i��������д�ش���i��� */
			addr = DINODESTART + pinode->i_ino *DINODESIZ;
			memcpy(disk+addr, &pinode->di_number, DINODESIZ);
		}
		else
		{
			/* ɾ������i�����ļ���Ӧ������� */
			block_num = pinode->di_size/BLOCKSIZ;
			block_num += (pinode->di_size % BLOCKSIZ) ? 1 : 0;
			for (i = 0; i < block_num; i++)
				bfree(pinode->di_addr[i]);
			ifree(pinode->i_ino);
		}

		/* �ͷ��ڴ�i��� */
		{
			int inodeid;
			inodeid = (pinode->i_ino)  % NHINO ;//�ҵ����ڵĹ�ϣ����

			/* �Ӹù�ϣ������ɾ�� */
			if (hinode[inodeid].i_forw == pinode)
			{
				hinode[inodeid].i_forw = pinode->i_forw;
				if (pinode->i_forw)
					pinode->i_forw->i_back = pinode->i_forw;
			}
			else
			{
				pinode->i_back->i_forw = pinode->i_forw;
				if (pinode->i_forw)
					pinode->i_forw->i_back = pinode->i_back;
			}
		}
		free(pinode);
			
	}

	return;
}













