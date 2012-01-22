/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2008 Matteo Bertozzi
 * Copyright (c) 2010-2011 Pier Luigi Fiorini
 *
 * Author(s):
 *    Matteo Bertozzi <theo.bertozzi@gmail.com>
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Vibe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Vibe.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "vmimetype.h"

extern "C" {
#include <sys/stat.h>
}

static int hex2int(int c)
{
    if (!isascii((unsigned char) c))
        return -1;
    if (isdigit((unsigned char) c))
        return c - '0';
    if ((c >= 'a') && (c <= 'f'))
        return c + 10 - 'a';
    if ((c >= 'A') && (c <= 'F'))
        return c + 10 - 'A';
    return -1;
}

static void extract_offset(const QString &offset, int *startOffset, int *endOffset)
{
    int index = offset.indexOf(':');
    if (index < 0) {
        *startOffset = offset.toInt();
        *endOffset = *startOffset;
    } else {
        *startOffset = offset.mid(0, index).toInt();
        *endOffset = *startOffset + offset.mid(index + 1).toInt();
    }
}

static QByteArray convert_string(const char *source)
{
    QByteArray value;
    char c, val;
    while ((c = *source++) != '\0') {
        if (c == '\\') {
            switch (c = *source++) {
                case '\0':
                    return value;
                case 'n':
                    value += '\n';
                    break;
                case 'r':
                    value += '\r';
                    break;
                case 'b':
                    value += '\b';
                    break;
                case 't':
                    value += '\t';
                    break;
                case 'f':
                    value += '\f';
                    break;
                case 'v':
                    value += '\v';
                    break;
                    /* \ and up to 3 octal digits */
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                    val = c - '0';
                    c = *source++;  /* try for 2 */
                    if (c >= '0' && c <= '7') {
                        val = (val << 3) | (c - '0');
                        c = *source++;  /* try for 3 */
                        if (c >= '0' && c <= '7')
                            val = (val << 3) | (c - '0');
                        else
                            --source;
                    } else
                        --source;
                    value += val;
                    break;
                    /* \x and up to 2 hex digits */
                case 'x':
                    val = 'x';	/* Default if no digits */
                    c = hex2int(*source++);	/* Get next char */
                    if (c >= 0) {
                        val = c;
                        c = hex2int(*source++);
                        if (c >= 0)
                            val = (val << 4) + c;
                        else
                            --source;
                    } else
                        --source;
                    value += val;
                    break;
                default:
                    value += c;
                    break;
            }
        } else {
            value += c;
        }
    }
    return value;
}

static unsigned long convert_number(const QByteArray &text)
{
    if (text.startsWith("0x")) {
        bool ok;
        unsigned long num = text.toULong(&ok, 16);
        return ok ? num : -1;
    }
    if (text.startsWith("0"))
        return text.toULong(NULL, 8);
    return text.toULong();
}

/* Parse the value and mask attributes of a <match> element with a
 * numerical type (anything except "string").
 */
static void parse_int_value(int bytes, const char *in, const char *in_mask,
                            QByteArray *parsed_value, QByteArray *parsed_mask,
                            bool big_endian)
{
    unsigned long value = convert_number(in);
    for (int b = 0; b < bytes; b++) {
        int shift = (big_endian ? (bytes - b - 1) : b) * 8;
        parsed_value->append((value >> shift) & 0xff);
    }
    if ((bytes == 1 && (value & ~0xff)) ||
            (bytes == 2 && (value & ~0xffff))) {
        qWarning("Number out-of-range");
        return;
    }
    if (in_mask) {
        unsigned long mask = convert_number(in_mask);
        parsed_mask->fill(0, bytes);
        for (int b = 0; b < bytes; b++) {
            int shift = (big_endian ? (bytes - b - 1) : b) * 8;
            parsed_mask->data()[b] = (mask >> shift) & 0xff;
        }
    }
    if (parsed_mask->length() > 0 && parsed_mask->at(0) == 0)
        parsed_mask->clear();
}

/* 'len' is the length of the value. The mask created will be the same
 * length.
 */
static QByteArray parse_string_mask(const char *mask, int len)
{
    if (mask == NULL || len < 1)
        return(QByteArray());
    if (mask[0] != '0' || mask[1] != 'x') {
        qWarning("parse_string_mask(): String masks must be in base 16 (starting with 0x)");
        return QByteArray();
    }
    mask += 2;
    QByteArray parsed_mask(len, 0);
    for (int i = 0; mask[i]; ++i) {
        int c = hex2int(mask[i]);
        if (c == -1) {
            qWarning("parse_string_mask(): is not a valid hex digit");
            return QByteArray();
        }
        if (i >= len * 2) {
            qWarning("parse_string_mask(): Mask is longer than value");
            return QByteArray();
        }
        parsed_mask[i >> 1] = parsed_mask[i >> 1] | ((i & 1) ? c : (c << 4));
    }
    if (parsed_mask.length() > 0 && (char) parsed_mask[0] == 0)
        parsed_mask.clear();
    return parsed_mask;
}

