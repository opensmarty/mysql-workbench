/* 
 * Copyright (c) 2012, 2015, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#pragma once

#include "mysql-parser-common.h"

/**
 * C++ interface for the ANTLR based MySQL parser.
 */

class MySQLRecognizer;

/**
 * The tree walker allows to easily navigate the AST generated by the recognizer and get all kind
 * of token information (like position, type, content etc.).
 */
class PARSERS_PUBLIC_TYPE MySQLRecognizerTreeWalker : public RecognizerTreeWalker
{
  friend class MySQLRecognizer; // For protected c-tor.
public:
  MySQLRecognizer *recognizer();

  void goToSubQueryStart();
  MySQLQueryType getCurrentQueryType();
  MySQLQueryType getMainQueryType();

protected:
  MySQLRecognizerTreeWalker(MySQLRecognizer *recognizer, pANTLR3_BASE_TREE tree);
  
private:
  MySQLQueryType queryType();
};

class PARSERS_PUBLIC_TYPE MySQLRecognizer : public MySQLRecognitionBase
{
  friend class MySQLRecognizerTreeWalker;
public:
  MySQLRecognizer(long server_version, const std::string &sql_mode, const std::set<std::string> &charsets);
  virtual ~MySQLRecognizer();
  
  void parse(const char *text, size_t length, bool is_utf8, MySQLParseUnit parse_unit);

  std::string dump_tree();
  std::string dump_tree(pANTLR3_BASE_TREE tree, const std::string &indentation);
  
  MySQLRecognizerTreeWalker tree_walker();
  
  virtual void set_sql_mode(const std::string &new_mode);
  virtual void set_server_version(long new_version);
  virtual std::string text() const;
  virtual const char* lineStart() const;

  long server_version();

  ParserToken token_at_index(ANTLR3_MARKER index);

protected:

private:
  class Private;
  Private *d;
};
