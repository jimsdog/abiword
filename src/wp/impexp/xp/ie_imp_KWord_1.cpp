/* AbiWord
 * Copyright (C) 2001 AbiSource, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA��
 * 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ie_imp_KWord_1.h"
#include "ie_types.h"
#include "pd_Document.h"
#include "ut_growbuf.h"

/*
 * This file is meant to import KWord 1.x documents.
 * Kword is a component of KOffice for KDE.
 */

/*****************************************************************/
/*****************************************************************/

#ifdef ENABLE_PLUGINS

// completely generic code to allow this to be a plugin

#include "xap_Module.h"

// we use a reference-counted sniffer
static IE_Imp_KWord_1_Sniffer * m_sniffer = 0;

ABI_FAR extern "C"
int abi_plugin_register (XAP_ModuleInfo *mi)
{
  if (!m_sniffer)
  {
    m_sniffer = new IE_Imp_KWord_1_Sniffer ();
  }
  else
  {
    m_sniffer->ref();
  }

  mi->name = "KWord 1.x Importer";
  mi->desc = "Import KWord 1.x Documents";
  mi->version = ABI_VERSION_STRING;
  mi->author = "Abi the Ant"
  mi->usage = "No Usage";

  IE_Imp::registerImporter(m_sniffer);
  return 1;
}

ABI_FAR extern "C"
int abi_plugin_unregister(XAP_ModuleInfo *mi)
{
  mi->name = 0;
  mi->desc = 0;
  mi->version = 0;
  mi->author = 0;
  mi->usage = 0;

  UT_ASSERT(m_sniffer);

  IE_Imp::unregisterImporter(m_sniffer);
  if (!m_sniffer->unref())
  {
    m_sniffer = 0;
  }

  return 1;
}

ABI_FAR extern "C"
int abi_plugin_supports_version(UT_uint32 major, UT_uint32 minor, UT_uint32 release)
{
  return isCurrentAbiVersion(major, minor, release) ? 1 : 0;
}

#endif

/***********************************************************/
/***********************************************************/

bool IE_Imp_KWord_1_Sniffer::recognizeContents(const char *szBuf, UT_uint32 iNumbytes)
{

  // first, look for the <?xml at the beginning of the document
  if(strncmp(szBuf, "<?xml", 4) != 0)
    return false;

  // now look for the KWord beginning tag <DOC
  if(strstr(szBuf, "<DOC") == NULL)
    return false;

  return true;
}

bool IE_Imp_KWord_1_Sniffer::recognizeSuffix(const char *szSuffix)
{
  return (UT_stricmp(szSuffix,".kwd") == 0);
}

UT_Error IE_Imp_KWord_1_Sniffer::constructImporter(PD_Document *pDocument, IE_Imp **ppie)
{
  IE_Imp_KWord_1 *p = new IE_Imp_KWord_1(pDocument);
  *ppie = p;
  return UT_OK;
}

bool IE_Imp_KWord_1_Sniffer::getDlgLabels(const char **pszDesc, const char **pszSuffixList,
                                          IEFileType *ft)
{
  *pszDesc = "KWord 1.x (.kwd)";
  *pszSuffixList = "*.kwd";
  *ft = getFileType();
  return true;
}

/*****************************************************************/
/*****************************************************************/

IE_Imp_KWord_1::~IE_Imp_KWord_1()
{
}

IE_Imp_KWord_1::IE_Imp_KWord_1(PD_Document *pDocument) : IE_Imp_XML(pDocument, true)
{
  m_bInText = false;
}

void IE_Imp_KWord_1::_charData(const XML_Char *buf, int len)
{
  for (int i = 0; i < len; i++)
  m_szTextBuffer += buf[i];
}

/*****************************************************************/
/*****************************************************************/

