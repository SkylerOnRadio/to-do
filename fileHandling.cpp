#include "header_files/fileHandling.h"
#include "header_files/taskClass.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::unique_ptr<Tasks> parseLineToTask(std::string_view line) {
  int quoteCount{0};
  int i{1};
  std::string_view field;

  int id;
  std::string task;
  std::string category;
  int status;
  bool renewing;
  time_t created_at;
  time_t modified_at;

  int startPoint{0};
  int endPoint{0};

  for (char ch : line) {
    if (ch == ',' && !(quoteCount & 1)) {
      field = line.substr(startPoint, endPoint - startPoint);
      switch (i) {
      case 1:
        id = std::stoi(static_cast<std::string>(field));
        break;

      case 2:
        task = static_cast<std::string>(field);
        break;

      case 3:
        category = static_cast<std::string>(field);
        break;

      case 4:
        renewing = field == "false" ? false : true;
        break;

      case 5:
        status = std::stoi(static_cast<std::string>(field));
        break;

      case 6: {
        std::istringstream ss{static_cast<std::string>(field)};
        struct std::tm tmp_time{0};
        ss >> std::get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (!ss.fail())
          created_at = mktime(&tmp_time);
        break;
      }

      case 7: {
        std::istringstream ss{static_cast<std::string>(field)};
        struct std::tm tmp_time{0};
        ss >> std::get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (!ss.fail())
          modified_at = mktime(&tmp_time);
        break;
      }

      default:
        break;
      }

      startPoint = endPoint + 1;
      i++;
    }
    if (ch == '"')
      quoteCount++;
    endPoint++;
  }

  return std::make_unique<Tasks>(
      Tasks(id, task, status, renewing, created_at, modified_at, category));
}

int loadFile(std::string filename,
             std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  std::ifstream file{filename};
  if (!file.is_open())
    return -1;

  std::string line;
  while (std::getline(file, line)) {
    tasksMain.push_back(parseLineToTask(line));
  }

  return 0;
}

std::string parseCSVToString(std::string_view csvStr) {
  std::string parsedText{""};

  for (int i = 0; i < csvStr.length(); ++i) {
    if ((csvStr.at(i) == '"') &&
        (i + 1 < csvStr.length() ? (csvStr.at(i + 1) == '"') : false)) {
      parsedText += '"';
      i += 1;
      continue;
    } else if (csvStr.at(i) == '"')
      continue;
    parsedText += csvStr.at(i);
  }
  return parsedText;
}
