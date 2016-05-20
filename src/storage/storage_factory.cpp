#include "storage_factory.h"
//----------------------------------------------------------------------------------------------
#include "sqlite_storage.h"
//----------------------------------------------------------------------------------------------

IAStorage* AStorageFactory::getStorage ()
{
	// чтение типа хранилища из конфига
	QSettings settings;

	QString type = settings.value("storage/type", "SQLite").toString();

	if (type == "SQLite")
		return new ASQLiteStorage();

	return NULL;
}
//----------------------------------------------------------------------------------------------
