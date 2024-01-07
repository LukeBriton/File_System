#include "filesys.h"
#define CLEN 10
#define CNUM 15 
//enum ctype  
char commands[CNUM][CLEN]={
"exit",
"dir",
"mkdir",
"cd",
"mkfile",
"del",
"write",
"read",
"who",
"pwd",
"rename",
"password"
};
int getcid(char *command){
	int i;
	if(command == NULL) 
		return -1;
	for(i=0;i<CNUM;i++){
		if(strcmp(command,commands[i]) == 0)
			return i;
	}
	return -1;
}
int shell(int user_id,char *str){
	char seps[] =" \t\n\0";
	char* token,*tstr,*buf;
	char wd[2048];
	unsigned short mode;
	int fd;
	int cid,size;
	token = strtok(str,seps);
	if(token == NULL)
		return 1;
	cid = getcid(token);
	switch(cid){
	case 1:
		_dir();
		break;
	case 2:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkdir�������ȷ��ʽΪmkdir dirname����������!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd�������ȷ��ʽΪcd dirname����������!\n");
			break;
		}
		chdir(token);
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkfile �������ȷ��ʽΪmkfile filename [mode]����������!\n");
			break;
		}
		tstr =token;
		mode = DEFAULTMODE;
		token = strtok(NULL,seps);
		if(token != NULL){
			sscanf(token,"%o",&mode);
		}
		mode = mode|DIFILE|0700;
		fd = creat(user_id,tstr,mode);
		if(fd == -1){
			printf("�����ļ�ʧ�ܣ�\n");
			break;
		}
		//printf("%d %d\n", user_id, fd);/*�����þ�*/
		close(user_id,fd);
		break;
	case 5:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("del �������ȷ��ʽΪdel filename����������!\n");
			break;
		}
		delete(token);
		break;
	case 6:
		mode = WRITE;
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("write �������ȷ��ʽΪwrite filename bytes����������!\n");
			break;
		}
		if(token[0] == '-'){
			if(token[1] == 'a')
				mode = FAPPEND;
		}else{
			sscanf(token,"%d",&size);
		}
		fd = open(user_id,tstr,char(mode));
		if (fd == -1) {
			printf("�ļ������ڣ�\n");
			break;
		}
		//printf("%d %d\n", user_id, fd);/*�����þ�*/
		buf = (char*)malloc(size);
		size = write(fd,buf,size);
		printf("%d bytes have been written into the file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 7:
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("read �������ȷ��ʽΪread filename bytes����������!\n");
			break;
		}
		sscanf(token,"%d",&size);
		fd = open(user_id,tstr,READ);
		if (fd == -1) {
			printf("�ļ������ڣ�\n");
			break;
		}
		//printf("%d %d\n", user_id, fd);/*�����þ�*/
		buf = (char*)malloc(size+1);
		size = read(fd,buf,size);
		printf("%d bytes have been read into buf from the file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 8:
		who(user_id);
		break;
	case 9:
		_pwd(wd);
		printf("%s\n", wd);
		break;
	case 10:
		token = strtok(NULL, seps);
		tstr = token;
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("rename �������ȷ��ʽΪrename oldname newname����������!\n");
			break;
		}
		_rename(tstr, token);
		break;
	case 11:
		token = strtok(NULL, seps);
		tstr = token;
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("password �������ȷ��ʽΪpassword oldpw newpw����������!\n");
			break;
		}
		_password(user_id, tstr, token);
		break;

	case 0:
		return 0;
	default:
		printf("����:û������%s��\n",token);
		break;
	}
	return 1;
}
