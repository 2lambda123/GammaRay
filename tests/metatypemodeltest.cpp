/*
  metatypemodeltest.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <core/tools/metatypebrowser/metatypesmodel.h>
#include <ui/tools/metatypebrowser/metatypesclientmodel.h>
#include <common/tools/metatypebrowser/metatyperoles.h>

#include <common/objectid.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>

using namespace GammaRay;

class MetaTypeModelTest : public QObject
{
    Q_OBJECT
private:
    QModelIndex indexForName(const QString &name, QAbstractItemModel *model)
    {
        for (int i = 0; i < model->rowCount(); ++i) {
            const auto idx = model->index(i, 0);
            if (idx.data().toString() == name)
                return idx;
        }
        return QModelIndex();
    }

private slots:
    void testModel()
    {
        MetaTypesModel srcModel;
        MetaTypesClientModel model;
        model.setSourceModel(&srcModel);
        ModelTest modelTest(&model);
        QVERIFY(model.rowCount() > 0);

        auto idx = indexForName("QObject*", &model);
        QVERIFY(idx.isValid());
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QVERIFY(!idx.data(MetaTypeRoles::MetaObjectIdRole).value<ObjectId>().isNull());
#endif

        idx = indexForName("int", &model);
        QVERIFY(idx.isValid());
        QVERIFY(idx.data(MetaTypeRoles::MetaObjectIdRole).isNull());
    }

};

QTEST_MAIN(MetaTypeModelTest)

#include "metatypemodeltest.moc"
