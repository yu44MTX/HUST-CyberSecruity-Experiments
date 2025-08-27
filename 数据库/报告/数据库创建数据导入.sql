USE `csedb-u202112146`;
create table Student 
 (
	Sno CHAR(9) PRIMARY KEY,
	Sname CHAR(20) UNIQUE,
	Ssex CHAR(2),
	Sage SMALLINT,
	Sdept CHAR(20),
	Scholarship char(2)
 );

/*表 Student 的主码为 Sno，属性列 Sname 取唯一值*/
create table Course 
 (Cno CHAR(4) PRIMARY KEY,
 Cname CHAR(40),
 Cpno CHAR(4),
 Ccredit SMALLINT,
 FOREIGN KEY (Cpno) REFERENCES Course(Cno)
 );

/*表 Course 的主码为 Cno，属性列 Cpno(先修课)为外码，被参照表为 Course，被参照列是
Cno*/
create table SC 
 (Sno CHAR(9),
 Cno CHAR(4),
 Grade SMALLINT,
 primary key (Sno, Cno),
 FOREIGN KEY (Sno) REFERENCES Student(Sno),
 FOREIGN KEY (Cno) REFERENCES Course(Cno)
 );


insert into student values('200215121','李勇','男',20,'CS','否');
insert into student values('200215122','刘晨','女',19,'CS','否');
insert into student values('200215123','王敏','女',18,'MA','否');
insert into student values('200215125','张立','男',19,'IS','否');


insert into course values('1', '数据库', NULL,4);
insert into course values('2', '数学', NULL,2);
insert into course values('3', '信息系统', NULL,4);
insert into course values('4', '操作系统', NULL,3);
insert into course values('5', '数据结构', NULL,4);
insert into course values('6', '数据处理', NULL, 2);
insert into course values('7', 'java', NULL,4);

update Course set Cpno = '5' where Cno = '1';
update Course set Cpno = '1' where Cno = '3';
update Course set Cpno = '6' where Cno = '4';
update Course set Cpno = '7' where Cno = '5';
update Course set Cpno = '6' where Cno = '7';
/*为表 Course 添加数据*/

insert into SC values('200215121', '1',92);
insert into SC values('200215121', '2',85);
insert into SC values('200215121', '3',88);
insert into SC values('200215122', '2',90);
insert into SC values('200215122', '3',80);
/*为表 SC 添加数据*/
