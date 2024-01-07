#include <stdio.h> 
#include <string.h>
#include "filesys.h" 

int login(unsigned short uid, char *passwd){
	int i,j;

	for (i=0; i<PWDNUM; i++){
		if ((uid == pwd[i].p_uid) && (!strcmp(passwd, pwd[i].password))){
			for (j=0; j<USERNUM; j++){
				if (user[j].u_uid == 0) 
					break;
			}
 
			if (j == USERNUM){
				printf("\nToo many users in the system, fail to login!\n");
				return -1;
			}else{
				user[j].u_uid = uid;
				user[j].u_gid = pwd[i].p_gid; 
				user[j].u_default_mode = DEFAULTMODE;
			}
			break;
		}//if
	}//for
	if (i == PWDNUM){
		printf("\nIncorrect password!\n");
		return -1;
	}else{
		return j;
	}
}

int logout(unsigned short uid){
	int i,j,sys_no;
	struct inode *inode;

	for (i=0; i<USERNUM; i++){
		if (uid == user[i].u_uid)
			break;
	}

	if (i == USERNUM){
		printf("\nThere isn't such a user!\n");
		return 0;
	}

	for (j=0; j<NOFILE; j++){
		if (user[i].u_ofile[j] != SYSOPENFILE+1){
			/* iput the inode free the sys_ofile and clear the user_ofile */
			sys_no = user[i].u_ofile[j];
			inode = sys_ofile[sys_no].f_inode;
			iput(inode);
			sys_ofile[sys_no].f_count--;
			user[i].u_ofile[j] = SYSOPENFILE+1;
		}
	}
	return 1;
}

void who(unsigned short user_id)
{
	printf("user[%u].u_uid = %u\n", user_id, user[user_id].u_uid);
	printf("user[%u].u_gid = %u\n", user_id, user[user_id].u_gid);
	for (int i = 0; i < PWDNUM; i++) {
		if (user[user_id].u_uid == pwd[i].p_uid) {
			printf("pwd[%d].password = %s\n", i, pwd[i].password);
			break;
		}
	}
}

void _password(unsigned short user_id, char *oldpw, char *newpw)
{
	int i;
	for (i = 0; i < PWDNUM; i++) {
		if (user[user_id].u_uid == pwd[i].p_uid) {
			if(!strcmp(oldpw, pwd[i].password))
				break;
			else
			{
				printf("密码错误\n");
				return;
			}
		}
	}
	if (strlen(newpw) > PWDSIZ)
	{
		printf("新密码长度超过上限\n");
		return;
	}
	strcpy(pwd[i].password, newpw);
}
void adduser(unsigned short uid, unsigned short gid, char *password)
{
	if (strlen(password) > PWDSIZ)
	{
		printf("密码长度超过上限\n");
		return;
	}
	int i;
	for (i = 0; i < PWDNUM; i++) {
		if (pwd[i].p_uid == 0 && pwd[i].p_gid == 0)
		{
			pwd[i].p_uid = uid;
			pwd[i].p_gid = gid;
			strcpy(pwd[i].password, password);
			return;
		}
		else
		{
			if (pwd[i].p_uid == uid)
			{
				printf("用户ID重合\n");
				return;
			}
		}
	}
	if (i == PWDNUM)
	{
		printf("密码数量已满\n");
		return;
	}
}



























