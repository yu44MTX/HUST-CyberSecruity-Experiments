SELECT c.Cno, c.Cname, s.Sno, s.Grade
FROM Course AS c
LEFT JOIN SC AS s ON c.Cno = s.Cno;

SELECT s1.Sno, s1.Sname, s1.Sage
FROM Student AS s1, Student AS s2
WHERE s2.Sname = '张立' AND s1.Sage = s2.Sage AND s1.Sname <> '张立';

SELECT s1.Sno, s1.Sname, s1.Sage
FROM Student AS s1
WHERE s1.Sage = (
    SELECT s2.Sage
    FROM Student AS s2
    WHERE s2.Sname = '张立'
)
AND s1.Sname <> '张立';

SELECT s1.Sno, s1.Sname, s1.Sage
FROM Student AS s1
JOIN Student AS s2 ON s1.Sage = s2.Sage
WHERE s2.Sname = '张立' AND s1.Sname <> '张立';

SELECT s.Sno, s.Sname
FROM SC AS sc
JOIN Student AS s ON s.Sno = sc.Sno
WHERE sc.Cno = '3' AND sc.Grade BETWEEN 80 AND 89;

SELECT c.Cno, c.Cname
FROM Course AS c
JOIN SC AS sc ON c.Cno = sc.Cno
WHERE sc.Sno = '200215122';

SELECT s.Sno, s.Cno
FROM SC AS s
JOIN (
    SELECT Cno, AVG(Grade) - 5 AS avg
    FROM SC
    GROUP BY Cno
) AS avg_scores ON s.Cno = avg_scores.Cno
WHERE s.Grade < avg_scores.avg;

SELECT s.Sno, s.Sname, s.Sage
FROM Student AS s,
(
    SELECT MIN(Sage) AS age
    FROM Student
    WHERE Ssex = '男'
) AS min_age
WHERE s.Ssex = '女' AND s.Sage < min_age.age;

SELECT s.Sname, s.Sdept
FROM Student AS s
JOIN SC AS sc ON s.Sno = sc.Sno
WHERE sc.Cno = '2';

UPDATE Student AS s
JOIN SC AS sc ON s.Sno = sc.Sno
SET s.Sage = s.Sage + 2
WHERE sc.Grade BETWEEN 80 AND 89;

SELECT DISTINCT s.Sno, s.Sname, s.Sage
FROM Student AS s
JOIN SC AS sc ON s.Sno = sc.Sno
WHERE sc.Grade BETWEEN 80 AND 89;

INSERT INTO Course (Cno, Cname)
VALUES
    ('10', 'C语言'),
    ('11', '人工智能');

DELETE FROM Course
WHERE Cname = '人工智能';

SELECT *
FROM Course;
