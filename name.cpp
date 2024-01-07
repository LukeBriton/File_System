#include <stdio.h> 
#include <string.h>
#include "filesys.h" 
/* 
namei������������Ҫ���ҵ��ļ����������ļ���Ӧ���ڴ�Ŀ¼����i���������-1��ʾû�и��ļ���
���ܣ������ļ��ڵ�ǰĿ¼�¶�Ӧ���ڴ�Ŀ¼��ĺţ� 
*/
int namei(char *name){
	int  i;

	for (i=0; i<DIRNUM; i++){
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))//����ƥ����Ŀ¼����Ч��
		{
			return i;	//liwen change it ,userful in delete
		}
	}
	return -1;  //not found
}
/* 
iname���������ܣ�������п�Ŀ¼�Ϊ��ǰ�ļ�����Ŀ¼�
           ��������Ҫ������ļ�����
		   ���أ�0û�п���Ŀ¼�i�����Ŀ¼��š�
*/
unsigned short iname(char *name){
	int i, notfound = 1;

	for (i=0; ((i<DIRNUM) && (notfound)); i++){
		if (dir.direct[i].d_ino == 0)//��Ŀ¼��δ���䡣
		{
			notfound = 0;
			break;
		}
	}

	if (notfound){
		printf("\nThe current directory is full!!!\n");
		return 0;
	}
	strcpy( dir.direct[i].d_name, name); 
	return i; 
}

void _rename(char *oldname, char *newname)//new �Ǳ�����
{
	int i = namei(oldname);
	if (i == -1)
	{
		printf("��ǰĿ¼���� %s\n", oldname);
		return;
	}
	int j = namei(newname);
	if (j != -1)
	{
		printf("��ǰĿ¼�������� %s ������\n", newname);
		return;
	}
	if (strlen(newname) > DIRSIZ)
	{
		printf("���������ȳ�������\n", newname);
		return;
	}
	strcpy(dir.direct[i].d_name, newname);
	return;
}








































