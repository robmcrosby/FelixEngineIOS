//
//  XMLTag.cpp
//  TackyEngine
//
//  Created by Robert Crosby on 1/6/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "XMLTag.h"
#include <cstring>

using namespace std;

XMLTag::XMLTag(const XMLTag &tag): name_(tag.name_), element_(tag.element_),
level_(tag.level_), subTags_(tag.subTags_), subCount_(tag.subCount_), subCap_(tag.subCap_),
subTagNames_(tag.subTagNames_), attributes_(tag.attributes_) {
   if (subCap_) {
      subTags_ = new XMLTag[subCap_];
      for (int i = 0; i < subCount_; ++i)
         subTags_[i] = tag.subTags_[i];
   }
}

XMLTag::~XMLTag() {
   delete [] subTags_;
}

void XMLTag::operator=(const XMLTag &tag) {
   name_ = tag.name_;
   element_ = tag.element_;
   level_ = tag.level_;
   subTags_ = tag.subTags_;
   subCount_ = tag.subCount_;
   subCap_ = tag.subCap_;
   subTagNames_ = tag.subTagNames_;
   attributes_ = tag.attributes_;
   
   if (subCap_) {
      subTags_ = new XMLTag[subCap_];
      for (int i = 0; i < subCount_; ++i)
         subTags_[i] = tag.subTags_[i];
   }
}

std::ostream &XMLTag::Write(std::ostream &os) const {
   map<string, string>::const_iterator attItr;
   
   os << string(level_*3, ' ') << "<" << name_;
   for (attItr = attributes_.begin(); attItr != attributes_.end(); ++attItr)
      os << " " << attItr->first << "=\"" << attItr->second << "\"";
   
   if (subTags_) {
      os << ">" << endl;
      
      for (int i = 0; i < subCount_; ++i)
         os << subTags_[i];
      
      os << string(level_*3, ' ') << "</" << name_ << ">" << endl;
   }
   else if (element_ == "")
      os << "/>" << endl;
   else
      os << ">" << element_ << "</" << name_ << ">" << endl;
   
   return os;
}

std::istream &XMLTag::Read(std::istream &is) {
   list<XMLTag *> stack;
   string line;
   
   while (!is.eof() && !stack.size()) {
      string::size_type start, end;
      // update for other situations
      getline(is, line);
      start = line.find("<");
      end = line.find(">");
      if (start != string::npos && end != string::npos) {
         *this = TagFromStr(line.substr(start+1, end-start-1));
         stack.push_back(this);
      }
   }
   
   while (!is.eof()) {
      getline(is, line);
      ParseLine(&stack, line);
   }
   
   return is;
}

string XMLTag::GetAttribute(const std::string name) const {
   map<string, string>::const_iterator itr;
   
   itr = attributes_.find(name);
   return itr != attributes_.end() ? itr->second : "";
}

string XMLTag::GetTagElement(const std::string name) const {
   map<string, int>::const_iterator itr;
   
   itr = subTagNames_.find(name);
   if (itr != subTagNames_.end()) {
      XMLTag *tag = subTags_ + itr->second;
      return tag->Element();
   }
   return "";
}

string XMLTag::GetValue(const string name) const {
   if (CheckAttribute(name))
      return GetAttribute(name);
   if (CheckTag(name))
      return GetTagElement(name);
   return "";
}

XMLTag* XMLTag::GetTag(const std::string name) {
   map<string, int>::const_iterator itr;
   
   itr = subTagNames_.find(name);
   return itr != subTagNames_.end() ? subTags_ + itr->second : NULL;
}

const XMLTag* XMLTag::GetTag(const std::string name) const {
   map<string, int>::const_iterator itr;
   
   itr = subTagNames_.find(name);
   return itr != subTagNames_.end() ? subTags_ + itr->second : NULL;
}

void XMLTag::AddSubTag(const XMLTag &tag) {
   if (subCount_ >= subCap_ - 1)
      Resize(subCap_ + kIncSize);
   
   if (subTagNames_.find(tag.Name()) == subTagNames_.end())
      subTagNames_[tag.Name()] = subCount_;
   subTags_[subCount_] = tag;
   subTags_[subCount_++].SetLevel(level_+1);
}

void XMLTag::Resize(int newSize) {
   XMLTag *oldTags = subTags_;
   
   subTags_ = new XMLTag [newSize];
   for (int i = 0; i < subCount_; ++i)
      subTags_[i] = oldTags[i];
   
   delete [] oldTags;
   subCap_ = newSize;
}

XMLTag* XMLTag::ReadXML(istream &is) {
   list<XMLTag *> stack;
   string line;
   
   while (!is.eof()) {
      getline(is, line);
      ParseLine(&stack, line);
   }
   
   return stack.size() ? stack.front() : NULL;
}

void XMLTag::ParseLine(list<XMLTag *> *stack, const string &line) {
   string::size_type loc1, loc2;
   
   loc1 = line.find("<");
   loc2 = line.find(">");
   
   if (loc1 != string::npos && loc2 != string::npos) {
      ++loc1;
      ParseTag(stack, line.substr(loc1, loc2 - loc1), line.substr(loc2+1));
   }
   else {
      loc1 = line.find_first_not_of("\t ");
      if (loc1 != string::npos && stack->size())
         stack->back()->SetElement(line.substr(loc1));
   }
}

void XMLTag::ParseTag(list<XMLTag *> *stack, const string &str1, const string &str2) {
   string::size_type loc;
   
   loc = str1.find("/");
   
   if (loc == string::npos) {
      if (!stack->size())
         stack->push_back(new XMLTag(TagFromStr(str1)));
      else {
         stack->back()->AddSubTag(TagFromStr(str1));
         stack->push_back(stack->back()->end() - 1);
      }
      
      if (str2 != "") {
         loc = str2.find("</");
         if (loc != string::npos) {
            stack->back()->SetElement(str2.substr(0, loc));
            if (stack->size() > 1)
               stack->pop_back();
         }
         else
            stack->back()->SetElement(str2);
      }
   }
   else if (loc == 0) {
      if (stack->size() > 1)
         stack->pop_back();
   }
   else {
      stack->back()->AddSubTag(TagFromStr(str1));
   }
}

XMLTag XMLTag::TagFromStr(const string &str) {
   string::size_type loc;
   string subStr = str;
   
   loc = str.find_first_of(" /");
   XMLTag tag(str.substr(0, loc));
   
   loc = str.find_first_not_of(" ", loc);
   while (loc != string::npos && subStr[loc] != '/') {
      string name;
      subStr = subStr.substr(loc);
      
      loc = subStr.find("=\"");
      if (loc == string::npos)
         break;
      name = subStr.substr(0, loc);
      
      subStr = subStr.substr(loc+2);
      loc = subStr.find("\"");
      if (loc == string::npos)
         break;
      tag.AddAttribute(name, subStr.substr(0, loc));
      
      loc = subStr.find_first_not_of(" ", loc+1);
   }
   
   return tag;
}