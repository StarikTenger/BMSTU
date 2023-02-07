--Ћабораторна€ работа є10. –ежимы выполнени€ транзакций
--1.»сследовать и проиллюстрировать на примерах различные уровни изол€ции транзакций MS SQL Server, 
	--устанавливаемые с использованием инструкции SET TRANSACTION ISOLATION LEVEL.

-- query 1 ---------------------------------------------------------------------------------

drop table if exists dbo.users
go
create table dbo.users 
(
	id			int,
	nickname		varchar(35),
	karma		int,

	PRIMARY KEY (id)
);
go

insert into dbo.users values
	(1, 'xtoter', 999),
	(2, 'stariktenger', 1337),
	(3, 'tonitan', 1500)
go

-- 1

--begin transaction
--	select * from dbo.users
--	update dbo.users 
--		set karma = 90 
--		where id = 1
--	waitfor delay '00:00:05'
--	select * from dbo.users
--commit


-- 2

--set transaction isolation level 
--	read committed
--begin transaction
--	select * from dbo.users
--	waitfor delay '00:00:05'
--	select * from dbo.users
--	rollback

-- 3

--set transaction isolation level 
--	repeatable read
--begin transaction
--	select * from dbo.users
--	waitfor delay '00:00:05'
--	select * from dbo.users
-- commit


-- 4

set transaction isolation level 
	repeatable read
begin transaction
	select * from dbo.users where karma < 1000
	waitfor delay '00:00:05'
	select * from dbo.users where karma < 1000
commit

-- 5
--set transaction isolation level 
--	serializable
--begin transaction
--	select * from dbo.users where karma < 1000
--	waitfor delay '00:00:05'
--	select * from dbo.users where karma < 1000
--		--только строки в диапазоне (1 .. 2) PRIMARY KEY будут захвачены
--		--остальные строки таблицы можно измен€ть
--	rollback --?
--select * from dbo.users

-- query 2 ---------------------------------------------------------------------------------

--1

--set transaction isolation level 
--	read uncommitted
--select * from dbo.users



-- 2

--set transaction isolation level 
--	read committed
--select * from dbo.users;



-- 3

--update dbo.users set karma = 90 where id = 3
--insert into dbo.users values (9, 'mazdaywik', 0)


-- 4

update dbo.users set karma = 90 where id = 2
insert into dbo.users values (9, 'mazdaywik', 999)


-- results ---------------------------------------------------------------------------------

-- 1 - 1 dirty read      (solved in 1 - 2)
-- 2 - 3 read uncommited (solved in 3 - 3)
-- 4 - 4 phantom read    (solved in 5 - 4)
