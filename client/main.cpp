#include "remotemodel.h"
#include "client.h"
#include "selectionmodelclient.h"
#include "objectclient.h"
#include "clienttoolmodel.h"

#include <network/objectbroker.h>
#include <network/streamoperators.h>

#include <QApplication>
#include <QStringList>

using namespace GammaRay;

static NetworkObject* objectFactory(const QString &name)
{
  return new ObjectClient(name, qApp);
}

static QAbstractItemModel* modelFactory(const QString &name)
{
  return new RemoteModel(name, qApp);
}

static QItemSelectionModel* selectionModelFactory(QAbstractItemModel* model)
{
  return new SelectionModelClient(model->objectName() + ".selection", model, qApp);
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  StreamOperators::registerOperators();

  QString hostName = QLatin1String("localhost");
  quint16 port = Client::defaultPort();

  if (app.arguments().size() == 2) {
    hostName = app.arguments().at(1);
  } else if (app.arguments().size() == 3) {
    hostName = app.arguments().at(1);
    port = app.arguments().at(2).toUShort();
  }

  Client client;
  client.connectToHost(hostName, port);
  QObject::connect(&client, SIGNAL(disconnected()), &app, SLOT(quit()));

  // TODO make this async, show some status indicator/splash screen while connecting

  ObjectBroker::setObjectFactoryCallback(objectFactory);
  ObjectBroker::setModelFactoryCallback(modelFactory);
  ObjectBroker::setSelectionModelFactoryCallback(selectionModelFactory);

  ClientToolModel toolModel;
  ObjectBroker::registerModel("com.kdab.GammaRay.ToolModel", &toolModel);

  return app.exec();
}
