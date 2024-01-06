## 程序错误

提供给大家的文件系统存在以下错误，大家可以对这些错误进行修改：

1. - [x] 程序运行开始就越界崩溃（在有的编译环境可能没有该错误）。

 ![1](img/1.png)

2. - [x] 创建一个与目录同名的文件时，系统崩溃。 -- commit [`dd64462`](https://github.com/LukeBriton/File_System/commit/dd64462ed5a42e6f4187aa9e5e83663e5872c456)

 ![2](img/2.png)

3. - [x] 重复创建文件目录时提示错误；重复创建同一个文件时出错，错误如下： -- commit [`682eee7`](https://github.com/LukeBriton/File_System/commit/682eee7a16b82f9cbb646c0886461c39b8a1072c)

 ![3](img/3.png)

4. - [x] Dir命令，显示当前目录一直是“..”，请修改为正确的当前路径。 -- commit [`37acf99`](https://github.com/LukeBriton/File_System/commit/37acf99fc34207aa1cb698fc44f423f5f57a2a88)

 

5. - [x] 在根目录下面创建子目录a，在a中创建文件b，并写入大于一个块(512字节)的内容，之后返回根目录，查看目录内容的时候会出现错误。 -- commit [`57bbe1c`](https://github.com/LukeBriton/File_System/commit/57bbe1c22f8b58f9e396f10652114215131a08d1)

 ![5](img/5.png)

6. - [x] Read或Write一个不存在的文件时，程序会崩溃。 -- commit [`c372139`](https://github.com/LukeBriton/File_System/commit/c372139aca8a9d138d15ef0cb3a9f2ae4290dca8), [`e80798c`](https://github.com/LukeBriton/File_System/commit/e80798cb61c3a12337b2a29501e5338f872c74b8) [`899fb44`](https://github.com/LukeBriton/File_System/commit/899fb44b9e616babe51e81b1876f00fe3363afed)

 ![6](img/6.png)

7. - [x] 磁盘回收后再分配会出错，例如图中的第13个盘块就不见了。 -- commit [`d5d3535`](https://github.com/LukeBriton/File_System/commit/d5d3535840912e74d3d56ddf6be48f95ff2f708e)

 ![7](img/7.png)

8. - 系统其他错误。

- [x] `gets(buf)` replaced by `gets_s(buf, n)` -- commit [`09134fb`](https://github.com/LukeBriton/File_System/commit/09134fb2e5891bb34cfa0ea7264eaf503e802f11)
- [x] 奇奇怪怪英文拼写错误、语法问题。（随时跟进）

## 建议增加的功能

1. - [ ] 现有的程序是用内存模拟磁盘，建议大家改用文件模拟磁盘。这样，新创建的文件和目录等可以保留在模拟磁盘中，下次重新登录系统，可以查看上次登录时创建的文件和目录；

2. - [ ] 增加新建用户的新功能：在命令行中输入“adduser ***”后，可以新增用户，新增的用户下次登录后仍然存在；

3. - [ ] 增加password命令可以修改用户密码；

4. - [ ] 增加查看当前用户信息的新功能：在命令行中输入“who”，可以显示当前用户的所有信息，包括这个用户的id号、登录密码、所属用户组等；

5. - [ ] 在命令提示符前加入路径；增加pwd命令用来显示当前路径；

6. - [ ] 文件名更名；

7. - [ ] 复制文件；

8. - [ ] 其他功能。