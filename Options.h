#pragma once

#include <string>

class Options {
private:
  bool debug;
  std::string fileName;

public:
  Options();

  bool isDebugMode() const;
  void setDebugMode(bool);

  void setFileName(const std::string &name);
  const std::string &getFileName();
  bool hasFileName() const;
};
