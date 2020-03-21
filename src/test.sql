-- SuSQL test file
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
