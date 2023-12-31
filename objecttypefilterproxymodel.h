/*
  objecttypefilterproxymodel.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2010-2011 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

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

#ifndef GAMMARAY_OBJECTTYPEFILTERPROXYMODEL_H
#define GAMMARAY_OBJECTTYPEFILTERPROXYMODEL_H

#include <qsortfilterproxymodel.h>
#include "objectlistmodel.h"

namespace GammaRay {

template <typename T>
class ObjectTypeFilterProxyModel : public QSortFilterProxyModel
{
  public:
    explicit ObjectTypeFilterProxyModel(QObject *parent = 0) : QSortFilterProxyModel(parent)
    {
      setDynamicSortFilter(true);
    }

  protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
      const QModelIndex source_index = sourceModel()->index(source_row, 0, source_parent);
      if (!source_index.isValid()) {
        return false;
      }

      QObject *obj = source_index.data(ObjectModel::ObjectRole).value<QObject*>();
      if (!qobject_cast<T*>(obj)) {
        return false;
      }
      return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
};

}

#endif // GAMMARAY_OBJECTTYPEFILTERPROXYMODEL_H
