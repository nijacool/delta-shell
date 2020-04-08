-- SuSQL test file
drop table students; 
# Must fail
drop table students; 
# Must fail 
create table students(name char(16), age int, name char(1), gpa float); 
create table students(name char(16), age int, gender char(1), gpa float);
# Must fail
create table students(name char(16), age int, gender char(1), gpa float);
drop table students;
# Must fail
drop table students; 
# Must fail
insert into students values("Jane Jillian Verylongname", 22, "F", 2.12);
create table students(name char(16), age int, gender char(1), gpa float);
# Must fail
insert into students values("Jane Jillian Verylongname", 22, "F", 2.12);
# Must fail
insert into students values("Jane Smith", 22, 2.12, "F");
# Must fail
insert into students values("Jane Smith", 22.5, "F", 3);
insert into students values("Jane Smith", 22, "F", 2.12);
