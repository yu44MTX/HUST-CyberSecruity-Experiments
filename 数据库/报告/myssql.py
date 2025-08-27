import pymysql

nice_grade = 80


def add_student(cur, data):
    cmd = "insert into student values (%s,%s,%s,%s,%s,'否')"
    cur.execute(cmd, data)
    cur.connection.commit()
    return True


def update_student(cur, type, data, condtion):
    cmd = f"update student set {type} = %s where {condtion}"
    cur.execute(cmd, (data,))
    cur.connection.commit()
    return True


def add_course(cur, data):
    cmd = 'insert into course values(%s,%s,%s,%s)'
    cur.execute(cmd, data)
    cur.connection.commit()
    return True


def update_course(cur, type, data, condition):
    cmd = f"update course set {type} = %s where {condition}"
    cur.execute(cmd, (data,))
    cur.connection.commit()
    return True


def add_sc(cur, data):
    cmd = 'insert into sc values(%s,%s,%s)'
    cur.execute(cmd, data)
    cur.connection.commit()
    return True


def update_sc(cur, sno, cno, grade):
    cmd = f"update sc set grade = %s where sno = '{sno}' and cno = '{cno}'"
    cur.execute(cmd, (grade,))
    cur.connection.commit()
    return True


def show_statistics(cur, cno):
    total_d = dict()
    # 统计总人数
    cmd = f"select sdept, count(*) from student,sc " \
          f"where student.sno = sc.sno and cno = '{cno}' " \
          f"group by sdept"
    cur.execute(cmd)
    for row in cur.fetchall():
        mid = list(row)
        total_d[mid[0]] = [mid[1]]

    # 统计优秀人数
    nice_d = dict()
    cmd = f"select sdept, count(*) from student,sc " \
          f"where student.sno = sc.sno and cno = '{cno}' and grade > {nice_grade} " \
          f"group by sdept"
    cur.execute(cmd)
    for row in cur.fetchall():
        mid = list(row)
        nice_d[mid[0]] = [mid[1]]

    # 统计不及格人数
    bad_d = dict()
    cmd = f"select sdept, count(*) from student,sc " \
          f"where student.sno = sc.sno and cno = '{cno}' and grade < 60 " \
          f"group by sdept"
    cur.execute(cmd)
    for row in cur.fetchall():
        mid = list(row)
        bad_d[mid[0]] =[mid[1]]

    # 统计平均成绩，最好成绩，最差成绩
    grades_d = dict()
    cmd = f"select sdept, avg(grade), max(grade), min(grade) from student,sc " \
          f"where student.sno = sc.sno and sc.cno = '{cno}'" \
          f"group by sdept"
    cur.execute(cmd)
    for row in cur.fetchall():
        mid = list(row)
        grades_d[mid[0]] = mid[1:]

    print("-"*30)
    print("系名\t\t平均\t\t最好\t\t最差\t\t优秀率\t\t不及格")
    for key in total_d.keys():
        print(f"{key}\t\t{grades_d[key][0]}\t\t{grades_d[key][1]}\t\t"
              f"{grades_d[key][2]}\t\t{nice_d.get(key,[0])[0] / total_d[key][0]}\t\t"
              f"{bad_d.get(key,[0])[0]}")
    print("-"*30)
    return True


def pri_sinfo(cur, sno):
    cmd = f"select * from student where sno = '{sno}'"
    cur.execute(cmd)
    sinfo = cur.fetchall()[0]
    return sinfo


def pri_cinfo(cur, sno):
    cmd = f"select cname,grade from sc,course " \
          f"where sc.cno = course.cno and sno = '{sno}'"
    cur.execute(cmd)
    cinfo = cur.fetchall()
    return cinfo


def pri_cmp(a):
    return a[1]


def show_rank(cur, sdept):
    avg_d = dict()
    cmd = f"select student.sno,avg(grade) " \
          f"from student,sc " \
          f"where student.sno = sc.sno and sdept = '{sdept}' " \
          f"group by student.sno"
    cur.execute(cmd)
    res = sorted(cur.fetchall(), key=pri_cmp,reverse=True)
    for row in res:
        avg_d[row[0]] = row[1]
    for key in avg_d.keys():
        sinfo = pri_sinfo(cur, key)
        cinfo = pri_cinfo(cur, key)
        print("-" * 30)
        print(f"学号:{key} 姓名:{sinfo[1]} 平均分:{avg_d[key]} 性别:{sinfo[2]} "
              f"年龄:{sinfo[3]} 院系:{sinfo[4]} 奖学金:{sinfo[5]}")
        print(f"所选课程:", end=' ')
        for item in cinfo:
            print(f"{item[0]}:{item[1]} ", end='')
        print()
    print("-" * 30)


def show_student(cur, sno):
    sinfo = pri_sinfo(cur, sno)
    cinfo = pri_cinfo(cur, sno)
    print("-" * 30)
    print(f"学号:{sno} 姓名:{sinfo[1]} 性别:{sinfo[2]} "
          f"年龄:{sinfo[3]} 院系:{sinfo[4]} 奖学金:{sinfo[5]}")
    print(f"所选课程:", end=' ')
    for item in cinfo:
        print(f"{item[0]}:{item[1]} ", end='')
    print()
    print("-" * 30)
    return True