--Задание 13. Создание распределенных баз данных на основе секционированных представлений


--1.Создать две базы данных на одном экземпляре СУБД SQL Server 2012.
--2.Создать в базах данных п.1. горизонтально фрагментированные таблицы.
--3.Создать секционированные представления, обеспечивающие работу с данными таблиц (выборку, вставку, изменение, удаление).



--1.Создать две базы данных на одном экземпляре СУБД SQL Server 2012.
use master;
go
drop database if exists lab13_1
create database lab13_1
go


use master;
go
drop database if exists lab13_2
create database lab13_2
go



--2.Создать в базах данных п.1. горизонтально фрагментированные таблицы.
use lab13_1;
go
drop table if exists dbo.student
CREATE TABLE student(
	id INT,
	full_name NVARCHAR(60) NOT NULL,
	phone_number NVARCHAR(60),
	PRIMARY KEY (id),
	CONSTRAINT check_id
                CHECK (id <= 5)
)
go

use lab13_2;
go
drop table if exists dbo.student
CREATE TABLE student(
	id INT,
	full_name NVARCHAR(60) NOT NULL,
	phone_number NVARCHAR(60),
	PRIMARY KEY (id),
	CONSTRAINT check_id
                CHECK (id > 5)
)
go



--3.Создать секционированные представления, обеспечивающие работу с данными таблиц 
	--(выборку, вставку, изменение, удаление).
use lab13_1;
go
drop view if exists horizontal_dist_v;
go
create view horizontal_dist_v as
	select * from lab13_1.dbo.student
	union all
	select * from lab13_2.dbo.student
go



insert horizontal_dist_v values
	(1, 'Kanila Dryazihin', '88005553535'),
	(2, 'Igorg Asyutkyin', '83743734129'),
	(3, 'Pzhegor Phekipch', '9123873843'),
	(5, 'John Silver', '454325235'),
	(6, 'Captain Smollett', '88005553535'),
	(7, 'Dr. Livesey ', '563452354')
	;

select * from lab13_1.dbo.student;
select * from lab13_2.dbo.student;

select * from horizontal_dist_v

update horizontal_dist_v
	set id = 10
	where id = 1

delete horizontal_dist_v
	where full_name = 'Pzhegor Phekipch'

select * from lab13_1.dbo.student;
select * from lab13_2.dbo.student;