/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (C) 2009 Dzimi Mve Alexandre
 * Copyright (c) 2011 Pier Luigi Fiorini
 *
 * Author(s):
 *	Dzimi Mve Alexandre <dzimiwine@gmail.com>
 *	Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Vibe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef VBREADCRUMBBAR_H
#define VBREADCRUMBBAR_H

#include <QWidget>

#include <VibeCore/VGlobal>

class QComboBox;
template < typename T >
class QList;

class VAbstractBreadCrumbModel;
class VBreadCrumbBarPrivate;

class VIBE_EXPORT VBreadCrumbBar : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VBreadCrumbBar)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable RESET goToDefault DESIGNABLE true)
    Q_PROPERTY(QString location READ location WRITE setLocation DESIGNABLE true NOTIFY locationChanged)
public:
    explicit VBreadCrumbBar(QWidget *parent = 0);
    ~VBreadCrumbBar();

    QSize sizeHint() const;

    bool isEditable() const;
    void setEditable(bool);

    QString location() const;
    void setLocation(const QString &);

    QComboBox *comboBox() const;

    VAbstractBreadCrumbModel *model() const;
    void setModel(VAbstractBreadCrumbModel *);

    void refresh();

public slots:
    void edit();
    void goToDefault();

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

signals:
    void locationChanged(const QString &);

private:
    VBreadCrumbBarPrivate *d_ptr;
};

#endif // VBREADCRUMBBAR_H
