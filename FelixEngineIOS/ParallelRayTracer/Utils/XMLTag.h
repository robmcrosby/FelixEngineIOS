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

class XMLTag {
public:
   XMLTag(): level_(0), subTags_(0), subCount_(0), subCap_(0) {}
   XMLTag(const std::string &n): name_(n), level_(0), subTags_(0), subCount_(0), subCap_(0) {}
   XMLTag(const std::string &n, const std::string &e): name_(n), element_(e),
   level_(0), subTags_(0), subCount_(0), subCap_(0) {}
   XMLTag(const XMLTag &tag);
   ~XMLTag();
   
   void operator=(const XMLTag &tag);
   
   inline XMLTag *begin() {return subTags_;}
   inline XMLTag *end()   {return subTags_ + subCount_;}
   
   inline const XMLTag *begin() const {return subTags_;}
   inline const XMLTag *end()   const {return subTags_ + subCount_;}
   
   inline std::string Name()    const {return name_;}
   inline std::string Element() const {return element_;}
   
   std::string GetAttribute(const std::string name) const;
   std::string GetTagElement(const std::string name) const;
   XMLTag *GetTag(const std::string name);
   std::string GetValue(const std::string name) const;
   const XMLTag *GetTag(const std::string name) const;
   
   inline bool CheckAttribute(const std::string name) const
      {return attributes_.find(name) != attributes_.end();}
   inline bool CheckTag(const std::string name) const
      {return subTagNames_.find(name) != subTagNames_.end();}
   inline bool Check(const std::string name) const
      {return CheckAttribute(name) || CheckTag(name);}
   
   inline void AddSubTag(const XMLTag &tag);
   inline void SetElement(const std::string &e) {element_ = e;}
   inline void SetLevel(int l) {level_ = l;}
   inline void AddAttribute(const std::string &n, const std::string &v) {attributes_[n] = v;}
   
   friend std::ostream &operator<<(std::ostream &os, const XMLTag &tag) {return tag.Write(os);}
   friend std::istream &operator>>(std::istream &is, XMLTag &tag) {return tag.Read(is);}
   
   static XMLTag* ReadXML(std::istream &is);
private:
   void Resize(int newSize);
   std::ostream &Write(std::ostream &os) const;
   std::istream &Read(std::istream &is);
   
   enum {kIncSize = 10, kBuffSize = 256};
   
   std::string name_;
   std::string element_;
   
   int level_;
   
   int subCount_;
   int subCap_;
   XMLTag *subTags_;
   
   std::map<std::string, int> subTagNames_;
   std::map<std::string, std::string> attributes_;
   
   static void ParseLine(std::list<XMLTag *> *stack, const std::string &line);
   static void ParseTag(std::list<XMLTag *> *stack, const std::string &str1, const std::string &str2);
   static XMLTag TagFromStr(const std::string &str);
};

#endif /* defined(__TackyEngine__XMLTag__) */
