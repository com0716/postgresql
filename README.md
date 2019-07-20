#记录postgresql学习心得
PS:
  1 github使用http连接push代码，需要修改.git/config文件中remote的url，例如
    http://com0716@github.com/com0716/xxx.git

2019-07-20
Code：记录postgresql学习过程中关于代码的心得
libpq：记录关于接口libpq的相关心得


Makefile：
  1 $(notdir '/usr/bin/gcc') 得到结果gcc，作用是取掉路径中的文件夹
  2 $(addprefix '/usr/bin/', 'gcc') 得到结果'/usr/bin/gcc'，字符串拼接


