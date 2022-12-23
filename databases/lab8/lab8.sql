
use master;
go
if DB_ID (N'lab8_') is null
        create database lab8_
        on (
                NAME = lab8dat,
                FILENAME = 'C:\db\lab8_.mdf',
                SIZE = 10,
                MAXSIZE = UNLIMITED,
                FILEGROWTH = 5
                )
        log on (
                NAME = lab8log,
                FILENAME = 'C:\db\lab8_.ldf',
                SIZE = 5,
                MAXSIZE = 20,
                FILEGROWTH = 5
                );
go
 
use lab8_;
go
DROP TABLE IF EXISTS students
CREATE TABLE students(
	student_id INT IDENTITY(1,1) PRIMARY KEY,
	first_name NVARCHAR(60) NOT NULL CHECK (LEN(first_name) > 1),
	second_name NVARCHAR(60) NOT NULL CHECK (LEN(second_name) > 1),
	patronymic NVARCHAR(60) NOT NULL CHECK (LEN(patronymic) > 1),
	phone_number NVARCHAR(60) NOT NULL CHECK (LEN(phone_number) > 10),
	date_of_birth Date CHECK (date_of_birth < DATEADD(year, -12, GETDATE())) DEFAULT DATEADD(year, -12, GETDATE()),
	room_id INT NOT NULL
);


INSERT INTO students (second_name, first_name, patronymic, phone_number, date_of_birth, room_id) VALUES
(N'Makaronov',' Dagar','Olegovich','89116620590', '1999-12-06', '717'),
(N'Kanila',' Dryazikhin','Olegovich','88005553535', '2006-02-06', '902'),
(N'Teryukha',' Mikhail','Romanovich','99875643728', '2005-12-12', '323'),
(N'Khzyshoterian',' Kzhyzhtov','Kzhypzhystekevixh','99871245728', '2001-05-05', '717'),
(N'Rulon',' Oboyev','Rulonovich','91231245728', '2002-05-09', '902'),
(N'Asyutkyin',' Igorg','Olegovich','99875643749', '2003-02-06', '323');

select * from students;
go


-- Вспомогательные функции
if OBJECT_ID(N'dbo.get_age', N'FN') is not null
	drop function dbo.get_age;
go
create function dbo.get_age(@birthdate date)
	returns int
	with execute as caller
	as
	begin
		declare @current_date datetime = GETDATE();
		declare @current_year int, @year_delta int;

		set @current_year = YEAR(@current_date);
		set @year_delta = @current_year - YEAR(@birthdate);
	
		return @year_delta;
	end
go

if OBJECT_ID(N'dbo.compare_ages', N'FN') is not null
	drop function dbo.compare_ages;
go
create function dbo.compare_ages(@compare_what int, @compare_to int)
	returns int
	with execute as caller
	as
	begin
		declare @retval int;

		if (@compare_what >= @compare_to)
			set @retval = 1;
		else 
			set @retval = 0;

		return @retval;
	end
go


--Создать хранимую процедуру, производящую выборку из некоторой таблицы и возвращающую результат выборки в виде курсора.

--Модифицировать хранимую процедуру п.1. таким образом, чтобы выборка осуществлялась 
--с формированием столбца, значение которого формируется пользовательской функцией.

if OBJECT_ID(N'dbo.sub_proc', N'P') is not null
	drop procedure dbo.sub_proc
go

create procedure dbo.sub_proc
	@curs cursor varying output
as
	set nocount on;
	set @curs = cursor
	scroll static for
		select first_name, second_name, dbo.get_age(date_of_birth)
		from dbo.students
		option (maxrecursion 0);
	open @curs;
go

--Создать хранимую процедуру, вызывающую процедуру п.1., 
--осуществляющую прокрутку возвращаемого курсора и выводящую сообщения, 
--сформированные из записей при выполнении условия, заданного еще одной пользовательской функцией.

if OBJECT_ID(N'dbo.external_proc', N'P') is not null
	drop procedure dbo.external_proc
go
create procedure dbo.external_proc
AS
	declare @ext_curs cursor;
	declare @t_fstnm varchar(254);
	declare @t_sndnm varchar(35);
	declare @t_age int;

	exec dbo.sub_proc @curs = @ext_curs OUTPUT;

	--always use fetch_next before actual fetch according to MSDN
	FETCH NEXT FROM @ext_curs INTO @t_fstnm, @t_sndnm, @t_age;
	print 'First Fetch: "' + @t_fstnm + '"'

	WHILE (@@FETCH_STATUS = 0)
	BEGIN
		IF (dbo.compare_ages(@t_age, 18) = 1)
			print @t_fstnm + ' ' + @t_sndnm + ' is ' + CAST(@t_age as varchar) + ' years ago'
		FETCH NEXT FROM @ext_curs
		INTO @t_fstnm, @t_sndnm, @t_age;
	END

	CLOSE @ext_curs;
	DEALLOCATE @ext_curs;
GO

exec dbo.external_proc
go


--Модифицировать хранимую процедуру п.2. таким образом, 
--чтобы выборка формировалась с помощью табличной функции.

IF EXISTS (SELECT * FROM sysobjects WHERE id = object_id(N'dbo.get_classic') 
    AND xtype IN (N'FN', N'IF', N'TF'))
    DROP FUNCTION dbo.get_classic
go
--table inline function: CREATE FUNCTION ... RETURNS TABLE AS RETURN ( SELECT ... );
create function dbo.get_adults()
	--not inline function:
	returns @tt table
	(
		adult_name nvarchar(254),
		adult_age date
	)
	as
	begin
		insert @tt
			select students.first_name, students.date_of_birth
			from dbo.students
			where dbo.get_age(students.date_of_birth) >= 18
		return
	end
go

alter procedure dbo.sub_proc
	@curs cursor VARYING OUTPUT
as
begin
	set nocount on;
	set @curs = cursor
	scroll static for
		select adult_name, adult_age
		from dbo.get_adults();		--table function
	open @curs;
end
go

declare @another_curs cursor;

EXEC dbo.sub_proc @curs = @another_curs OUTPUT;
fetch next from @another_curs;
while (@@FETCH_STATUS = 0)
begin
	fetch next from @another_curs;
end
close @another_curs;
deallocate @another_curs;
go