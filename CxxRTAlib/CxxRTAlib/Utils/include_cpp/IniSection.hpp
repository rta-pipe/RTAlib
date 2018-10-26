/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INISECTION_HPP
#define INISECTION_HPP

#include <string>
#include <vector>

#include "IniEntry.hpp"

/**
 * Class for storing sections.
 */
class IniSection {
public:
  /**
   * Constructor for the section.
   * @param name (optional)
   * @param comment (optional)
   * @throws might throw exceptions (see std::string (constructor))
   */
  IniSection(std::string name = "", std::string comment = "")
      : name(name), comment(comment) {}

  /**
   * Returns the number of entries.
   * @param number of entries
   */
  size_t size() const { return entries.size(); }

  /**
   * Returns whether the section is empty or not
   * @param true, if it is empty
   */
  bool empty() const { return entries.empty(); }

  /**
   * Returns a std::vector of all entries.
   * @param all entries
   */
  std::vector<IniEntry> getAllEntries() const { return entries; }

  /**
   * Getter for the name.
   * @param name
   */
  std::string getName() const { return name; }

  /**
   * Getter for the value of an entry with a given name.
   * @param entry
   * @throws might throw exceptions (see std::vector::push_back)
   */
  IniEntry &at(const std::string &key) {
    for (unsigned int i = 0; i < entries.size(); ++i)
      if (entries.at(i).getKey() == key)
        return entries.at(i);

    entries.push_back(IniEntry(key));
    return entries.at(entries.size() - 1);
  }

  /**
   * Getter for the value of an entry with a given name.
   * @param entry
   * @throws might throw exceptions (see std::vector::push_back)
   */
  IniEntry &operator[](const std::string &key) { return at(key); }

  /**
   * Comparison operator.
   * @param section
   * @return true if the values are equal
   */
  bool operator==(const IniSection &other) const {
    return (entries == other.entries && name == other.name &&
            comment == other.comment);
  }

  /**
   * Unequal comparison operator.
   * @param section
   * @return false if the values are equal
   */
  bool operator!=(const IniSection &other) const { return !operator==(other); }

  /**
   * Deletes all entries without a value.
   */
  void clean() {
    for (int i = entries.size() - 1; i >= 0; --i)
      if (entries.at(i).getString() == "")
        entries.erase(entries.begin() + i);
  }

  /**
   * Deletes all entries.
   */
  void clear() { entries.clear(); }

  /**
   * Inserts an entry.
   * @param entry
   * @throws might throw exceptions (see std::vector::push_back)
   */
  void insert(IniEntry entry) { entries.push_back(entry); }

  /**
   * Erases an entry with a given name.
   * @param entry
   */
  void erase(const std::string &key) {
    for (unsigned int i = 0; i < entries.size(); ++i)
      if (entries.at(i).getKey() == key)
        entries.erase(entries.begin() + i);
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

protected:
private:
  std::string name;
  std::string comment;
  std::vector<IniEntry> entries;
};

#endif // INISECTION_HPP
