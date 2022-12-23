use lab6

--1.Создать представление на основе одной из таблиц задания 6.
if OBJECT_ID(N'students_view', N'V') is not null
	drop view StudentView;
go

create view StudentView as 
	select *
	from Student
	where RoomID = 323;
go

select* from StudentView;
go

--2.Создать представление на основе полей обеих связанных таблиц задания 6.
if OBJECT_ID(N'students_view_joined', N'V') is not null
	drop view students_view_joined;
go
create view students_view_joined as
	select 
		s.SecondName + ' ' + s.FirstName + ' ' + s.Patronymic as StudentName, 
		'room ' + cast(r.RoomID as varchar) + ' where ' + cast(r.Capacity as varchar) + ' students live' as RoomDescription
	from Student s 
	inner join Room r
		on s.RoomID = r.RoomID
go

select * from students_view_joined;
go

--3.Создать индекс для одной из таблиц задания 6, включив в него дополнительные неключевые поля.
if EXISTS (select name from sys.indexes 
			where name = N'IndexStudentDateOfBirth')
	drop index IndexStudentDateOfBirth on Student;
go
create index IndexStudentDateOfBirth
	on Student (PhoneNumber)
	include (DateOfBirth);
go

select * from Student where PhoneNumber like '%977%'

--4.Создать индексированное представление.
if OBJECT_ID(N'RoomIndexView', N'V') is not null
	drop view RoomIndexView;
go

--table that is referenced by view_with_SCHEMABINDING cannot be modified
--if this modification affects view
create view RoomIndexView 
	with SCHEMABINDING	-- restrictions
	as select FirstName, DateOfBirth
	from dbo.Student
	where RoomID = 323;
go

--unique clustered index must be the !first! to be made
--only then any other index can be applied
if EXISTS (select name from sys.indexes 
			where name = N'RoomIndexView')
	drop index RoomIndexView on Student;
go
create UNIQUE CLUSTERED index IndexStudentDateOfBirth -- clustered index has to maintain the physical order of the stored records according to the indexed column i.e. id
	on RoomIndexView(FirstName, DateOfBirth);
go

select * from RoomIndexView;
