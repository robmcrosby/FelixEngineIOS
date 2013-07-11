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
#include <list>


/**
 * XMLTag can be read from and written to an XML file. It can also
 * be used to explore and change an XML structure as well.
 */
class XMLTag {
public:
   /**
    * Default constructor.
    */
   XMLTag(): _level(0), _parrent(0) {}
   
   /**
    * Construct an XMLTag with an element name.
    * @param e string for the element name.
    */
   XMLTag(const std::string &e): _element(e), _level(0), _parrent(0) {}
   
   /**
    * Construct an XMLTag with an element name and contents.
    * @param e string for the element name.
    * @param c string for the contents of the new tag.
    */
   XMLTag(const std::string &e, const std::string &c): _element(e), _contents(c), _level(0), _parrent(0) {}
   
   /**
    * Copy constructor uses assignment operator to copy the given XMLTag.
    * @param tag XMLTag to be copied from.
    */
   XMLTag(const XMLTag &tag) {*this = tag;}
   
   /**
    * Destructor deletes the Subtags.
    */
   ~XMLTag() {
      if (_parrent)
         _parrent->removeSubTag(this);
      clearSubTags();
   }
   
   
   /**
    * Definition of an iterator for Subtags.
    */
   typedef std::list<XMLTag*>::iterator iterator;
   
   /**
    * Definition of a const iterator for Subtags.
    */
   typedef std::list<XMLTag*>::const_iterator const_iterator;
   
   
   /**
    * Deletes and clears all of the Subtags.
    */
   inline void clearSubTags() {
      while (_subTags.size())
         delete *_subTags.begin();
   }
   
   /**
    * Adds a valid Subtag to the Subtag collection.
    * @param [in] tag Pointer to an XMLTag object which this takes
    * ownership.
    */
   inline void addSubTag(XMLTag *tag) {
      tag->_level = 1 + _level;
      tag->_parrent = this;
      _subTags.push_back(tag);
   }
   
   /**
    * Makes a deep copy of the subtags of the given tag.
    * @param tag XMLTag that will have its Subtags copied from.
    */
   inline void copySubTags(const XMLTag &tag) {
      for (const_iterator itr = tag.begin(); itr != tag.end(); ++itr)
         addSubTag(new XMLTag(**itr));
   }
   
   /**
    * Copies the Attributes map from the given tag.
    * @param tag XMLTag that will have its Attributes copied from.
    */
   inline void copyAttributes(const XMLTag &tag) {
      _attributes = tag._attributes;
   }
   
   
   /**
    * Attempts to remove the given tag from the Subtags collection
    * with out deletion.
    * @param [in,out] tag Pointer to an XMLTag that is to be removed.
    */
   inline void removeSubTag(XMLTag *tag) {
      std::list<XMLTag*>::iterator itr;
      
      itr = std::find(_subTags.begin(), _subTags.end(), tag);
      if (itr != _subTags.end()) {
         tag->_level = 0;
         tag->_parrent = NULL;
         _subTags.erase(itr);
      }
   }
   
   /**
    * Attempts to delete the first occurence of an XMLTag with
    * the given element name.
    * @param e string for the Elemenent name.
    * @return true if a deletion or false if not.
    */
   inline bool deleteSubTag(const std::string &e) {
      iterator itr = find(e, begin());
      if (itr == end())
         return false;
      delete *itr;
      return true;
   }
   
   /**
    * Deletes all of the XMLTags with the given element name.
    * @param e string for the Elemenent name.
    */
   inline void deleteSubTags(const std::string &e) {
      while (deleteSubTag(e))
         ;
   }
   
   /**
    * Sets an Attribute in the Attribute map.
    * @param n string for the name/key of the Attribute.
    * @param v string for the value of the Attribute.
    */
   inline void setAttribute(const std::string &n, const std::string &v) {
      _attributes[n] = v;
   }
   
   /**
    * Removes an Attribute with the given name.
    * @param n string for the name/key of the Attribute to remove.
    */
   inline void removeAttribute(const std::string &n) {
      _attributes.erase(n);
   }
   
   
   /**
    * Getter for the number of XMLTags that wrap this XMLTag.
    * @return int for the level.
    */
   inline int getLevel() const {return _level;}
   
