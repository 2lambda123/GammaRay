/*
  clientpropertymodel.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2018 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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

#include "clientpropertymodel.h"

#include <common/propertymodel.h>

using namespace GammaRay;

ClientPropertyModel::ClientPropertyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
}

ClientPropertyModel::~ClientPropertyModel()
{
}

QVariant ClientPropertyModel::data(const QModelIndex& index, int role) const
{
    return QIdentityProxyModel::data(index, role);
}

QVariant ClientPropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Property");
        case 1:
            return tr("Value");
        case 2:
            return tr("Type");
        case 3:
            return tr("Class");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}
