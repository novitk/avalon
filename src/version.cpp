/*!
 * \file
 * \brief Обрпаботка шаблона файла версии, который меняется при каждом билде
 */
//----------------------------------------------------------------------------------------------
#include "version.h"
#include "sysheaders.h"
//----------------------------------------------------------------------------------------------

int getBuildNumber ()
{
	return AVALON_BUILD;
}
//----------------------------------------------------------------------------------------------

QString getVersionString ()
{
	return QString("3.0.") + QString::number(getBuildNumber());
}
//----------------------------------------------------------------------------------------------

QString getAgentString ()
{
	return QString("avalon/") + getVersionString() + " (https://github.com/abbat/avalon)";
}
//----------------------------------------------------------------------------------------------

QString getTagline ()
{
	QSettings settings;

	QString tagline = settings.value("ui/tagline", "%%version%%").toString();

	tagline = tagline.replace("%%version%%", "[url=https://github.com/abbat/avalon]avalon/" + getVersionString() + "[/url]");

	return tagline;
}
//----------------------------------------------------------------------------------------------
