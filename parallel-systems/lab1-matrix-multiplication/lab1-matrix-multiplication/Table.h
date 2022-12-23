#include <fstream>
#include <string>
#include <vector>

class Table {
private:
	std::string data = "";

public:
	Table() {
		data = R"(
<?xml version="1.0"?>
<Workbook xmlns="urn:schemas-microsoft-com:office:spreadsheet"
 xmlns:o="urn:schemas-microsoft-com:office:office"
 xmlns:x="urn:schemas-microsoft-com:office:excel"
 xmlns:ss="urn:schemas-microsoft-com:office:spreadsheet"
 xmlns:html="http://www.w3.org/TR/REC-html40">
 <Worksheet ss:Name="Sheet1">
  <Table x:FullColumns="1" x:FullRows="1">
)";
	}

	void add_row(std::vector<int> nums) {
		data += "<Row>";
		for (const auto& num : nums) {
			data += "<Cell><Data ss:Type=\"Number\">" + std::to_string(num) + "</Data></Cell>";
		}
		data += "</Row>";
	}

	void add_row(std::vector<std::string> strs) {
		data += "<Row>";
		for (const auto& str : strs) {
			data += "<Cell><Data ss:Type=\"String\">" + str + "</Data></Cell>";
		}
		data += "</Row>";
	}

	void save(std::string filename) {
		auto _data = data + R"(  </Table>
 </Worksheet>
</Workbook>)";
		std::ofstream file(filename);
		file << _data;
		file.close();
	}
};