#!/usr/bin/env python
################################################################################
# ConvertLanguage.py - converts an old 3RVX version 2.X language file to the new
# 3.X format. A 'template' file is used to provide the missing strings that
# didn't exist in the old 2.X translations.
#
# Matthew Malensek <matt@malensek.net>
################################################################################

def convert(input, template, output):
    from xml.etree import ElementTree
    convert = ElementTree.parse(input)
    cr = convert.getroot()
    template = ElementTree.parse(template)
    tr = template.getroot()

    strings = set()
    for strtag in tr.findall('string'):
        strings.add(strtag.find('original').text)

    translations = {}
    for wordtag in cr.findall('word'):
        str = wordtag.find('key').text
        if str in strings:
            trans = wordtag.find('translation').text
            translations[str] = trans

    for strtag in tr.findall('string'):
        orig = strtag.find('original')
        trans = strtag.find('translation')
        if orig == None or trans == None:
            continue

        if orig.text in translations:
            trans = strtag.find('translation')
            print(trans.text + ' -> ' + translations[orig.text])
            trans.text = translations[orig.text]
        else:
            trans.text = orig.text
    template.write(output, encoding='UTF-8')

def print_usage(appname='ConvertLanguage'):
    print('Usage: ' + appname + ' input.xml template.xml output.xml')
    print('File descriptions:')
    print('input.xml       The old (v2) language file to convert')
    print('template.xml    Template file to use for missing translations')
    print('output.xml      Output file name')

if __name__ == "__main__":
    import os
    import sys
    args = sys.argv
    if len(args) < 3:
        print_usage(args[0])
    else:
        convert(args[1], args[2], args[3])
