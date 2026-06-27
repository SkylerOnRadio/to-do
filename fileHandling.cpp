#include "header_files/fileHandling.h"
#include "header_files/taskClass.h"
#include <cstdio>
#include <ctime>
#include <filesystem>
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

std::string parseStringToCSV(std::string_view str) {
  std::string parsedText{""};
  bool addQuotes{false};

  for (char c : str) {
    if (c == '"') {
      parsedText += '"';
      addQuotes = true;
    } else if (c == ',' || c == '\n') {
      addQuotes = true;
    }

    parsedText += c;
  }

  if (addQuotes)
    parsedText = '"' + parsedText + '"';

  return parsedText;
}

int saveToFile(std::string filename,
               std::vector<std::unique_ptr<Tasks>> &tasksMain) {
  std::ofstream file{filename};

  for (const auto &taskObj : tasksMain) {
    // tm is a struct that contains the seconds, days, etc dince epoch time
    // time_t is int_64t underneath and is an alias
    // localtime takes a integers and converts it to the tm struct

    std::string created_at(100, 2);
    created_at.resize(strftime(&created_at[0], created_at.size(),
                               "%Y-%m-%d:%H-%M-%S",
                               localtime(&taskObj->created_at)));
    std::string modified_at(100, 2);
    modified_at.resize(strftime(&modified_at[0], modified_at.size(),
                                "%Y-%m-%d:%H-%M-%S",
                                localtime(&taskObj->modified_at)));

    file << parseStringToCSV(std::to_string(taskObj->id)) << ","
         << parseStringToCSV(taskObj->task) << ","
         << parseStringToCSV(taskObj->category) << ","
         << parseStringToCSV(std::to_string(taskObj->status)) << ","
         << parseStringToCSV(taskObj->renewing ? "true" : "false") << ","
         << parseStringToCSV(created_at) << "," << parseStringToCSV(modified_at)
         << "\n";
  }

  return 0;
}

int parseLineAndAddToOutputFile(std::ofstream &file, std::string_view line) {
  int quoteCount{0};
  int i{1};
  std::string_view field;

  std::string id;
  std::string task;
  std::string category;
  std::string status;
  std::string created_at;
  std::string modified_at;

  int startPoint{0};
  int endPoint{0};

  for (char ch : line) {
    if (ch == ',' && !(quoteCount & 1)) {
      field = line.substr(startPoint, endPoint - startPoint);
      switch (i) {
      case 1:
        id = static_cast<std::string>(field);
        break;

      case 2:
        task = static_cast<std::string>(parseCSVToString(field));
        break;

      case 3:
        status = field == "False" ? "0" : "2";
        break;

      case 4: {
        created_at = static_cast<std::string>(field);
        break;
      }

      case 5: {
        modified_at = static_cast<std::string>(field);
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

  if (i == 6) {
    field = line.substr(startPoint, endPoint - startPoint);
    category = std::string(field);
  }

  file << parseStringToCSV(id) << "," << parseStringToCSV(task) << ","
       << parseStringToCSV(category) << "," << parseStringToCSV(status) << ","
       << parseStringToCSV("false") << "," << parseStringToCSV(created_at)
       << "," << parseStringToCSV(modified_at) << "\n";

  return 0;
}

int updatePreviousVersionFile(std::string filename) {
  std::ifstream file{filename};
  std::string outputFile = "output_" + filename;
  std::ofstream oFile{outputFile};

  std::string line;
  int inputLine{0};
  while (std::getline(file, line)) {
    parseLineAndAddToOutputFile(oFile, line);
    ++inputLine;
  }

  std::filesystem::path filePath{filename};
  std::filesystem::path dirName = filePath.parent_path();
  if (dirName.empty())
    dirName = ".";

  std::cout << filePath << "\t" << dirName << "\n";

  std::filesystem::rename(dirName / outputFile, dirName / ".tasks.csv");

  return 0;
}
