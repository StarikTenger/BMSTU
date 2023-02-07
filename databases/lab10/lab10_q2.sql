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


-- 1-1 dirty read (solved in 1-2)
-- 2-3 read uncommited (solved in 3-3)
-- 4-4 phantom read (solved in 5-4)
