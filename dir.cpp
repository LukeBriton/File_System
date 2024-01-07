#include <stdio.h>
#include <string.h>
#include "filesys.h" 

void _dir(){
	unsigned int di_mode;
	int i,j,k;          //xiao   
	struct inode *temp_inode;

	printf("\n CURRENT DIRECTORY :%s\n",dir.direct[1].d_name); 
	printf("当前共有%d个文件/目录\n",dir.size);
	for (i=0; i<DIRNUM; i++){
		if (dir.direct[i].d_ino != DIEMPTY){
			printf("%-14s", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			di_mode = temp_inode->di_mode & 00777;			 
			for (j=0; j<9; j++){
				if (di_mode%2){
					printf("x");
				}else{
					printf("-");
				}
				di_mode = di_mode/2;
			}
			printf("\ti_ino->%d\t",temp_inode->i_ino);
			if (temp_inode->di_mode & DIFILE){
				printf(" %ld ", temp_inode->di_size);
				printf("block chain:");
				j=(temp_inode->di_size%BLOCKSIZ)?1:0;
				for (k=0; k < temp_inode->di_size/BLOCKSIZ+j; k++)
					printf("%4d", temp_inode->di_addr[k]);
				printf("\n");
			}else{
				printf("<dir>\n");
			}//else
			iput(temp_inode);
		}// if (dir.direct[i].d_ino != DIEMPTY) 
	}//for 
	return;
}
void mkdir(char *dirname){
	int dirid, dirpos;
	struct inode *inode;
	struct direct buf[BLOCKSIZ/(DIRSIZ+4)];
	unsigned int block;
	if (strlen(dirname) > DIRSIZ)
	{
		printf("命名长度超过上限\n", dirname);
		return;
	}
	dirid= namei(dirname);
	if (dirid != -1){
		inode = iget(dirid);
		if (inode->di_mode & DIDIR)
			printf("目录%s已存在！\n", dirname); //xiao
		else
			printf("已有%s的类型不是目录！\n", dirname);
		iput(inode);
		return;
	}
	dirpos = iname(dirname);					//取得在addr中的空闲项位置,并将目录名写到此项里
	inode = ialloc();							//分配i节点
	dir.direct[dirpos].d_ino = inode->i_ino;	//设置该目录的磁盘i节点号
	dir.size++;									//目录数++		
	
	strcpy(buf[0].d_name,"..");					//子目录的上一层目录 当前目录
	buf[0].d_ino = cur_path_inode->i_ino;
	strcpy(buf[1].d_name, ".");
	buf[1].d_ino = inode->i_ino;				//子目录的本目录 子目录

	block = balloc();
	memcpy(disk+DATASTART+block*BLOCKSIZ, buf, BLOCKSIZ);

	inode->di_size = 2*(DIRSIZ+4);
	inode->di_number = 1; 
	inode->di_mode = user[user_id].u_default_mode|DIDIR;
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;

	iput(inode);
	return;
}


void chdir(char *dirname){
	unsigned int dirid;
	struct inode *inode;
	unsigned short block;
	int i,j,low=0, high=0;

	dirid = namei(dirname);
	if (dirid == -1){
		printf("不存在目录%s！\n", dirname);
		return;
	}	
	inode =iget(dir.direct[dirid].d_ino);
	if(!(inode->di_mode&DIDIR)){
		printf("%s的类型不是目录！\n", dirname);
		return;
	}
	for (i=0; i<dir.size; i++){
		if(dir.direct[i].d_ino == 0){
			for(j=DIRNUM-1;j>i&&dir.direct[j].d_ino == 0;j--);
			memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ+4);  //xiao
			dir.direct[j].d_ino = 0;
		}		
	}
	j = cur_path_inode->di_size%BLOCKSIZ?1:0;
	for (i=0; i<cur_path_inode->di_size/BLOCKSIZ+j; i++){
		bfree(cur_path_inode->di_addr[i]);
	}
	for (i=0; i<dir.size; i+=BLOCKSIZ/(DIRSIZ+4)){
		block = balloc();
		cur_path_inode->di_addr[i] = block;
		memcpy(disk+DATASTART+block*BLOCKSIZ, &dir.direct[i], BLOCKSIZ);
	}
	cur_path_inode->di_size = dir.size*(DIRSIZ+4);
	iput(cur_path_inode);
	cur_path_inode = inode;

	j=0;
	for (i=0; i<inode->di_size/BLOCKSIZ+1; i++){
		memcpy(&dir.direct[j],disk+DATASTART+inode->di_addr[i]*BLOCKSIZ, BLOCKSIZ);
		//printf("inode->di_addr[%d]:%u\n", i, inode->di_addr[i]);
		j+=BLOCKSIZ/(DIRSIZ+4);
	}
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);
	for (i=dir.size; i<DIRNUM; i++){ 
		dir.direct[i].d_ino = 0;
	}
	
	//end by xiao

	return;  
} 

