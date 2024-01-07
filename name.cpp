#include <stdio.h> 
#include <string.h>
#include "filesys.h" 
/* 
namei函数：参数：要查找的文件名。返回文件对应的内存目录项标号i；如果返回-1表示没有该文件。
功能：查找文件在当前目录下对应的内存目录项的号； 
*/
int namei(char *name){
	int  i;

	for (i=0; i<DIRNUM; i++){
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))//名字匹配且目录项有效；
		{
			return i;	//liwen change it ,userful in delete
		}
	}
	return -1;  //not found
}
/* 
iname函数：功能：如果还有空目录项，为当前文件分配目录项；
           参数：将要分配的文件名。
		   返回：0没有空闲目录项；i分配的目录项号。
*/
unsigned short iname(char *name){
	int i, notfound = 1;

	for (i=0; ((i<DIRNUM) && (notfound)); i++){
		if (dir.direct[i].d_ino == 0)//该目录项未分配。
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

void _rename(char *oldname, char *newname)//new 是保留字
{
	int i = namei(oldname);
	if (i == -1)
	{
		printf("当前目录下无 %s\n", oldname);
		return;
	}
	int j = namei(newname);
	if (j != -1)
	{
		printf("当前目录下已有与 %s 重名的\n", newname);
		return;
	}
	if (strlen(newname) > DIRSIZ)
	{
		printf("新命名长度超过上限\n", newname);
		return;
	}
	strcpy(dir.direct[i].d_name, newname);
	return;
}








