class VMimeTypePrivate
{
public:
    QString mimeType;
    QDomDocument xmlDocument;
    QDomElement mimeNode;
};

VMimeType::VMimeType(const QString &mimeType, QObject *parent) :
    QObject(parent),
    d_ptr(new VMimeTypePrivate())
{
    Q_D(VMimeType);

    // Save MIME type if any
    d->mimeType = mimeType;

    // Open freedesktop.org database
    QFile xml(":/etc/freedesktop.org.xml");
    if (xml.open(QIODevice::ReadOnly)) {
        d->xmlDocument.setContent(&xml);
        xml.close();
    }
}

bool VMimeType::fromFileName(const QString &fileName)
{
    Q_D(VMimeType);

    QFileInfo info(fileName);

    // Is it a directory?
    if (info.isDir()) {
        if (access(fileName.toAscii().data(), R_OK) < 0)
            d->mimeType = "inode/directory-locked";
        else
            d->mimeType = "inode/directory";
        return getMimeNode();
    }

    // Is it a symlink?
    if (info.isSymLink()) {
        d->mimeType = "inode/symlink";
        return getMimeNode();
    }

    // Check whether it's a special file
    struct stat st;
    if (stat(fileName.toAscii().data(), &st) != -1) {
        if (S_ISCHR(st.st_mode)) {
            d->mimeType = "inode/chardevice";
            return getMimeNode();
        }
        if (S_ISBLK(st.st_mode)) {
            d->mimeType = "inode/blockdevice";
            return getMimeNode();
        }
        if (S_ISFIFO(st.st_mode)) {
            d->mimeType = "inode/fifo";
            return getMimeNode();
        }
        if (S_ISSOCK(st.st_mode)) {
            d->mimeType = "inode/socket";
            return getMimeNode();
        }
    }

    // Search by glob
    QDomElement root = d->xmlDocument.documentElement();
    QDomNodeList globList = root.elementsByTagName("glob");
    for (uint i = 0; i < globList.length(); ++i) {
        QDomElement globNode = globList.item(i).toElement();
        QRegExp regExp(globNode.attribute("pattern"));
        regExp.setPatternSyntax(QRegExp::Wildcard);
        if (regExp.exactMatch(info.fileName())) {
            d->mimeNode = globNode.parentNode().toElement();
            d->mimeType = d->mimeNode.attribute("type");
            return true;
        }
    }
    return false;
}

bool VMimeType::fromDevice(QIODevice *dev)
{
    Q_D(VMimeType);

    // Search by magic pattern
    QDomElement root = d->xmlDocument.documentElement();
    QDomNodeList magicList = root.elementsByTagName("magic");
    QList<QDomElement> matchMagicList;

    for (uint i = 0; i < magicList.length(); ++i) {
        QDomElement magicNode = magicList.item(i).toElement();
        if (checkMagic(magicNode, dev))
            matchMagicList.append(magicNode);
    }

    if (matchMagicList.size() > 0) {
        QDomElement magic = matchMagicList[0];
        for (int i = 1; i < matchMagicList.size(); ++i) {
            if (!matchMagicList[i].hasAttribute("priority")) {
                magic = matchMagicList[i];
                break;
            } else if (magic.hasAttribute("priority") && magic.attribute("priority") < matchMagicList[i].attribute("priority"))
                magic = matchMagicList[i];
            d->mimeNode = magic.parentNode().toElement();
            d->mimeType = d->mimeNode.attribute("type");
            return true;
        }
    }

    return false;
}

QString VMimeType::iconName() const
{
}

QString VMimeType::iconNameForUrl(const QUrl &url)
{
}

QString VMimeType::mimeType() const
{
    Q_D(const VMimeType);
    return d->mimeType;
}

QString VMimeType::subClassOf() const
{
    Q_D(const VMimeType);
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("sub-class-of");
    return (nodeList.length() > 0) ? nodeList.item(0).toElement().attribute("type") : QString();
}

QString VMimeType::comment() const
{
    Q_D(const VMimeType);
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("comment");
    return (nodeList.length() > 0) ? nodeList.item(0).toElement().text() : QString();
}

QString VMimeType::genericIconName() const
{
    Q_D(const VMimeType);
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("generic-icon");
    return (nodeList.length() > 0) ? nodeList.item(0).toElement().attribute("name") : QString();
}

QString VMimeType::acronym() const
{
    Q_D(const VMimeType);
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("acronym");
    return (nodeList.length() > 0) ? nodeList.item(0).toElement().text() : QString();
}

