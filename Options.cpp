#include "Options.h"

Options::Options() : debug(false) {}

bool Options::getDebug() { return debug; }
void Options::setDebug(bool val) { debug = val; }

void Options::setFileName(const std::string &name) { fileName = name; }
const std::string &Options::getFileName() { return fileName; }
bool Options::hasFileName() const { return !fileName.empty(); }
