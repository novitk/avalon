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
#if QT_VERSION < 0x050300
	ADatabaseError::setLastError(QSqlDatabase::lastError().databaseText() + "\n" + QSqlDatabase::lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + QString::number(QSqlDatabase::lastError().number()));
#else
	ADatabaseError::setLastError(QSqlDatabase::lastError().databaseText() + "\n" + QSqlDatabase::lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + QSqlDatabase::lastError().nativeErrorCode());
#endif
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
#if QT_VERSION < 0x050300
		ADatabaseError::setLastError(query->lastError().databaseText() + "\n" + query->lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + QString::number(query->lastError().number()));
#else
		ADatabaseError::setLastError(query->lastError().databaseText() + "\n" + query->lastError().driverText() + "\n" + QString::fromUtf8("Ошибка: ") + query->lastError().nativeErrorCode());
#endif

		delete query;

		return NULL;
	}

	return query;
}
//----------------------------------------------------------------------------------------------