   /**
    * Sets the level of this XMLTag and updates is Subtags.
    * @param l int for the new level.
    */
   inline void setLevel(int l) {
      _level = l;
      for (iterator itr = begin(); itr != end(); ++itr)
         (*itr)->setLevel(_level + 1);
   }
   
   
   /**
    * Assignment operator copies the given XMLTag.
    * @param tag XMLTag to be copied from.
    */
   inline void operator=(const XMLTag &tag) {
      _element = tag._element;
      _contents = tag._contents;
      _attributes = tag._attributes;
      clearSubTags();
      copySubTags(tag);
   }
   
   
   /**
    * Comparison of XMLTag element with the given string.
    * @param e string to compare with Element name.
    * @return true if the strings match or false if not.
    */
   inline bool operator==(const std::string &e) const {
      return _element == e;
   }
   
   /**
    * Comparison of XMLTag element with the given string.
    * @param e string to compare with Element name.
    * @return false if the strings match or true if not.
    */
   inline bool operator!=(const std::string &e) const {
      return _element != e;
   }
   
   /**
    * Set the contents of this XMLTag.
    * @param c string for the new contents.
    */
   inline void setContents(const std::string &c) {_contents = c;}
   
   
   /**
    * Gets the begining iterator for the Subtags collection.
    * @return iterator to the begining of the Subtags.
    */
   inline iterator begin() {return _subTags.begin();}
   
   /**
    * Gets the ending iterator for the Subtags collection.
    * @return iterator to the ending of the Subtags.
    */
   inline iterator end() {return _subTags.end();}
   
   
   /**
    * Gets the begining const iterator for the Subtags collection.
    * @return const iterator to the begining of the Subtags.
    */
   inline const_iterator begin() const {return _subTags.begin();}
   
   /**
    * Gets the const ending iterator for the SubTags collection.
    * @return const iterator to the ending of the Subtags.
    */
   inline const_iterator end() const {return _subTags.end();}
   
   
   /**
    * Finds the first occurence of an XMLTag with the given Element name.
    * @param e string element name.
    * @return iterator to the found XMLTag or Iterator to the SubTags
    * end if not found.
    */
   inline iterator findSubTag(const std::string &e) {
      return find(e, begin());
   }
   
   /**
    * Finds the first occurence of an XMLTag with the given Element name
    * after the given iterator position in the SubTags collection.
    * @param e string element name.
    * @param itr iterator to the starting point of the search.
    * @return iterator to the found XMLTag or iterator to the SubTags
    * end if not found.
    */
   inline iterator findSubTag(const std::string &e, iterator itr) {
      return find(e, itr);
   }
   
   /**
    * Finds the first occurence of an XMLTag with the given Element name.
    * @param e string element name.
    * @return const iterator to the found XMLTag or Iterator to the Subtags
    * end if not found.
    */
   inline const_iterator findSubTag(const std::string &e) const {
      return find(e, begin());
   }
   
   /**
    * Finds the first occurence of an XMLTag with the given Element name
    * after the given iterator position in the SubTags collection.
    * @param e string element name.
    * @param itr iterator to the starting point of the search.
    * @return const iterator to the found XMLTag or iterator to the SubTags
    * end if not found.
    */
   inline const_iterator findSubTag(const std::string &e, const_iterator itr) const {
      return find(e, itr);
   }
   
   
   /**
    * Gets the Element name of this XMLTag.
    * @return string for the Element name.
    */
   inline std::string getElement() const {return _element;}
   
   /**
    * Gets the Contents of this XMLTag.
    * @return string for the Contents.
    */
   inline std::string getContents() const {return _contents;}
   
   /**
    * Gets the Attribute value for the given name/key.
    * @param n string for the name/key of the attribute.
    * @return string for the value or an empty string if not found.
    */
   inline std::string getAttribute(const std::string &n) const {
      std::map<std::string, std::string>::const_iterator itr = _attributes.find(n);
      return itr != _attributes.end() ? itr->second : "";
   }
   
