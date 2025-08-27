from sqlalchemy import create_engine, Column, String, Integer, ForeignKey, Float, CheckConstraint, and_, func
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
import pymysql

# 配置数据库连接
# 使用pymysql的连接方式配置SQLAlchemy的引擎
engine = create_engine('mysql+pymysql://root:Mysql@localhost/csedb-u202112146')
Base = declarative_base()
Session = sessionmaker(bind=engine)
session = Session()

# 定义学生表
class Student(Base):
    __tablename__ = 'student'
    Sno = Column(String(9), primary_key=True)  # 学号
    Sname = Column(String(20), unique=True)    # 姓名
    Ssex = Column(String(2))                   # 性别
    Sage = Column(Integer)                     # 年龄
    Sdept = Column(String(20))                 # 系别
    Scholarship = Column(String(2))            # 奖学金状态

# 定义课程表
class Course(Base):
    __tablename__ = 'course'
    Cno = Column(String(4), primary_key=True)  # 课程号
    Cname = Column(String(40))                 # 课程名
    Cpno = Column(String(4), ForeignKey('course.Cno'))  # 先修课课程号
    Ccredit = Column(Integer)                  # 学分
    pre_course = relationship("Course", remote_side=[Cno])  # 自引用关系

# 定义选修表
class SC(Base):
    __tablename__ = 'sc'
    Sno = Column(String(9), ForeignKey('student.Sno'), primary_key=True)  # 学号
    Cno = Column(String(4), ForeignKey('course.Cno'), primary_key=True)   # 课程号
    Grade = Column(Integer, CheckConstraint('Grade >= 0 and Grade <= 100'))  # 成绩

# 创建所有表
Base.metadata.create_all(engine)

# 增加新生入学信息
def add_student(sno, sname, ssex, sage, sdept, scholarship):
    student = Student(Sno=sno, Sname=sname, Ssex=ssex, Sage=sage, Sdept=sdept, Scholarship=scholarship)
    session.add(student)
    session.commit()

# 修改学生信息
def update_student(sno, **kwargs):
    student = session.query(Student).filter(Student.Sno == sno).first()
    for key, value in kwargs.items():
        setattr(student, key, value)
    session.commit()

# 增加新课程
def add_course(cno, cname, cpno, ccredit):
    course = Course(Cno=cno, Cname=cname, Cpno=cpno, Ccredit=ccredit)
    session.add(course)
    session.commit()

# 修改课程信息
def update_course(cno, **kwargs):
    course = session.query(Course).filter(Course.Cno == cno).first()
    for key, value in kwargs.items():
        setattr(course, key, value)
    session.commit()

# 删除没有选课的课程信息
def delete_course(cno):
    course = session.query(Course).filter(Course.Cno == cno).first()
    # 检查是否有学生选修了该课程
    if not session.query(SC).filter(SC.Cno == cno).first():
        session.delete(course)
        session.commit()

# 录入学生成绩
def add_grade(sno, cno, grade):
    sc = SC(Sno=sno, Cno=cno, Grade=grade)
    session.add(sc)
    session.commit()

# 修改学生成绩
def update_grade(sno, cno, grade):
    sc = session.query(SC).filter(and_(SC.Sno == sno, SC.Cno == cno)).first()
    sc.Grade = grade
    session.commit()

# 统计学生成绩信息
def calculate_statistics():
    avg_grade = session.query(SC.Cno, func.avg(SC.Grade)).group_by(SC.Cno).all()
    max_grade = session.query(SC.Cno, func.max(SC.Grade)).group_by(SC.Cno).all()
    min_grade = session.query(SC.Cno, func.min(SC.Grade)).group_by(SC.Cno).all()
    excellent_rate = session.query(SC.Cno, func.sum(SC.Grade >= 90) / func.count(SC.Grade)).group_by(SC.Cno).all()
    fail_count = session.query(SC.Cno, func.sum(SC.Grade < 60)).group_by(SC.Cno).all()
    return avg_grade, max_grade, min_grade, excellent_rate, fail_count

# 按系对学生成绩进行排名
def rank_students_by_department():
    ranks = session.query(Student.Sdept, Student.Sno, Student.Sname, SC.Cno, SC.Grade).\
            join(SC, Student.Sno == SC.Sno).\
            order_by(Student.Sdept, SC.Grade.desc()).all()
    return ranks

# 显示学生基本信息和选课信息
def get_student_info(sno):
    student_info = session.query(Student).filter(Student.Sno == sno).first()
    courses = session.query(Course.Cno, Course.Cname, SC.Grade).\
              join(SC, Course.Cno == SC.Cno).\
              filter(SC.Sno == sno).all()
    return student_info, courses

# 示例操作
if __name__ == '__main__':
    # 增加新生入学信息
    add_student('200215126', '赵刚', '男', 21, 'CS', '否')
    # 修改学生信息
    update_student('200215126', Sname='赵伟')
    # 增加新课程
    add_course('8', '机器学习', None, 3)
    # 修改课程信息
    update_course('8', Cname='深度学习')
    # 删除没有选课的课程信息
    delete_course('8')
    # 录入学生成绩
    add_grade('200215126', '1', 95)
    # 修改学生成绩
    update_grade('200215126', '1', 85)
    # 统计学生成绩信息
    avg_grade, max_grade, min_grade, excellent_rate, fail_count = calculate_statistics()
    # 按系对学生成绩进行排名
    ranks = rank_students_by_department()
    # 显示学生基本信息和选课信息
    student_info, courses = get_student_info('200215126')

    # 打印结果
    print(f"学生信息: {student_info}")
    print(f"课程信息: {courses}")
    print(f"平均成绩: {avg_grade}")
    print(f"最高成绩: {max_grade}")
    print(f"最低成绩: {min_grade}")
    print(f"优秀率: {excellent_rate}")
    print(f"不及格人数: {fail_count}")
    print(f"排名: {ranks}")


