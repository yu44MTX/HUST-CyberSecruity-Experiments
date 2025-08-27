SELECT Sno AS 学号, Sname AS 姓名, Sage AS 年龄
FROM Student;

SELECT *
FROM Student AS s
WHERE s.Sdept = 'CS';

SELECT Sno AS 学号, Cno AS 课程号, Grade AS 成绩
FROM SC AS s
WHERE s.Grade >= 90 OR s.Grade < 60;

SELECT Sname AS 姓名, Ssex AS 性别, Sage AS 年龄
FROM Student AS s
WHERE s.Sage NOT IN (19, 20);

SELECT Sname AS 姓名, Sdept AS 所在系
FROM Student
WHERE Sdept IN ('MA', 'IS');

SELECT Cno AS 课程号, Cname AS 课程名, Ccredit AS 学分
FROM Course AS c
WHERE c.Cname LIKE '%数据%';

SELECT Sno AS 学号
FROM Student AS s
WHERE NOT EXISTS (
    SELECT 1
    FROM SC AS sc
    WHERE s.Sno = sc.Sno
);

SELECT MAX(Grade) AS 最高分, MIN(Grade) AS 最低分, AVG(Grade) AS 平均分
FROM SC AS s
WHERE s.Sno = '200215121';

SELECT Sno AS 学号, Grade AS 成绩
FROM SC
WHERE Cno = '2'
ORDER BY Grade ASC;

SELECT Sdept AS 系别, AVG(Sage) AS 平均年龄
FROM Student AS s
GROUP BY Sdept;
