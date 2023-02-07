from os import listdir
from os.path import isfile, join
import re

# путь к папке с файлами
path = "."

files = [f for f in listdir(path) if isfile(join(path, f))]

pattern = re.compile("(.*.h|.*.cpp)$") # здесь можно указать нужные расширения
# pattern = re.compile(".*.py$") # вариант для питонистов
code_files = filter(lambda x : bool(pattern.match(x)), files)

outstring = ""
for filename in code_files:
    file = open(path + "/" + filename, "r")
    # замените c++ на python, если вы питонист:
    outstring += filename + "\n```c++\n" + file.read() + "\n```\n" 

# сохраняем в файл
outfile = open("report.md", "w")
outfile.write(outstring)