//
//  XMLTag.h
//  TackyEngine
//
//  Created by Robert Crosby on 1/6/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __TackyEngine__XMLTag__
#define __TackyEngine__XMLTag__

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>

class XMLTag {
public:
   XMLTag(): _level(0), _parrent(0) {}
   XMLTag(const std::string &e): _element(e), _level(0), _parrent(0) {}
   XMLTag(const std::string &e, const std::string &c): _element(e), _contents(c), _level(0), _parrent(0) {}
   XMLTag(const XMLTag &tag) {*this = tag;}
   ~XMLTag() {
      if (_parrent)
         _parrent->removeSubTag(this);
      clearSubTags();
   }
   
   typedef std::set<XMLTag*>::iterator iterator;
   typedef std::set<XMLTag*>::const_iterator const_iterator;
   
   inline void clearSubTags() {
      while (_subTags.size())
         delete *_subTags.begin();
   }
   inline void addSubTag(XMLTag *tag) {
      tag->_level = 1 + _level;
      tag->_parrent = this;
      _subTags.insert(tag);
   }
   inline void copySubTags(const XMLTag &tag) {
      for (const_iterator itr = tag.begin(); itr != tag.end(); ++itr)
         addSubTag(new XMLTag(**itr));
   }
   inline void copyAttributes(const XMLTag &tag) {_attributes = tag._attributes;}
   inline void removeSubTag(XMLTag *tag) {
      tag->_level = 0;
      tag->_parrent = NULL;
      if (_subTags.find(tag) != _subTags.end())
         _subTags.erase(tag);
   }
   inline bool deleteSubTag(const std::string &e) {
      iterator itr = find(e, begin());
      if (itr == end())
         return false;
      delete *itr;
      return true;
   }
   inline void deleteSubTags(const std::string &e) {while (deleteSubTag(e));}
   
   inline void setAttribute(const std::string &n, const std::string &v) {_attributes[n] = v;}
   inline void removeAttribute(const std::string &n) {_attributes.erase(n);}
   
   inline int getLevel() const {return _level;}
   inline void setLevel(int l) {
      _level = l;
      for (iterator itr = begin(); itr != end(); ++itr)
         (*itr)->setLevel(_level + 1);
   }
   
   inline void operator=(const XMLTag &tag) {
      _element = tag._element;
      _contents = tag._contents;
      _attributes = tag._attributes;
      clearSubTags();
      copySubTags(tag);
   }
   
   inline bool operator==(const std::string &e) const {return _element == e;}
   inline bool operator!=(const std::string &e) const {return _element != e;}
   
   inline void setContents(const std::string &cont) {_contents = cont;}
   
   inline iterator begin() {return _subTags.begin();}
   inline iterator end() {return _subTags.end();}
   
   inline const_iterator begin() const {return _subTags.begin();}
   inline const_iterator end() const {return _subTags.end();}
   
   inline iterator findSubTag(const std::string &e) {return find(e, begin());}
   inline iterator findSubTag(const std::string &e, iterator itr) {return find(e, itr);}
   inline const_iterator findSubTag(const std::string &e) const {return find(e, begin());}
   inline const_iterator findSubTag(const std::string &e, const_iterator itr) const {
      return find(e, itr);
   }
   
   inline std::string getElement() const {return _element;}
   inline std::string getContents() const {return _contents;}
   inline std::string getAttribute(const std::string &a) const {
      std::map<std::string, std::string>::const_iterator itr = _attributes.find(a);
      return itr != _attributes.end() ? itr->second : "";
   }
   
   inline std::string getSubContents(const std::string &e) const {
      const_iterator itr = findSubTag(e);
      return itr != end() ? (*itr)->getContents() : "";
   }
   inline XMLTag* getSubTag(const std::string &e) {
      iterator itr = findSubTag(e);
      return itr != end() ? *itr : NULL;
   }
   inline const XMLTag* getSubTag(const std::string &e) const {
      const_iterator itr = findSubTag(e);
      return itr != end() ? *itr : NULL;
   }
   inline void getSubTags(std::set<XMLTag*> *tags, const std::string &e) {
      for (iterator itr = begin(); itr != end(); ++itr) {
         if (**itr == e)
            tags->insert(*itr);
      }
   }
   
   inline bool hasAttribute(const std::string &a) const {
      return _attributes.find(a) != _attributes.end();
   }
   inline bool hasTag(const std::string &e) const {return findSubTag(e) != end();}
   