#define TT_OTHER           0                    // anything else
#define TT_ATTRIBUTE       1                    // attributes of document
#define TT_BOTTOMBORDER    2                    // bottom border
#define TT_CHARSET         3                    // chatset
#define TT_CLIPARTS        4                    // cliparts
#define TT_COLOR           5                    // color stuff
#define TT_COUNTER         6                    // ??
#define TT_DOC             7                    // a document <kwd>
#define TT_FLOW            8                    // allignment
#define TT_FOLLOWING       9                    // ??
#define TT_FONT            10                   // a font
#define TT_FORMAT          11                   // ??
#define TT_FORMATS         12                   // ??
#define TT_FRAME           13                   // a frame
#define TT_FRAMESET        14                   // a frameset
#define TT_INDENTS         15                   // indent
#define TT_ITALIC          16                   // italic font
#define TT_LAYOUT          17                   // layout
#define TT_LEFTBORDER      18                   // left border
#define TT_LINESPACING     19                   // line spacing
#define TT_NAME            20                   // ??
#define TT_OFFSETS         21                   // offsets
#define TT_PAPER           22                   // ??
#define TT_PAPERBORDERS    23                   // ??
#define TT_PAGEBREAKING    24                   // page breaking
#define TT_PARAGRAPH       25                   // paragraphs
#define TT_RIGHTBORDER     26                   // right border
#define TT_SIZE            27                   // size of a font
#define TT_STRIKEOUT       28                   // strikeout font
#define TT_STYLE           29                   // style
#define TT_STYLES          30                   // styles
#define TT_TEXT            31                   // written text
#define TT_TOPBORDER       32                   // top border
#define TT_UNDERLINE       33                   // underline font
#define TT_VERTALIGN       34                   // vertical alignment
#define TT_WEIGHT          35                   // font weight

// KEEP IN ALPHABETICAL ORDER!!

static struct xmlToIdMapping s_Tokens[] =
{
  { "ATTRIBUTE",     TT_ATTRIBUTE    },
  { "BOTTOMBORDER",  TT_BOTTOMBORDER },
  { "CHARSET",       TT_CHARSET      },
  { "CLIPARTS",      TT_CLIPARTS     },
  { "COLOR",      TT_COLOR     },
  { "COUNTER",       TT_COUNTER      },
  { "DOC",           TT_DOC          },
  { "FLOW",          TT_FLOW         },
  { "FOLLOWING",     TT_FOLLOWING    },
  { "FONT",          TT_FONT         },
  { "FORMAT",        TT_FORMAT       },
  { "FORMATS",       TT_FORMATS      },
  { "FRAME",         TT_FRAME        },
  { "FRAMESET",      TT_FRAMESET     },
  { "INDENTS",       TT_INDENTS      },
  { "ITALIC",        TT_ITALIC       },
  { "LAYOUT",        TT_LAYOUT       },
  { "LEFTBORDER",    TT_LEFTBORDER   },
  { "LINESPACING",   TT_LINESPACING  },
  { "NAME",          TT_NAME         },
  { "OFFSETS",       TT_OFFSETS      },
  { "PAPER",         TT_PAPER        },
  { "PAPERBORDERS",  TT_PAPERBORDERS },
  { "PAGEBREAKING",  TT_PAGEBREAKING },
  { "PARAGRAPH",     TT_PARAGRAPH    },
  { "RIGHTBORDER",   TT_RIGHTBORDER  },
  { "SIZE",          TT_SIZE         },
  { "STRIKEOUT",     TT_STRIKEOUT    },
  { "STYLE",         TT_STYLE        },
  { "STYLES",        TT_STYLES       },
  { "TEXT",          TT_TEXT         },
  { "TOPBORDER",     TT_TOPBORDER    },
  { "UNDERLINE",     TT_UNDERLINE    },
  { "VERTALIGN",     TT_VERTALIGN    },
  { "WEIGHT",        TT_WEIGHT       }
};

#define TokenTableSize ((sizeof(s_Tokens)/sizeof(s_Tokens[0])))

/*****************************************************************/
/*****************************************************************/

#define X_TestParseState(ps) ((m_parseState==(ps)))

