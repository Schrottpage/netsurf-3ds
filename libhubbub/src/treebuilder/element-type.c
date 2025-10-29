/*
 * This file is part of Hubbub.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2021 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "treebuilder/element-type.h"

static const struct element_type_map wordlist[] = {
        {"a", A},
        {"address", ADDRESS},
        {"annotation-xml", ANNOTATION_XML},
        {"applet", APPLET},
        {"area", AREA},
        {"article", ARTICLE},
        {"aside", ASIDE},
        {"b", B},
        {"base", BASE},
        {"basefont", BASEFONT},
        {"bgsound", BGSOUND},
        {"big", BIG},
        {"blockquote", BLOCKQUOTE},
        {"body", BODY},
        {"br", BR},
        {"button", BUTTON},
        {"caption", CAPTION},
        {"center", CENTER},
        {"col", COL},
        {"colgroup", COLGROUP},
        {"command", COMMAND},
        {"dd", DD},
        {"desc", DESC},
        {"details", DETAILS},
        {"dialog", DIALOG},
        {"dir", DIR},
        {"div", DIV},
        {"dl", DL},
        {"dt", DT},
        {"em", EM},
        {"embed", EMBED},
        {"fieldset", FIELDSET},
        {"figcaption", FIGCAPTION},
        {"figure", FIGURE},
        {"font", FONT},
        {"footer", FOOTER},
        {"foreignobject", FOREIGNOBJECT},
        {"form", FORM},
        {"frame", FRAME},
        {"frameset", FRAMESET},
        {"h1", H1},
        {"h2", H2},
        {"h3", H3},
        {"h4", H4},
        {"h5", H5},
        {"h6", H6},
        {"head", HEAD},
        {"hr", HR},
        {"html", HTML},
        {"i", I},
        {"iframe", IFRAME},
        {"image", IMAGE},
        {"img", IMG},
        {"input", INPUT},
        {"isindex", ISINDEX},
        {"li", LI},
        {"link", LINK},
        {"listing", LISTING},
        {"malignmark", MALIGNMARK},
        {"marquee", MARQUEE},
        {"math", MATH},
        {"menu", MENU},
        {"meta", META},
        {"mglyph", MGLYPH},
        {"mi", MI},
        {"mn", MN},
        {"mo", MO},
        {"ms", MS},
        {"mtext", MTEXT},
        {"nobr", NOBR},
        {"noembed", NOEMBED},
        {"noframes", NOFRAMES},
        {"noscript", NOSCRIPT},
        {"object", OBJECT},
        {"ol", OL},
        {"optgroup", OPTGROUP},
        {"option", OPTION},
        {"output", OUTPUT},
        {"p", P},
        {"param", PARAM},
        {"plaintext", PLAINTEXT},
        {"pre", PRE},
        {"s", S},
        {"script", SCRIPT},
        {"select", SELECT},
        {"small", SMALL},
        {"spacer", SPACER},
        {"strike", STRIKE},
        {"strong", STRONG},
        {"style", STYLE},
        {"summary", SUMMARY},
        {"svg", SVG},
        {"table", TABLE},
        {"tbody", TBODY},
        {"td", TD},
        {"textarea", TEXTAREA},
        {"tfoot", TFOOT},
        {"th", TH},
        {"thead", THEAD},
        {"title", TITLE},
        {"tr", TR},
        {"tt", TT},
        {"u", U},
        {"ul", UL},
        {"wbr", WBR},
        {"xmp", XMP},
};

static bool element_name_equals(const char *name, size_t len,
                const char *candidate)
{
        size_t i;

        if (strlen(candidate) != len) {
                return false;
        }

        for (i = 0; i < len; i++) {
                unsigned char a = (unsigned char)name[i];
                unsigned char b = (unsigned char)candidate[i];

                if (tolower(a) != tolower(b)) {
                        return false;
                }
        }

        return true;
}

/* Exported function, documented in element-type.h */
element_type element_type_from_name(
                hubbub_treebuilder *treebuilder,
                const hubbub_string *tag_name)
{
        size_t i;

        UNUSED(treebuilder);

        for (i = 0; i < sizeof(wordlist) / sizeof(wordlist[0]); i++) {
                if (element_name_equals((const char *)tag_name->ptr,
                                tag_name->len, wordlist[i].name)) {
                        return wordlist[i].type;
                }
        }

        return UNKNOWN;
}

/**
 * Convert an element type to a name
 *
 * \param type  The element type
 * \return Pointer to name
 */
const char *element_type_to_name(element_type type)
{
	size_t i;

	for (i = 0; i < sizeof(wordlist) / sizeof(wordlist[0]); i++) {
		if (wordlist[i].type == type) {
			return wordlist[i].name;
		}
	}

	return "UNKNOWN";
}
