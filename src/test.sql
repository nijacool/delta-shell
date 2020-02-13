# Are comments allowed?
create table my_data (id int, name char(15), age float);
insert into my_data values (10, "Bob Smith", 23.7);
select age from my_data where id=10;
update my_data set name="Robert Smith" where age>25;
delete from my_data where id <=10;
select * from my_data;
drop table my_data;
-- Are comments allowed?
^end^