   /**
    * Attempts to get the Contents of the first Subtag with the given
    * Element name.
    * @param e string for the Element name to search for.
    * @return string of the contents of the found Subtag or empty
    * empty string if not found.
    */
   inline std::string getSubContents(const std::string &e) const {
      const_iterator itr = findSubTag(e);
      return itr != end() ? (*itr)->getContents() : "";
   }
   
   /**
    * Gets the first occurence of a Subtag with the given Element name.
    * @param e string for the element name to search for.
    * @return pointer to the found XMLTag or NULL if not found.
    */
   inline XMLTag* getSubTag(const std::string &e) {
      iterator itr = findSubTag(e);
      return itr != end() ? *itr : NULL;
   }
   
   /**
    * Gets the first occurence of a Subtag with the given Element name.
    * @param e string for the element name to search for.
    * @return const pointer to the found XMLTag or NULL if not found.
    */
   inline const XMLTag* getSubTag(const std::string &e) const {
      const_iterator itr = findSubTag(e);
      return itr != end() ? *itr : NULL;
   }
   
   /**
    * Checks if an Attribute is present in the Attribute Map.
    * @return true if the Attribute is in the Attribute Map or false if not. 
    */
   inline bool hasAttribute(const std::string &a) const {
      return _attributes.find(a) != _attributes.end();
   }
   
   /**
    * Checks if the Subtags collection contains an XMLTag with the given element
    * name.
    * @param e string for the Element name to search for.
    * @return true if there is a SubTag with the Element name or false if not.
    */
   inline bool hasSubTag(const std::string &e) const {
      return findSubTag(e) != end();
   }
   
   
   /**
    * Write an XMLTag and its SubTags to the given output stream.
    * @param os output stream to be written to.
    * @param tag XMLTag that will write to the output stream.
    * @return output stream that was written to by the XMLTag.
    */
   friend std::ostream &operator<<(std::ostream &os, const XMLTag &tag) {
      return tag.write(os);
   }
   
   /**
    * Read into an XMLTag and its SubTags from the given input stream.
    * @param is input stream to be read from.
    * @param tag XMLTag that will read from the input stream.
    * @return input stream that was read from by the XMLTag.
    */
   friend std::istream &operator>>(std::istream &is, XMLTag &tag) {
      return tag.read(is);
   }
   
private:
   
   /**
    * Finds the first occurence of an XMLTag with the given Element name
    * after the given iterator position in the SubTags collection.
    * @param e string element name.
    * @param itr iterator to the starting point of the search.
    * @return iterator to the found XMLTag or iterator to the SubTags
    * end if not found.
    */
   inline iterator find(const std::string &e, iterator itr) {
      while (itr != end() && **itr != e)
         ++itr;
      return itr;
   }
   
   /**
    * Finds the first occurence of an XMLTag with the given Element name
    * after the given iterator position in the SubTags collection.
    * @param e string element name.
    * @param itr iterator to the starting point of the search.
    * @return const iterator to the found XMLTag or iterator to the SubTags
    * end if not found.
    */
   inline const_iterator find(const std::string &e, const_iterator itr) const {
      while (itr != end() && **itr != e)
         ++itr;
      return itr;
   }
   
   
   /**
    * Writes this XMLTag and SubTags to the given output stream.
    * @param os output stream to be written to.
    * @return output stream that was written to.
    */
   std::ostream &write(std::ostream &os) const {
      std::map<std::string, std::string>::const_iterator attItr;
      
      /* write the Element name in the opening tag. */
      os << std::string(_level*3, ' ') << "<" << _element;
      
      /* write the Attributes in the opening tag. */
      for (attItr = _attributes.begin(); attItr != _attributes.end(); ++attItr)
         os << " " << attItr->first << "=\"" << attItr->second << "\"";
      
      if (_subTags.size()) {
         /* Recursively write the SubTags between the opening an closing tags
          * assuming an empty Contents string. */
         os << ">" << std::endl;
         for (const_iterator sub = begin(); sub != end(); ++sub)
            os << **sub;
         os << std::string(_level*3, ' ') << "</" << _element << ">" << std::endl;
      }
      else if (_contents == "") {
         /* close to a single tag if no Contents. */
         os << "/>" << std::endl;
      }
      else {
         /* write the Contents between opening and closing tags */
         os << ">" << _contents << "</" << _element << ">" << std::endl;
      }
      
      return os;
   }
   
