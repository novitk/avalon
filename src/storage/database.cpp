#include "database.h"
//----------------------------------------------------------------------------------------------

ADatabase::ADatabase  (const QString& type) : ADatabaseError(), QSqlDatabase (type)
{
}
//----------------------------------------------------------------------------------------------

ADatabase::~ADatabase ()
{
	close();
}
//----------------------------------------------------------------------------------------------

void ADatabase::setLastError ()
{
	ADatabaseError::setLastError(QSqlDatabase::lastError().databaseText() + "\n" + QSqlDatabase::lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + QSqlDatabase::lastError().nativeErrorCode());
}
//----------------------------------------------------------------------------------------------

AQuery* ADatabase::createQuery (const QString& sql)
{
	return new AQuery(this, sql);
}
//----------------------------------------------------------------------------------------------

AQuery* ADatabase::createPreparedQuery (const QString& sql)
{
	AQuery* query = new AQuery(this);

	if (query->prepare(sql) == false)
	{
		ADatabaseError::setLastError(query->lastError().databaseText() + "\n" + query->lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + query->lastError().nativeErrorCode());

		delete query;

		return NULL;
	}

	return query;
}
//----------------------------------------------------------------------------------------------
