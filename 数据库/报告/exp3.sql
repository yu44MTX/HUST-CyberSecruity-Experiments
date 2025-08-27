create view CS_View as
select * from Student
where Sdept='CS';

select * from CS_View where exists(
	select * from SC where CS_View.Sno=SC.Sno and Cno='1'
);

select * from Student
where Sdept='IS' and exists(
	select * from SC where Student.Sno=SC.Sno and Grade > 80
);
select * from IS_View;

grant select, update on table Student to U1;
grant insert on table SC to U2;


DELIMITER //

CREATE TRIGGER update_scholarship
AFTER UPDATE ON SC
FOR EACH ROW
BEGIN
    DECLARE has_high_score INT;

    IF NEW.Grade >= 95 THEN
        UPDATE Student SET Scholarship = '是' WHERE Sno = NEW.Sno AND Scholarship = '否';
    ELSE
        SELECT COUNT(*) INTO has_high_score
        FROM SC
        WHERE Sno = NEW.Sno AND Grade >= 95;
        
        IF has_high_score = 0 AND OLD.Grade >= 95 THEN
            UPDATE Student SET Scholarship = '否' WHERE Sno = NEW.Sno;
        END IF;
    END IF;
END //

DELIMITER ;

UPDATE SC SET Grade = 98 WHERE Sno = '200215121' AND Cno = '1';
SELECT Sno, Scholarship FROM Student WHERE Sno = '200215121';

UPDATE SC SET Grade = 80 WHERE Sno = '200215121' AND Cno = '1';
SELECT Sno, Scholarship FROM Student WHERE Sno = '200215121';

DROP TRIGGER IF EXISTS update_scholarship;

DELIMITER //

CREATE PROCEDURE calculate_CS_scores()
BEGIN
    SELECT Cno, AVG(Grade) AS AvgGrade, MAX(Grade) AS MaxGrade
    FROM SC
    WHERE Cno IN (SELECT Cno FROM Course WHERE Cname LIKE '%CS%')
    GROUP BY Cno;
END //

DELIMITER ;
CALL calculate_CS_scores();
DELIMITER //

CREATE PROCEDURE get_student_grades(IN student_no CHAR(9))
BEGIN
    SELECT S.Sname, SC.Cno, C.Cname, SC.Grade
    FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Course C ON SC.Cno = C.Cno
    WHERE S.Sno = student_no;
END //

DELIMITER ;
CALL get_student_grades('200215121');
DELIMITER //

CREATE FUNCTION get_student_grades_func(student_no CHAR(9))
RETURNS TABLE (
    Sname CHAR(20),
    Cno CHAR(4),
    Cname CHAR(40),
    Grade SMALLINT
)
BEGIN
    RETURN SELECT S.Sname, SC.Cno, C.Cname, SC.Grade
    FROM Student S
    JOIN SC ON S.Sno = SC.Sno
    JOIN Course C ON SC.Cno = C.Cno
    WHERE S.Sno = student_no;
END //

DELIMITER ;

UPDATE SC SET Grade = 120 WHERE Sno = '200215121' AND Cno = '1';
SELECT * FROM SC WHERE Sno = '200215121' AND Cno = '1';

-- 将成绩修改回原来的值
UPDATE SC SET Grade = 98 WHERE Sno = '200215121' AND Cno = '1';
ALTER TABLE SC
ADD CONSTRAINT chk_grade CHECK (Grade BETWEEN 0 AND 100);
-- 尝试将成绩修改为 120，应该会失败
UPDATE SC SET Grade = 120 WHERE Sno = '200215121' AND Cno = '1';

-- 查询验证
SELECT * FROM SC WHERE Sno = '200215121' AND Cno = '1';
