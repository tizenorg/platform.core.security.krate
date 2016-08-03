/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#ifndef __XML_DOMPARSER_H__
#define __XML_DOMPARSER_H__

#include <string>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "document.h"

namespace xml {

class Parser {
public:
	static Document* parseFile(const std::string& filename, bool validate = false);
	static Document* parseString(const std::string& xml, bool validate = false);

private:
	static Document* parseContext(xmlParserCtxt* context, bool validate = false);
};

} // namespace xml
#endif //__XML_DOMPARSER_H__