   friend std::ostream &operator<<(std::ostream &os, const XMLTag &tag) {return tag.write(os);}
   friend std::istream &operator>>(std::istream &is, XMLTag &tag) {return tag.read(is);}
   
   static XMLTag* ReadXML(std::istream &is);
private:
   inline iterator find(const std::string &e, iterator itr) {
      while (itr != end() && **itr != e)
         ++itr;
      return itr;
   }
   inline const_iterator find(const std::string &e, const_iterator itr) const {
      while (itr != end() && **itr != e)
         ++itr;
      return itr;
   }
   
   std::ostream &write(std::ostream &os) const {
      std::map<std::string, std::string>::const_iterator attItr;
      
      os << std::string(_level*3, ' ') << "<" << _element;
      for (attItr = _attributes.begin(); attItr != _attributes.end(); ++attItr)
         os << " " << attItr->first << "=\"" << attItr->second << "\"";
      if (_subTags.size()) {
         os << ">" << std::endl;
         for (const_iterator sub = begin(); sub != end(); ++sub)
            os << **sub;
         os << std::string(_level*3, ' ') << "</" << _element << ">" << std::endl;
      }
      else if (_contents == "")
         os << "/>" << std::endl;
      else
         os << ">" << _contents << "</" << _element << ">" << std::endl;
      return os;
   }
   
   std::istream &read(std::istream &is) {
      std::list<XMLTag *> stack;
      std::string line;
      
      while (!is.eof() && !stack.size()) {
         std::string::size_type start, end;
         // update for other situations
         getline(is, line);
         start = line.find("<");
         end = line.find(">");
         if (start != std::string::npos && end != std::string::npos) {
            XMLTag *tag = TagFromStr(line.substr(start+1, end-start-1));
            
            tag->_level = _level;
            *this = *tag;
            stack.push_back(this);
            delete tag;
         }
      }
      while (!is.eof()) {
         getline(is, line);
         ParseLine(&stack, line);
      }
      return is;
   }
   
   static void ParseLine(std::list<XMLTag *> *stack, const std::string &line) {
      std::string::size_type loc1, loc2;
      
      loc1 = line.find("<");
      loc2 = line.find(">");
      
      if (loc1 != std::string::npos && loc2 != std::string::npos) {
         ++loc1;
         ParseTag(stack, line.substr(loc1, loc2 - loc1), line.substr(loc2+1));
      }
      else {
         loc1 = line.find_first_not_of("\t ");
         if (loc1 != std::string::npos && stack->size())
            stack->back()->setContents(line.substr(loc1));
      }
   }
   static void ParseTag(std::list<XMLTag *> *stack, const std::string &str1, const std::string &str2) {
      std::string::size_type loc;
      
      loc = str1.find("/");
      
      if (loc == std::string::npos) {
         if (!stack->size())
            stack->push_back(TagFromStr(str1));
         else {
            XMLTag *subTag = TagFromStr(str1);
            stack->back()->addSubTag(subTag);
            stack->push_back(subTag);
         }
         
         if (str2 != "") {
            loc = str2.find("</");
            if (loc != std::string::npos) {
               stack->back()->setContents(str2.substr(0, loc));
               if (stack->size() > 1)
                  stack->pop_back();
            }
            else
               stack->back()->setContents(str2);
         }
      }
      else if (loc == 0) {
         if (stack->size() > 1)
            stack->pop_back();
      }
      else
         stack->back()->addSubTag(TagFromStr(str1));
   }
   
   static XMLTag* TagFromStr(const std::string &str) {
      std::string subStr = str;
      std::string::size_type loc = str.find_first_of(" /");
      XMLTag *tag = new XMLTag(str.substr(0, loc));
      
      loc = str.find_first_not_of(" ", loc);
      while (loc != std::string::npos && subStr[loc] != '/') {
         std::string name;
         subStr = subStr.substr(loc);
         
         loc = subStr.find("=\"");
         if (loc == std::string::npos)
            break;
         name = subStr.substr(0, loc);
         
         subStr = subStr.substr(loc+2);
         loc = subStr.find("\"");
         if (loc == std::string::npos)
            break;
         tag->setAttribute(name, subStr.substr(0, loc));
         
         loc = subStr.find_first_not_of(" ", loc+1);
      }
      
      return tag;
   }
   
   int _level;
   XMLTag *_parrent;
   std::string _element;
   std::string _contents;
   
   std::set<XMLTag*> _subTags;
   std::map<std::string, std::string> _attributes;
};

#endif /* defined(__TackyEngine__XMLTag__) */
