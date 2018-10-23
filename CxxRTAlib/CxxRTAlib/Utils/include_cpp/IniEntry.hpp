/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef INIENTRY_HPP
#define INIENTRY_HPP

#include <string>

/**
 * Class for storing entries.
 */
class IniEntry {
public:
  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws might throw exceptions (see std::string (constructor))
   */
  IniEntry(std::string key, std::string value = "", std::string comment = "")
      : key(key), value(value), comment(comment) {}

  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, float value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}

  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, double value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}
  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, long double value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}

  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, int value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}
  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, long value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}

  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, long long value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}

  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, unsigned long value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}

  /**
   * Constructor for the entry.
   * @param key
   * @param value (optional)
   * @param comment (optional)
   * @throws std::bad_alloc or other (see std::string (constructor))
   */
  IniEntry(std::string key, unsigned long long value, std::string comment = "")
      : key(key), value(std::to_string(value)), comment(comment) {}

  // string

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   */
  IniEntry operator=(const std::string &other) {
    value = other;
    return *this;
  }

  // floating point

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const float &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const double &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const long double &other) {
    value = std::to_string(other);
    return *this;
  }

  // integer - signed

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const int &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const long &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const long long &other) {
    value = std::to_string(other);
    return *this;
  }

  // integer - unsigned

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const unsigned &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const unsigned long &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Equals operator which also assigns a new value.
   * @param value
   * @return current object
   * @throws std::bad_alloc
   */
  IniEntry operator=(const unsigned long long &other) {
    value = std::to_string(other);
    return *this;
  }

  /**
   * Comparison operator.
   * @param value
   * @return true if the values are equal
   */
  bool operator==(const IniEntry &other) const { return value == other.value; }

  /**
   * Unequal comparison operator.
   * @param value
   * @return false if the values are equal
   */
  bool operator!=(const IniEntry &other) const { return !operator==(other); }

  // string

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const std::string &other) { value = other; }

  // floating point

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const float &other) { value = std::to_string(other); }

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const double &other) { value = std::to_string(other); }

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const long double &other) { value = std::to_string(other); }

  // integer - signed

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const int &other) { value = std::to_string(other); }

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const long &other) { value = std::to_string(other); }

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const long long &other) { value = std::to_string(other); }

  // integer - unsigned

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const unsigned &other) { value = std::to_string(other); }

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const unsigned long &other) { value = std::to_string(other); }

  /**
   * Setter for the value.
   * @param value
   * @throws std::bad_alloc
   */
  void setValue(const unsigned long long &other) {
    value = std::to_string(other);
  }

  /**
   * Getter for the key.
   * @return key
   */
  std::string getKey() const { return key; }

  // strings

  /**
   * Getter for the value (as string).
   * @return value
   */
  std::string getString() const { return value; }

  // floating point

  /**
   * Getter for the value (as float).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  float getFloat() const { return std::stof(value); }

  /**
   * Getter for the value (as double).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  double getDouble() const { return std::stod(value); }

  /**
   * Getter for the value (as long double).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  long double getLongDouble() const { return std::stold(value); }

  // integer - signed

  /**
   * Getter for the value (as int).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  int getInt() const { return std::stoi(value); }

  /**
   * Getter for the value (as long).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  long getLong() const { return std::stol(value); }

  /**
   * Getter for the value (as long long ).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  long long getLongLong() const { return std::stoll(value); }

  // integer - unsigned

  /**
   * Getter for the value (as unsigned long).
   * @return
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  unsigned long getUnsignedLong() const { return std::stoul(value); }

  /**
   * Getter for the value (as unsigned long long).
   * @return value
   * @throws std::invalid_argument on conversion failure and std::out_of_range
   * if the value is out if range
   */
  unsigned long long getUnsignedLongLong() const { return std::stoull(value); }

  /**
   * Setter for the comment.
   * @return comment
   */
  void setComment(std::string comment) { this->comment = comment; }

  /**
   * Getter for the comment.
   * @return comment
   */
  std::string getComment() { return comment; }

protected:
private:
  std::string key;
  std::string value;
  std::string comment;
};

#endif // INIENTRY_HPP