#define X_VerifyParseState(ps) do { if (!(X_TestParseState(ps))) { m_error = UT_IE_BOGUSDOCUMENT; return; } } while(0)

#define X_CheckDocument(b) do {  if (!(b)) { m_error = UT_IE_BOGUSDOCUMENT; return; } } while (0)

#define X_CheckError(v) do {  if (!(v)) {  m_error = UT_ERROR; return; } } while (0)

#define X_EatIfAlreadyError() do {  if (m_error) return; } while (0)

/*****************************************************************/
/*****************************************************************/


void IE_Imp_KWord_1::_startElement(const XML_Char *name, const XML_Char **atts)
{
  UT_DEBUGMSG(("KWord import: startElement: %s\n", name));

  // xml parser keeps running until buffer consumed
  X_EatIfAlreadyError();

  UT_uint32 tokenIndex = _mapNameToToken(name, s_Tokens, TokenTableSize);

  XML_Char *pVal = 0;

  switch (tokenIndex)
  {
    case TT_ATTRIBUTE:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin ATTRIBUTE\n"));
        break;
      }

    case TT_BOTTOMBORDER:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin BOTTOMBORDER\n"));
        break;
      }

    case TT_CHARSET:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin CHARSET\n"));
        break;
      }

    case TT_CLIPARTS:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin CLIPARTS\n"));
        break;
      }

    case TT_COLOR:
      {

        const XML_Char *p = NULL;
	int red, green, blue;
	red = green = blue = 0;
	p = _getXMLPropValue("red", atts);
	if (p != NULL) {
	  red = atoi(p);
	}
	if (p != NULL) {
	  p = _getXMLPropValue("green", atts);
	  green = atoi(p);
	}
	if (p != NULL) {
	  p = _getXMLPropValue("blue", atts);
	  blue = atoi(p);
	}
	
	char buf[7];
	sprintf(buf, "%02x%02x%02x", red, green, blue);
	m_szProps += "color:";
	m_szProps += buf;
	m_szProps += "; ";
	break;

      }

    case TT_COUNTER:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin COUNTER\n"));
        break;
      }

    case TT_DOC:
      {
        //  X_VerifyParseState(_PS_Init);
        m_parseState = _PS_Doc;
        X_CheckError(getDoc()->appendStrux(PTX_Section,(const XML_Char**)NULL));
        X_CheckError(getDoc()->appendStrux(PTX_Block, NULL));
        return;
      }
      
    case TT_FOLLOWING:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin FOLLOWING\n"));
        break;
      }
      
    case TT_FRAME:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin FRAME\n"));
        break;
      }
      
    case TT_FRAMESET:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin FRAMESET\n"));
        break;
      }
      
    case TT_INDENTS:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin INDENTS\n"));
        break;
      }
      
    case TT_LAYOUT:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin LAYOUT\n"));
        break;
      }
      
    case TT_LEFTBORDER:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin LEFTBORDER\n"));
        break;
      }
      
    case TT_LINESPACING:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin LINESPACING\n"));
        break;
      }
    
    case TT_OFFSETS:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin OFFSETS\n"));
        break;
      }
      
    case TT_PAPER:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin PAPER\n"));
        break;
      }
      
    case TT_PAPERBORDERS:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin PAPERBORDERS\n"));
        break;
      }
      
    case TT_PAGEBREAKING:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin PAGEBREAKING\n"));
        break;
      }
      
    case TT_PARAGRAPH:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin PARAGRPAHS\n"));
        break;
      }
      
    case TT_RIGHTBORDER:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin RIGHTBORDER\n"));
        break;
      }
      
    case TT_STYLE:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin STYLE\n"));
        break;
      }
      
    case TT_STYLES:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin STYLES\n"));
        break;
      }
      
    case TT_TEXT:
      {
        m_bInText = true;
        return;
      }
      
    case TT_TOPBORDER:
      {
        UT_DEBUGMSG(("ABIDEBUG: begin TOPBORDER\n"));
        break;
      }
      
    case TT_ITALIC:
      {
        pVal = (XML_Char *)_getXMLPropValue("value", atts);
        if (pVal && UT_strcmp(pVal, "1") == 0 )
        m_szProps += "font-style:italic; ";
        break;
      }
      
    case TT_UNDERLINE:
      {
        pVal = (XML_Char *)_getXMLPropValue("value", atts);
        if (pVal && UT_strcmp(pVal, "1") == 0 )
        m_szProps += "text-decoration:underline; ";
        break;
      }
      
    case TT_WEIGHT:
      {
        pVal = (XML_Char *)_getXMLPropValue("value", atts);
        if ( pVal && UT_strcmp ( pVal, "75" ) == 0 )
        m_szProps += "font-weight:bold; ";
        break;
      }
      
    case TT_STRIKEOUT:
      {
        pVal = (XML_Char *)_getXMLPropValue("value", atts);
        if (pVal && UT_strcmp(pVal, "1") == 0 )
        m_szProps += "text-decoration:strike-through; ";
        break;
      }
      
    case TT_FONT:
      {
        pVal = (XML_Char *)_getXMLPropValue("name", atts);
        if (pVal)
        {
          m_szProps += "font-face:";
          m_szProps += pVal;
          m_szProps += "; ";
        }
        break;
      }
      
    case TT_SIZE:
      {
        pVal = (XML_Char *)_getXMLPropValue("value", atts);
        if (pVal)
        {
          m_szProps += "font-size:";
          m_szProps += pVal;
          m_szProps += "; ";
        }
        break;
      }
      
    case TT_FLOW:
    case TT_FORMAT:
    case TT_NAME:
    case TT_VERTALIGN:
    default:
      UT_DEBUGMSG(("ABIBUG: work in progress\n"));
      break;
    }
  
}

