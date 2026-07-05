#include "header_files/fileHandling.h"
#include "header_files/taskClass.h"
#include <cstdlib>
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

bool validateLine(std::string_view csvStr) {
  int numOfComma{0};
  for (char s : csvStr)
    if (s == ',')
      ++numOfComma;

  if (numOfComma != 6)
    return false;

  return true;
}

bool isNewDay(time_t modified, time_t current) {
  tm lastModified = *localtime(&modified);
  tm currentTime = *localtime(&current);

  return (lastModified.tm_yday != currentTime.tm_yday) ||
         (lastModified.tm_year != currentTime.tm_year);
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

std::unique_ptr<Tasks> parseLineToTask(std::string_view line) {
  bool valid = validateLine(line);
  if (!valid) {

    std::cout
        << "The CSV format contains fields with incorrect formatting. The "
           "operation was aborted to ensure the tasks remain unchanged.\n"
        << "Please fix the CSV by either running the app with the --updateFile "
           "flag if you updated the app from v1 to v2, else manually ensure "
           "all the fields are present.\n";
    exit(EXIT_FAILURE);
  }

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
    if ((ch == ',') && !(quoteCount & 1)) {
      field = line.substr(startPoint, endPoint - startPoint);
      switch (i) {
      case 1:
        id = std::stoi(parseCSVToString(field));
        break;

      case 2:
        task = parseCSVToString(field);
        break;

      case 3:
        category = parseCSVToString(field);
        break;

      case 4:
        status = std::stoi(parseCSVToString(field));
        break;

      case 5:
        renewing = field == "false" ? false : true;
        break;

      case 6: {
        std::istringstream ss{static_cast<std::string>(field)};
        struct std::tm tmp_time{0};
        ss >> std::get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
        if (!ss.fail())
          created_at = mktime(&tmp_time);
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

    // standard CSV has no comma at the end of the line, so the last feild is
    // not triggered and we have to handle it separately
    if (i == 7) {
      field = line.substr(startPoint, endPoint - startPoint);
      std::istringstream ss{static_cast<std::string>(field)};
      struct std::tm tmp_time{0};
      ss >> std::get_time(&tmp_time, "%Y-%m-%d:%H-%M-%S");
      if (!ss.fail())
        modified_at = mktime(&tmp_time);
    }
  }

  // if the modified time is over 24 hours and it is a renewing task
  if (renewing && isNewDay(modified_at, time(nullptr))) {
    status = 0;
    modified_at = time(nullptr);
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

  std::filesystem::rename(dirName / outputFile, dirName / ".tasks.csv");

  return 0;
}
