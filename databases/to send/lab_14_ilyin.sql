--Задание 14. Создание вертикально фрагментированных таблиц средствами СУБД SQL Server 2012


--1.Создать в базах данных пункта 1 задания 13 таблицы, содержащие вертикально фрагментированные данные.
--2.Создать необходимые элементы базы данных (представления, триггеры), 
	--обеспечивающие работу с данными вертикально фрагментированных таблиц (выборку, вставку, изменение, удаление).



use master;
go
drop database if exists lab14_1
create database lab14_1
go

 
use master;
go
drop database if exists lab14_2
create database lab14_2
go
 
 
--1.Создать в базах данных пункта 1 задания 13 таблицы, содержащие вертикально фрагментированные данные.
use lab14_1;
go
if OBJECT_ID(N'dbo.student', N'U') is not null
    drop table dbo.student;
go
create table dbo.student (
    id              int,
    student           varchar(60),
 
    PRIMARY KEY (id)
    );
go
 
 
use lab14_2;
go
if OBJECT_ID(N'dbo.student', N'U') is not null
    drop table dbo.student;
go
create table dbo.student (
    id              int,
    phone_number          varchar(60),
 
    PRIMARY KEY (id),
    );
go
 
 
--2.Создать необходимые элементы базы данных (представления, триггеры), 
	--обеспечивающие работу с данными вертикально фрагментированных таблиц (выборку, вставку, изменение, удаление).
use lab14_1;
go
if OBJECT_ID(N'vertical_dist_v', N'V') is not null
    drop view vertical_dist_v;
go
create view vertical_dist_v as
    select one.id, one.student, two.phone_number
        from lab14_1.dbo.student as one,
            lab14_2.dbo.student as two
        where one.id = two.id
go
 
 
-- insert --
drop trigger if exists dbo.dist_ins
go
create trigger dbo.dist_ins
    on dbo.vertical_dist_v
    instead of insert
    as
    begin
 
        insert into lab14_1.dbo.student(id, student)
            select id, student
                from inserted
 
        insert into lab14_2.dbo.student(id, phone_number)
            select id, phone_number
                from inserted
    end
go
 
insert vertical_dist_v values
	(1, 'Kanila Dryazihin', '88005553535'),
	(2, 'Igorg Asyutkyin', '83743734129'),
	(3, 'Pzhegor Phekipch', '9123873843'),
	(5, 'John Silver', '454325235'),
	(6, 'Captain Smollett', 'unknown'),
	(7, 'Dr. Livesey ', '563452354');
 
select * from dbo.vertical_dist_v
select * from lab14_1.dbo.student
select * from lab14_2.dbo.student
 
-- delete --
drop trigger if exists  dbo.dist_del
go
create trigger dbo.dist_del
    on dbo.vertical_dist_v
    instead of delete
    as
    begin
       
        delete lab14_1.dbo.student
            where id in (select d.id
                from deleted as d)
 
        delete lab14_2.dbo.student
            where id in (select d.id
                from deleted as d)
    end
go
 
delete dbo.vertical_dist_v
    where phone_number = 'unknown'
 
select * from dbo.vertical_dist_v
select * from lab14_1.dbo.student
select * from lab14_2.dbo.student
 
 
-- update --
drop trigger if exists  dbo.dist_upd
go
create trigger dbo.dist_upd
    on dbo.vertical_dist_v
    instead of update
    as
    begin
 
        if UPDATE(id)
            raiserror('[UPD TRIGGER]: "id" cant be modidfied', 16, 1)
        else
        begin
 
            update lab14_1.dbo.student
                set student.student = (select student from inserted where inserted.id = student.id)
                    where student.id = (select id from inserted where inserted.id = student.id)
 
            update lab14_2.dbo.student
                set student.phone_number = (select phone_number from inserted where inserted.id = student.id)
                    where student.id = (select id from inserted where inserted.id = student.id)
        end
    end
go
 
update dbo.vertical_dist_v
    set student = student +'X' --phone_number = '89116620589'
    --where phone_number = '88005553535'
 
select * from dbo.vertical_dist_v
select * from lab14_1.dbo.student
select * from lab14_2.dbo.student