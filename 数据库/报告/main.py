from myssql import *
import pymysql


def show_menu():
    print("-"*30)
    print("1.添加学生信息\t2.修改学生信息\n"
          "3.增加新课程\t4.修改课程信息\n"
          "5.录入学生成绩\t6.修改学生成绩\n"
          "7.按系统计信息\n"
          "8.系内排名\n"
          "9.查询学生信息\n"
          "0.退出")
    print("-" * 30)
    return


def op1(cur):
    ipt = input("请输入学生信息(学号,姓名,性别,年龄,院系): ")
    ipt = ipt.split(',')
    ipt[3] = int(ipt[3])
    data = tuple(ipt)
    add_student(cur, data)
    return


def op2(cur):
    func = {"姓名": "sname", "性别": "ssex", "年龄": "sage", "院系": "sdept"}
    ipt = input("请输入修改部分(姓名/性别/年龄/院系): ")
    typename = func.get(ipt, False)
    if not typename:
        print("输入错误")
        return
    ipt = input("请输入要修改的内容: ")
    data = ipt
    if typename == "sage":
        data = int(data)
    ipt = input("请输入学生学号: ")
    condition = f"sno = {ipt}"
    update_student(cur, typename, data, condition)
    return


def op3(cur):
    ipt = input("请输入课程信息(课程号,课程名,先修课号,学分)(无则填无): ")
    ipt = ipt.split(',')
    for it in range(0,4):
        if ipt[it] == '无':
            ipt[it] = None
    data = tuple(ipt)
    add_course(cur, data)
    return


def op4(cur):
    func = {"课程名": "cname", "先修课号": "cpno", "学分": "ccredit"}
    ipt = input("请输入修改部分(课程名/先修课号/学分): ")
    typename = func.get(ipt, False)
    if not typename:
        print("输入错误")
        return
    ipt = input("请输入要修改的内容: ")
    data = ipt
    ipt = input("请输入课程号: ")
    condition = f"cno = {ipt}"
    update_course(cur, typename, data, condition)
    return


def op5(cur):
    ipt = input("请输入成绩信息(学号,课程号,成绩)(无则填无): ")
    ipt = ipt.split(',')
    for it in range(0, 3):
        if ipt[it] == '无':
            ipt[it] = None
    data = tuple(ipt)
    add_sc(cur, data)
    return


def op6(cur):
    data = input("请输入成绩: ")
    sno = input("请输入学号: ")
    cno = input("请输入课程号: ")
    update_sc(cur, sno, cno, data)
    return


def op7(cur):
    cno = input("输入要统计的课程号: ")
    show_statistics(cur, cno)
    return


def op8(cur):
    sdept = input("输入要查询排名的系(IS/CS/MA): ")
    show_rank(cur, sdept)
    return


def op9(cur):
    sno = input("请输入要查询的学生学号: ")
    show_student(cur, sno)
    return


def operror(cur):
    print("选项号错误")
    return


op = {
    '1': op1,
    '2': op2,
    '3': op3,
    '4': op4,
    '5': op5,
    '6': op6,
    '7': op7,
    '8': op8,
    '9': op9
      }

def main():
    conn = pymysql.connect(host='localhost',
                           user='root',
                           password='123456',
                           db='csedb-u202112146'
                           )
    cur = conn.cursor()

    while True:
        show_menu()
        num = input("请输入操作序号: ")
        if num == '0':
            break
        op.get(num, operror)(cur)

    conn.close()
    cur.close()
    return

main()