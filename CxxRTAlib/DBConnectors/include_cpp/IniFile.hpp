/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INIFILE_HPP
#define INIFILE_HPP

#include <exception>
#include <string>
#include <vector>

#include "IniSection.hpp"

/**
 * Class to hold INI data.
 */
class IniFile {
public:
  /**
   * Constructor for the INI file.
   * @param comment (optional)
   * @throws might throw exceptions (see std::string constructor)
   */
  IniFile(std::string comment = "") : comment(comment) {}

  /**
   * Comparison operator.
   * @param file
   * @return true if the values are equal
   */
  bool operator==(const IniFile &other) const {
    return (sections == other.sections && comment == other.comment);
  }

  /**
   * Unequal comparison operator.
   * @param file
   * @return false if the values are equal
   */
  bool operator!=(const IniFile &other) const { return !operator==(other); }

  /**
   * Returns the number of currently loaded sections.
   * @return number of sections
   */
  size_t size() const { return sections.size(); }

  /**
   * Tests if there are currently any sections loaded.
   * @return true, if there are sections present
   */
  bool empty() const { return sections.empty(); }

  /**
   * Returns an std::vector of all currently loaded sections
   * @return sections
   */
  std::vector<IniSection> getAllSections() const { return sections; }

  /**
   * Returns the section with the given name.
   * @param section name
   * @return section
   * @throws might throw exceptions (see std::string construtor)
   */
  IniSection &at(const std::string &name = "") {
    for (unsigned int i = 0; i < sections.size(); ++i)
      if (sections.at(i).getName() == name)
        return sections.at(i);

    sections.push_back(IniSection(name));
    return sections.at(sections.size() - 1);
  }

  /**
   * Returns the section with the given name.
   * @param section name
   * @return section
   * @throws might throw exceptions (see std::string construtor)
   */
  IniSection &operator[](const std::string &name) { return at(name); }

  /**
   * cleans out all empty sections and empty entries.
   */
  void clean() {
    for (int i = sections.size() - 1; i >= 0; --i) {
      sections.at(i).clean();
      if (sections.at(i).empty())
        sections.erase(sections.begin() + i);
    }
  }

  /**
   * Deletes all sections.
   */
  void clear() { sections.clear(); }

  /**
   * Inserts a section.
   * @param section
   * @throws might throw exceptions (see std::vector::push_back)
   */
  void insert(const IniSection &section) { sections.push_back(section); }

  /**
   * Erases a section with a given name.
   * @param section
   */
  void erase(const std::string &sectionName) {
    for (unsigned int i = 0; i < sections.size(); ++i)
      if (sections.at(i).getName() == sectionName)
        sections.erase(sections.begin() + i);
  }

  /**
   * Setter for the comment.
   * @param comment
   */
  void setComment(std::string comment) { this->comment = comment; }

  /**
   * Getter for the comment.
   * @return comment
   */
  std::string getComment() { return comment; }

private:
  std::string comment;
  std::vector<IniSection> sections;
};

#endif // INIFILE_HPP