   /**
    * Reads the given input stream to this XMLTag and SubTags.
    * @param is input stream to read from.
    * @return input stream that was read from.
    */
   std::istream &read(std::istream &is) {
      std::list<XMLTag *> stack;
      std::string line;
      
      /* Get the first outer tag. */
      while (!is.eof() && !stack.size()) {
         std::string::size_type start, end;
         
         getline(is, line);
         start = line.find("<");
         end = line.find(">");
         
         /* Check for a complete tag */
         if (start != std::string::npos && end != std::string::npos) {
            XMLTag *tag = TagFromStr(line.substr(start+1, end-start-1));
            
            /* Copy the tag and add self to stack */
            tag->_level = _level;
            *this = *tag;
            stack.push_back(this);
            
            /* Clean up the original tag. */
            delete tag;
         }
      }
      
      /* Parse the rest of the input stream */
      while (!is.eof()) {
         getline(is, line);
         ParseLine(&stack, line);
      }
      
      return is;
   }
   
   /**
    * Parses a line for either adding to or removing tags from the stack.
    * @param [in, out] stack pointer to the stack of parrent XMLTags.
    * @param line string of the line to be parsed.
    */
   static void ParseLine(std::list<XMLTag *> *stack, const std::string &line) {
      std::string::size_type loc1, loc2;
      
      loc1 = line.find("<");
      loc2 = line.find(">");
      
      if (loc1 != std::string::npos && loc2 != std::string::npos) {
         /* Found either a new or closing tag. */
         ++loc1;
         ParseTag(stack, line.substr(loc1, loc2 - loc1), line.substr(loc2+1));
      }
      else {
         /* Check for any Contents */
         loc1 = line.find_first_not_of("\t ");
         if (loc1 != std::string::npos && stack->size())
            stack->back()->setContents(line.substr(loc1));
      }
   }
   
   /**
    * Attempts to parse a tag from the set of strings.
    * @param [in, out] stack pointer to the stack of parrent XMLTags.
    * @param str1 string potentially holding an Element name and Attributes.
    * @param str2 string potentially holding Conents or SubTags.
    */
   static void ParseTag(std::list<XMLTag *> *stack, const std::string &str1, const std::string &str2) {
      std::string::size_type loc;
      
      /* Check for a close. */
      loc = str1.find("/");
      
      if (loc == std::string::npos) {
         /* No close */
         if (!stack->size())
            stack->push_back(TagFromStr(str1));
         else {
            XMLTag *subTag = TagFromStr(str1);
            stack->back()->addSubTag(subTag);
            stack->push_back(subTag);
         }
         
         /* Check for Contents */
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
         /* Closing tag */
         if (stack->size() > 1)
            stack->pop_back();
      }
      else {
         /* Self closing tag */ 
         stack->back()->addSubTag(TagFromStr(str1));
      }
   }
   
   /**
    * Creates an XMLTag from a string for its Element and potential Attributes.
    * @param str string holding an Element name and potential Attributes.
    */
   static XMLTag* TagFromStr(const std::string &str) {
      std::string subStr = str;
      std::string::size_type loc = str.find_first_of(" /");
      XMLTag *tag = new XMLTag(str.substr(0, loc));
      
      /* Attempt to parse any Attributes */
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
   
   int _level;            /**< int for how many levels deep */
   XMLTag *_parrent;      /**< pointer to Parrent XMLTag or NULL if no Parrent */
   std::string _element;  /**< string for Element name */
   std::string _contents; /**< string for Contents */
   
   std::list<XMLTag*> _subTags; /**< collection of SubTags */
   std::map<std::string, std::string> _attributes; /**< map of Attributes */
};

#endif /* defined(__TackyEngine__XMLTag__) */