QString VMimeType::expandedAcronym() const
{
    Q_D(const VMimeType);
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("expanded-acronym");
    return (nodeList.length() > 0) ? nodeList.item(0).toElement().text() : QString();
}

QStringList VMimeType::aliases() const
{
    Q_D(const VMimeType);
    QStringList list;
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("alias");
    for (uint i = 0; i < nodeList.length(); i++)
        list.append(nodeList.item(i).toElement().attribute("type"));
    return list;
}

QStringList VMimeType::globs() const
{
    Q_D(const VMimeType);
    QStringList list;
    QDomNodeList nodeList = d->mimeNode.elementsByTagName("glob");
    for (uint i = 0; i < nodeList.length(); i++)
        list.append(nodeList.item(i).toElement().attribute("pattern"));
    return list;
}

bool VMimeType::getMimeNode()
{
    Q_D(VMimeType);
    QDomElement root = d->xmlDocument.documentElement();
    QDomNodeList mimeList = root.elementsByTagName("mime-type");
    for (uint i = 0; i < mimeList.length(); i++) {
        QDomElement xmlNode = mimeList.item(i).toElement();
        if (xmlNode.attribute("type") == d->mimeType) {
            d->mimeNode = xmlNode;
            return true;
        }
    }
    return false;
}

bool VMimeType::checkMagic(const QDomElement &magicNode, QIODevice *dev)
{
    QDomNodeList matchList = magicNode.elementsByTagName("match");
    return checkMagic(matchList, dev);
}

bool VMimeType::checkMagic(const QDomNodeList &matchList, QIODevice *dev)
{
    for (uint i = 0; i < matchList.length(); ++i) {
        QDomElement matchNode = matchList.item(i).toElement();
        if (matchNode.parentNode().nodeName() != "magic")
            continue;
        if (checkMatch(matchNode, dev)) {
            QDomNodeList subMatchList = matchNode.elementsByTagName("match");
            return (subMatchList.length() > 0) ? checkSubMagic(subMatchList, dev) : true;
        }
    }
    return false;
}

bool VMimeType::checkSubMagic(const QDomNodeList &matchList, QIODevice *dev)
{
    for (uint i = 0; i < matchList.length(); ++i) {
        QDomElement matchNode = matchList.item(i).toElement();
        if (matchNode.parentNode().nodeName() != "match")
            continue;
        if (checkMatch(matchNode, dev)) {
            QDomNodeList subMatchList = matchNode.elementsByTagName("match");
            return (subMatchList.length() > 0) ? checkSubMagic(subMatchList, dev) : true;
        }
    }
    return false;
}

bool VMimeType::checkMatch(const QDomElement &matchNode, QIODevice *dev)
{
    QString value = matchNode.attribute("value");
    QString mask = matchNode.attribute("mask");
    QString type = matchNode.attribute("type");
    QByteArray parsedValue, parsedMask;
    if (type == "string") {
        parsedValue = convert_string(value.toAscii());
        if (!mask.isEmpty())
            parsedMask = parse_string_mask(mask.toAscii(), parsedValue.length());
    } else if (type.contains("16"))
        parse_int_value(2, value.toAscii(), mask.toAscii(), &parsedValue, &parsedMask, type[0] != 'l');
    else if (type.contains("32"))
        parse_int_value(4, value.toAscii(), mask.toAscii(), &parsedValue, &parsedMask, type[0] != 'l');
    else if (type == "byte")
        parse_int_value(1, value.toAscii(), mask.toAscii(), &parsedValue, &parsedMask, false);
    else {
        qWarning("Vibe::VMimeType::checkMagic(): type not found!");
        return false;
    }
    return checkMagicString(matchNode.attribute("offset"), parsedValue, parsedMask, dev);
}

bool VMimeType::checkMagicString(const QString &offset, const QByteArray &value,
                                 const QByteArray &mask, QIODevice *dev)
{
    int startOffset, endOffset;
    extract_offset(offset, &startOffset, &endOffset);
    int valueLength = value.length();
    if (valueLength < 1)
        return false;
    if (!dev->seek(startOffset))
        return false;
    QByteArray fullData = dev->read(endOffset + valueLength + 1);
    if (fullData.length() < valueLength)
        return false;
    for (int i = 0; (startOffset + i) <= endOffset; ++i) {
        QByteArray data = fullData.mid(i, valueLength);
        if (data.length() < valueLength)
            break;
        bool match = true;
        if (mask.isEmpty())
            match = (data == value);
        else {
            for (int i = 0; i < valueLength; ++i) {
                if ((value[i] & mask[i]) != (data[i] & mask[i])) {
                    match = false;
                    break;
                }
            }
        }
        if (match)
            return true;
    }
    return false;
}

#include "vmimetype.moc"
