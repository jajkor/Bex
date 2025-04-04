#pragma once

#include <string>

class Options {
private:
  bool debug;
  std::string fileName;

public:
  Options();

  bool getDebug();
  void setDebug(bool);

  void setFileName(const std::string &name);
  const std::string &getFileName();
  bool hasFileName() const;
};