void IE_Imp_KWord_1::_appendText()
{
  if (m_szTextBuffer.size())
    {
      if (!getDoc()->appendSpan(m_szTextBuffer.ucs_str(), m_szTextBuffer.size()))
      {
        UT_DEBUGMSG(("Error appending text run\n"));
        return;
      }
      m_szTextBuffer.clear();
    }
}

void IE_Imp_KWord_1::_endElement(const XML_Char *name)
{

  UT_DEBUGMSG(("KWord import: endElement %s\n", name));

  X_EatIfAlreadyError();

  UT_uint32 tokenIndex = _mapNameToToken (name, s_Tokens, TokenTableSize);

  switch (tokenIndex)
  {
    case TT_ATTRIBUTE:
      UT_DEBUGMSG(("ABIDEBUG: end ATTRIBUTE\n"));
      break;

    case TT_BOTTOMBORDER:
      UT_DEBUGMSG(("ABIDEBUG: end BOTTOMBORDER\n"));
      break;

    case TT_CHARSET:
      UT_DEBUGMSG(("ABIDEBUG: end CHARSET\n"));
      break;

    case TT_CLIPARTS:
      UT_DEBUGMSG(("ABIDEBUG: end CLIPARTS\n"));
      break;

    case TT_COLOR:
      UT_DEBUGMSG(("ABIDEBUG: end COLOR\n"));
      break;

    case TT_COUNTER:
      UT_DEBUGMSG(("ABIDEBUG: end COUNTER\n"));
      break;

    case TT_DOC:
    {
      //  X_VerifyParseState(_PS_Init);
      m_parseState = _PS_Doc;
      X_CheckError(getDoc()->appendStrux(PTX_Section,(const XML_Char**)NULL));
      X_CheckError(getDoc()->appendStrux(PTX_Block, NULL));
      return;
    }

    case TT_FLOW:
      UT_DEBUGMSG(("ABIDEBUG: end FLOW\n"));
      break;

    case TT_FOLLOWING:
      UT_DEBUGMSG(("ABIDEBUG: end FOLLOWING\n"));
      break;

    case TT_FONT:
      UT_DEBUGMSG(("ABIDEBUG: end FONT\n"));
      break;

    case TT_FORMAT:
    {
      XML_Char *propsArray[3]; // I don't think that this is necessary!

      if (m_szProps.size() == 0)
        {
          UT_DEBUGMSG(("ABIDEBUG: no properties\n"));
          _appendText ();
          break;
        }

      m_szProps[m_szProps.size() - 2] = 0; // nock off the final ';'

      propsArray[0] = (XML_Char *)"props";
      propsArray[1] = (XML_Char *)m_szProps.c_str();
      propsArray[2] = 0;

      UT_DEBUGMSG(("ABIDEBUG: formatting properties are: %s\n",propsArray[1]));
      if (!getDoc()->appendFmt((const XML_Char **)propsArray))
        {
          UT_DEBUGMSG(("Error appending character formatting\n"));
        }

      m_szProps.clear();
      _appendText();

      break;
    }

    case TT_FORMATS:
      UT_DEBUGMSG(("ABIDEBUG: end FORMATS\n"));
      break;

    case TT_FRAME:
      UT_DEBUGMSG(("ABIDEBUG: end FRAME\n"));
      break;

    case TT_FRAMESET:
      UT_DEBUGMSG(("ABIDEBUG: end FRAMESET\n"));
      break;

    case TT_INDENTS:
      UT_DEBUGMSG(("ABIDEBUG: end INDENTS\n"));
      break;

    case TT_ITALIC:
      UT_DEBUGMSG(("ABIDEBUG: end ITALIC\n"));
      break;

    case TT_LAYOUT:
      UT_DEBUGMSG(("ABIDEBUG: end LAYOUT\n"));
      break;

    case TT_LEFTBORDER:
      UT_DEBUGMSG(("ABIDEBUG: end LEFTBORDER\n"));
      break;

    case TT_LINESPACING:
      UT_DEBUGMSG(("ABIDEBUG: end LINESPACING\n"));
      break;

   case TT_NAME:
      UT_DEBUGMSG(("ABIDEBUG: end NAME\n"));
      break;

    case TT_OFFSETS:
      UT_DEBUGMSG(("ABIDEBUG: end OFFSETS\n"));
      break;

    case TT_PAPER:
      UT_DEBUGMSG(("ABIDEBUG: end PAPER\n"));
      break;

    case TT_PAPERBORDERS:
      UT_DEBUGMSG(("ABIDEBUG: end PAPERBORDERS\n"));
      break;

    case TT_PAGEBREAKING:
      UT_DEBUGMSG(("ABIDEBUG: end PAGEBREAKING\n"));
      break;

    case TT_PARAGRAPH:
      UT_DEBUGMSG(("ABIDEBUG: end PARAGRPAH\n"));
      break;

    case TT_RIGHTBORDER:
      UT_DEBUGMSG(("ABIDEBUG: end RIGHTBORDER\n"));
      break;

    case TT_SIZE:
      UT_DEBUGMSG(("ABIDEBUG: end SIZE\n"));
      break;

    case TT_STRIKEOUT:
      UT_DEBUGMSG(("ABIDEBUG: end STRIKEOUT\n"));
      break;

    case TT_STYLE:
      UT_DEBUGMSG(("ABIDEBUG: end STYLE\n"));
      break;

    case TT_STYLES:
      UT_DEBUGMSG(("ABIDEBUG: end STYLES\n"));
      break;

    case TT_TEXT:
      m_bInText = false;
      return;

    case TT_TOPBORDER:
      UT_DEBUGMSG(("ABIDEBUG: end TOPBORDER\n"));
      break;

    case TT_UNDERLINE:
      UT_DEBUGMSG(("ABIDEBUG: end UNDERLINE\n"));
      break;

    case TT_VERTALIGN:
      UT_DEBUGMSG(("ABIDEBUG: end VERTALIGN\n"));
      break;

    case TT_WEIGHT:
      UT_DEBUGMSG(("ABIDEBUG: end WEIGHT\n"));
      break;

  }

}