void _pwd(char *wd)
{
	/*
	* 类似于：
	* cd .	a
	* cd ..	b
	* ...	
	* /
	* Path: /.../b/a	（通过）
	*/

	/*
	* 我们首先可以知道dir.direct[1].d_ino、cur_path_inode->i_ino表记了该级direct的inode号
	* 想要知道该级direct的name，理论上需要跑到上一级的dir里根据对应的inode号找对应direct知其name
	* 我们的想法是将低一级的inode号与上一级的dir（理论上只需要其中的一项即可）结合起来
	* 加载上一级的诸direct到ptr时需要一点说的道理（disk+DATASTART+inode->di_addr[i]*BLOCKSIZ中的inode->di_addr[i]）
	* 之后得到以上各级的name，在temp_inode->di_addr[0] == 0的根目录处终止
	* 要想办法把得到的name逆过来
	*/
	//unsigned int path[128];//记录每一级的d_ino编号
	unsigned int cur_inode = 0;
	int path_size = 0;
	struct direct* ptr;
	struct inode* temp_inode;
	char abs_path[2048] = "/";//128*16

	//ptr = (struct direct*)malloc(sizeof(BLOCKSIZ));		//会出错，可能因为没对齐
	//ptr = (struct direct*)malloc(sizeof(struct direct));	//与之对应memcpy也只能sizeof(struct direct)
	//ptr = (struct direct*)malloc(BLOCKSIZ / (DIRSIZ + 4)*sizeof(struct direct));	//如果表项数达到了DIRNUM...
	ptr = (struct direct*)malloc(DIRNUM * sizeof(struct direct));

	if (ptr == NULL) {
		printf("ptr malloc failure\n");
		return;
	}
	//复制出上一级目录的目录项，为的是得到d_ino编号//ptr前不加&!!!
	//memcpy(ptr, disk + DATASTART + cur_path_inode->di_addr[0] * BLOCKSIZ, sizeof(struct direct)); BLOCKSIZ会有概率出错

	//path[0] = dir.direct[1].d_ino;	//cd .	该级// dir.direct[1].d_ino;（第二个目录项）也即 cur_path_inode->i_ino;
	//path[1] = dir.direct[0].d_ino;	//cd .. 父级// dir.direct[0].d_ino;（第一个目录项）

	cur_inode = dir.direct[1].d_ino;
	temp_inode = iget(dir.direct[0].d_ino);

	/* 只考虑了占了一块的情况
	int j = 0;
	memcpy(ptr, disk + DATASTART + temp_inode->di_addr[0] * BLOCKSIZ, BLOCKSIZ);
	//memcpy(ptr, disk + DATASTART + temp_inode->di_addr[0] * BLOCKSIZ, sizeof(struct direct));
	printf("temp_inode->di_addr[%d]:%u\n", 0, temp_inode->di_addr[0]);
	*/

	int i = 0;
	int j = 0;
	for (i = 0; i < temp_inode->di_size / BLOCKSIZ + 1; i++) {
		memcpy(&ptr[j], disk + DATASTART + temp_inode->di_addr[i] * BLOCKSIZ, BLOCKSIZ);
		//printf("inode->di_addr[%d]:%u\n", i, temp_inode->di_addr[i]);
		j += BLOCKSIZ / (DIRSIZ + 4);
	}

	for (i = 0; i < DIRNUM; i++) {
		if (ptr[i].d_ino == cur_inode)
			break;
	}

	//printf("ptr[%d].d_name:%s\n", i, ptr[i].d_name);
	//printf("temp_inode->di_addr[0]:%d\n", temp_inode->di_addr[0]);
	if(strcmp(ptr[i].d_name, ".."))
	//if(temp_inode->di_addr[0])
	//if (ptr[0].d_ino != ptr[1].d_ino)
	{
		strcat(abs_path, ptr[i].d_name);
		//printf("%s\n", abs_path);
	}
	while (temp_inode->di_addr[0])
	{
		iput(temp_inode);
		cur_inode = ptr[1].d_ino;
		temp_inode = iget(ptr[0].d_ino);
		free(ptr);
		ptr = NULL;
		ptr = (struct direct*)malloc(DIRNUM * sizeof(struct direct));
		if (ptr == NULL) {
			printf("ptr malloc failure\n");
			return;
		}
		i = 0;
		j = 0;
		for (i = 0; i < temp_inode->di_size / BLOCKSIZ + 1; i++) {
			memcpy(&ptr[j], disk + DATASTART + temp_inode->di_addr[i] * BLOCKSIZ, BLOCKSIZ);
			j += BLOCKSIZ / (DIRSIZ + 4);
		}
		for (i = 0; i < DIRNUM; i++) {
			if (ptr[i].d_ino == cur_inode)
				break;
		}
		//printf("ptr[%d].d_name:%s\n", i, ptr[i].d_name);
		char temp_path[2048] = "/";
		if (strcmp(ptr[i].d_name, ".."))
		{
			strcat(temp_path, ptr[i].d_name);
			strcat(temp_path, abs_path);
			strcpy(abs_path, temp_path);
		}
	}

	iput(temp_inode);

	//printf("path[0]:%u\n", path[0]);
	//printf("path[1]:%u\n", path[1]);
	/*if (path[0] == path[1])
	{
		printf("/\n");
	}*/

	//printf("cur_path_inode->i_ino:%u\n", cur_path_inode->i_ino);
	//printf("cur_path_inode->di_addr[0]:%d\n", cur_path_inode->di_addr[0]);
	//printf("%s\n", ptr->d_name);
	//printf("ptr->d_ino:%u\n", ptr->d_ino);
	//printf("%s\n", abs_path);
	strcpy(wd, abs_path);
	free(ptr);
	ptr = NULL;
	//cur_path_inode->i_ino;
	return;

}


 
 





































