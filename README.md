必须跟的参数：  
输入的FASAT文件的地址 input.fasta 必须作为第一个参数  
输出共识序列的FASTA文件地址（带文件名，文件夹必须存在）必须作为第二个参数  

可选参数：  
后面需要跟一个数字（非负，因为使用最大值函数）  
-g	  间隙开启代价 （默认6）       -g 6  
-h      间隙延伸代价 （默认2）       -h 2  
-m     匹配分数        （默认0）       -m 0  
-mis  错配分数        （默认4）       -mis 4  

后面需要跟地址（文件夹必须是存在的）  
-dot    需要输出结果DOT文件     -dot C:\result\result.dot  

需要预先对输入的fasta进行排序，后跟数字  
-s   
后跟数字的意义：  
1.按序列长度升序排列  
2.按序列长度降序排列  
3.使用k-mer计算序列间距离的UPGMA聚类方法排序  
4.使用LLCS计算序列间距离的UPGMA聚类方法排序  
5.使用k-mer计算序列间距离的single linkage聚类方法排序  
6.使用LLCS计算序列间距离的single linkage聚类方法排序  
注：当计算序列间距离的方法是k-mer时，可以跟两个参数，第二个参数是k的值，注意k的值不要超过序列长度，k值默认为6  

不需要跟其他信息的参数：  
-score 可以输出比对过程最后的分数  

使用例子：  
1. 输出帮助信息  
POA -help  
2. 只带必须的两个参数，第一个参数是待比对的文件名，第二个参数是共识序列输出的文件名  
POA E:\test\sample.fasta E:\result\con.fasta  
3. 更改比对参数，如匹配、错配、开间隙、延伸间隙得分  
POA E:\test\sample.fasta E:\result\con.fasta -g 4 -h 1 -m 0 -mis 2  
4. 输出每一步比对的得分  
POA E:\test\sample.fasta E:\result\con.fasta -score  
![image](https://github.com/user-attachments/assets/e4d0011a-7e57-4fc8-b217-051551c2971c)
5. 选择排序方法（第一行会输出排序方法）
![image](https://github.com/user-attachments/assets/dfdcca9e-4e14-4a38-9a4e-6cc73a78f5a5)  
![image](https://github.com/user-attachments/assets/ee49e710-b23a-4d58-bc56-875a935396d3)  

   


   
