/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2000-2005 David Faure
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    David Faure <faure@kde.org>
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

#ifndef VABSTRACTCOMPRESSIONFILTER_H
#define VABSTRACTCOMPRESSIONFILTER_H

#include <QObject>
#include <QString>

#include <VibeCore/VGlobal>

class QIODevice;

/** \addtogroup core Core Kit
 *  @{
 */

class VAbstractCompressionFilterPrivate;

/**
 * This is the base class for compression filters
 * such as gzip and bzip2. It's pretty much internal.
 * Don't use directly, use VCompressionFilter instead.
 *
 * \author David Faure <faure@kde.org>
 * \author Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * @internal
 */
class VIBE_EXPORT VAbstractCompressionFilter
{
public:
    VAbstractCompressionFilter();
    virtual ~VAbstractCompressionFilter();

    /**
     * Sets the device on which the filter will work
     * @param dev the device on which the filter will work
     * @param autodelete if true, @p dev is deleted when the filter is deleted
     */
    void setDevice(QIODevice *dev, bool autodelete = false);
    // Note that this isn't in the constructor, because of KLibFactory::create,
    // but it should be called before using the filterbase !

    /**
     * Returns the device on which the filter will work.
     * @returns the device on which the filter will work
     */
    QIODevice *device();
    /** \internal */
    virtual void init(int mode) = 0;     // KDE5 TODO: return a bool
    /** \internal */
    virtual int mode() const = 0;
    /** \internal */
    virtual void terminate();
    /** \internal */
    virtual void reset();
    /** \internal */
    virtual bool readHeader() = 0;
    /** \internal */
    virtual bool writeHeader(const QByteArray &filename) = 0;
    /** \internal */
    virtual void setOutBuffer(char *data, uint maxlen) = 0;
    /** \internal */
    virtual void setInBuffer(const char *data, uint size) = 0;
    /** \internal */
    virtual bool inBufferEmpty() const;
    /** \internal */
    virtual int  inBufferAvailable() const = 0;
    /** \internal */
    virtual bool outBufferFull() const;
    /** \internal */
    virtual int  outBufferAvailable() const = 0;

    /** \internal */
    enum Result { Ok, End, Error };
    /** \internal */
    virtual Result uncompress() = 0;
    /** \internal */
    virtual Result compress(bool finish) = 0;

    /**
     * \internal
     */
    enum FilterFlags {
        NoHeaders = 0,
        WithHeaders = 1
    };

    /**
     * \internal
     */
    void setFilterFlags(FilterFlags flags);
    FilterFlags filterFlags() const;

    /**
     * Call this to create the appropriate filter for the file
     * named @p fileName.
     * @param fileName the name of the file to filter
     * @return the filter for the @p fileName, or 0 if not found
     */
    static VAbstractCompressionFilter *findFilterByFileName(const QString &fileName);

    /**
     * Call this to create the appropriate filter for the mimetype
     * @p mimeType. For instance application/x-gzip.
     * @param mimeType the mime type of the file to filter
     * @return the filter for the @p mimeType, or 0 if not found
     */
    static VAbstractCompressionFilter *findFilterByMimeType(const QString &mimeType);

protected: // TODO KDE5: move to d pointer
    QIODevice *m_dev;
    bool m_bAutoDel;
protected:
    /** Virtual hook, used to add new "virtual" functions while maintaining
    binary compatibility. Unused in this class.
    */
    virtual void virtual_hook(int id, void *data);
private:
    Q_DISABLE_COPY(VAbstractCompressionFilter)

    VAbstractCompressionFilterPrivate *const d;
};

/** @}*/

#endif // VABSTRACTCOMPRESSIONFILTER_H
