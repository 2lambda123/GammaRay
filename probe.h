/*
  probe.h

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

#ifndef GAMMARAY_PROBE_H
#define GAMMARAY_PROBE_H

#include <qobject.h>
#include "probeinterface.h"
#include <QReadWriteLock>
#include <QSet>
#include <QQueue>

class QPoint;
class QTimer;
class QGraphicsItem;

namespace GammaRay {

class ConnectionModel;
class ObjectListModel;
class ObjectTreeModel;
class ToolModel;

class MainWindow;

/**
 * Creates Probe instance in main thread and deletes self afterwards.
 */
class ProbeCreator : public QObject
{
  Q_OBJECT
  public:
    enum Type {
      CreateOnly,
      CreateAndFindExisting
    };
    ProbeCreator(Type t);

  private slots:
    void createProbe();

  private:
    Type m_type;
};

class Q_DECL_EXPORT Probe : public QObject, public ProbeInterface
{
  Q_OBJECT
  public:
    virtual ~Probe();
    static Probe *instance();
    static bool isInitialized();

    static void objectAdded(QObject *obj, bool fromCtor = false);
    static void objectRemoved(QObject *obj);
    static void connectionAdded(QObject *sender, const char *signal,
                                QObject *receiver, const char *method,
                                Qt::ConnectionType type);
    static void connectionRemoved(QObject *sender, const char *signal,
                                  QObject *receiver, const char *method);

    static void findExistingObjects();

    QAbstractItemModel *objectListModel() const;
    QAbstractItemModel *objectTreeModel() const;
    QAbstractItemModel *connectionModel() const;
    ToolModel *toolModel() const;

    static const char *connectLocation(const char *member);

    GammaRay::MainWindow *window() const;
    void setWindow(GammaRay::MainWindow *window);

    QObject *probe() const;

    /**
     * Lock this to check the validity of a QObject
     * and to access it safely afterwards.
     */
    QReadWriteLock *objectLock() const;
    /**
     * check whether @p obj is still valid
     *
     * NOTE: the objectLock must be locked when this is called!
     */
    bool isValidObject(QObject *obj) const;

  signals:
    /**
     * Emitted when the user selected @p widget at position @p pos in the probed application.
     */
    void widgetSelected(QWidget *widget, const QPoint &pos);

    void objectCreated(QObject *obj);
    void objectDestroyed(QObject *obj);

  protected:
    bool eventFilter(QObject *receiver, QEvent *event);

  private slots:
    void delayedInit();
    void queuedObjectsFullyConstructed();
    void handleObjectDestroyed(QObject *obj);

  private:
    void objectFullyConstructed(QObject *obj);

    explicit Probe(QObject *parent = 0);
    static void addObjectRecursive(QObject *obj);
    static Probe *s_instance;

    ObjectListModel *m_objectListModel;
    ObjectTreeModel *m_objectTreeModel;
    ConnectionModel *m_connectionModel;
    ToolModel *m_toolModel;
    GammaRay::MainWindow *m_window;
    // ensures proper information is returned by isValidObject by
    // locking it in objectAdded/Removed
    static QReadWriteLock s_lock;
    QSet<QObject*> m_validObjects;
    QQueue<QObject*> m_queuedObjects;
    QTimer *m_queueTimer;

    friend class ProbeCreator;
};

}

#endif // GAMMARAY_PROBE_H
