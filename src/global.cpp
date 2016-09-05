#include "global.h"
//----------------------------------------------------------------------------------------------
#include "storage/storage_factory.h"
//----------------------------------------------------------------------------------------------
AGlobal* g_global = NULL;
//----------------------------------------------------------------------------------------------

AGlobal* AGlobal::getInstance ()
{
	if (g_global != NULL)
		return g_global;

	return new AGlobal();
}
//----------------------------------------------------------------------------------------------

AGlobal::AGlobal ()
{
	g_global = this;

#if QT_VERSION < 0x050400
	// предпочитаемый список шифров tls1 (см. ниже про версии Qt и поддержку)
	// openssl ciphers -tls1 -s 'HIGH:!TLSv1.2:!aNULL:!MD5:!3DES:!CAMELLIA:!SRP:!PSK:@STRENGTH'
	static const char* AVALON_CIPHERS[] =
	{
		"ECDHE-RSA-AES256-SHA",     // SSLv3 Kx=ECDH        Au=RSA   Enc=AES(256) Mac=SHA1
		"ECDHE-ECDSA-AES256-SHA",   // SSLv3 Kx=ECDH        Au=ECDSA Enc=AES(256) Mac=SHA1
		"DHE-RSA-AES256-SHA",       // SSLv3 Kx=DH          Au=RSA   Enc=AES(256) Mac=SHA1
		"DHE-DSS-AES256-SHA",       // SSLv3 Kx=DH          Au=DSS   Enc=AES(256) Mac=SHA1
		"ECDH-RSA-AES256-SHA",      // SSLv3 Kx=ECDH/RSA    Au=ECDH  Enc=AES(256) Mac=SHA1
		"ECDH-ECDSA-AES256-SHA",    // SSLv3 Kx=ECDH/ECDSA  Au=ECDH  Enc=AES(256) Mac=SHA1
		"AES256-SHA",               // SSLv3 Kx=RSA         Au=RSA   Enc=AES(256) Mac=SHA1
		"ECDHE-RSA-AES128-SHA",     // SSLv3 Kx=ECDH        Au=RSA   Enc=AES(128) Mac=SHA1
		"ECDHE-ECDSA-AES128-SHA",   // SSLv3 Kx=ECDH        Au=ECDSA Enc=AES(128) Mac=SHA1
		"DHE-RSA-AES128-SHA",       // SSLv3 Kx=DH          Au=RSA   Enc=AES(128) Mac=SHA1
		"DHE-DSS-AES128-SHA",       // SSLv3 Kx=DH          Au=DSS   Enc=AES(128) Mac=SHA1
		"ECDH-RSA-AES128-SHA",      // SSLv3 Kx=ECDH/RSA    Au=ECDH  Enc=AES(128) Mac=SHA1
		"ECDH-ECDSA-AES128-SHA",    // SSLv3 Kx=ECDH/ECDSA  Au=ECDH  Enc=AES(128) Mac=SHA1
		"AES128-SHA",               // SSLv3 Kx=RSA         Au=RSA   Enc=AES(128) Mac=SHA1
		NULL
	};
#else
	// предпочитаемый список шифров tls1.2 (см. ниже про версии Qt и поддержку)
	// openssl ciphers -tls1 -s 'HIGH:!aNULL:!MD5:!3DES:!CAMELLIA:!SRP:!PSK:@STRENGTH'
	static const char* AVALON_CIPHERS[] =
	{
		"ECDHE-RSA-AES256-GCM-SHA384",     // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AESGCM(256) Mac=AEAD
		"ECDHE-ECDSA-AES256-GCM-SHA384",   // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AESGCM(256) Mac=AEAD
		"ECDHE-RSA-AES256-SHA384",         // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AES(256)    Mac=SHA384
		"ECDHE-ECDSA-AES256-SHA384",       // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AES(256)    Mac=SHA384
		"ECDHE-RSA-AES256-SHA",            // SSLv3   Kx=ECDH       Au=RSA   Enc=AES(256)    Mac=SHA1
		"ECDHE-ECDSA-AES256-SHA",          // SSLv3   Kx=ECDH       Au=ECDSA Enc=AES(256)    Mac=SHA1
		"DHE-DSS-AES256-GCM-SHA384",       // TLSv1.2 Kx=DH         Au=DSS   Enc=AESGCM(256) Mac=AEAD
		"DHE-RSA-AES256-GCM-SHA384",       // TLSv1.2 Kx=DH         Au=RSA   Enc=AESGCM(256) Mac=AEAD
		"DHE-RSA-AES256-SHA256",           // TLSv1.2 Kx=DH         Au=RSA   Enc=AES(256)    Mac=SHA256
		"DHE-DSS-AES256-SHA256",           // TLSv1.2 Kx=DH         Au=DSS   Enc=AES(256)    Mac=SHA256
		"DHE-RSA-AES256-SHA",              // SSLv3   Kx=DH         Au=RSA   Enc=AES(256)    Mac=SHA1
		"DHE-DSS-AES256-SHA",              // SSLv3   Kx=DH         Au=DSS   Enc=AES(256)    Mac=SHA1
		"ECDH-RSA-AES256-GCM-SHA384",      // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AESGCM(256) Mac=AEAD
		"ECDH-ECDSA-AES256-GCM-SHA384",    // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AESGCM(256) Mac=AEAD
		"ECDH-RSA-AES256-SHA384",          // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AES(256)    Mac=SHA384
		"ECDH-ECDSA-AES256-SHA384",        // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AES(256)    Mac=SHA384
		"ECDH-RSA-AES256-SHA",             // SSLv3   Kx=ECDH/RSA   Au=ECDH  Enc=AES(256)    Mac=SHA1
		"ECDH-ECDSA-AES256-SHA",           // SSLv3   Kx=ECDH/ECDSA Au=ECDH  Enc=AES(256)    Mac=SHA1
		"AES256-GCM-SHA384",               // TLSv1.2 Kx=RSA        Au=RSA   Enc=AESGCM(256) Mac=AEAD
		"AES256-SHA256",                   // TLSv1.2 Kx=RSA        Au=RSA   Enc=AES(256)    Mac=SHA256
		"AES256-SHA",                      // SSLv3   Kx=RSA        Au=RSA   Enc=AES(256)    Mac=SHA1
		"ECDHE-RSA-AES128-GCM-SHA256",     // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AESGCM(128) Mac=AEAD
		"ECDHE-ECDSA-AES128-GCM-SHA256",   // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AESGCM(128) Mac=AEAD
		"ECDHE-RSA-AES128-SHA256",         // TLSv1.2 Kx=ECDH       Au=RSA   Enc=AES(128)    Mac=SHA256
		"ECDHE-ECDSA-AES128-SHA256",       // TLSv1.2 Kx=ECDH       Au=ECDSA Enc=AES(128)    Mac=SHA256
		"ECDHE-RSA-AES128-SHA",            // SSLv3   Kx=ECDH       Au=RSA   Enc=AES(128)    Mac=SHA1
		"ECDHE-ECDSA-AES128-SHA",          // SSLv3   Kx=ECDH       Au=ECDSA Enc=AES(128)    Mac=SHA1
		"DHE-DSS-AES128-GCM-SHA256",       // TLSv1.2 Kx=DH         Au=DSS   Enc=AESGCM(128) Mac=AEAD
		"DHE-RSA-AES128-GCM-SHA256",       // TLSv1.2 Kx=DH         Au=RSA   Enc=AESGCM(128) Mac=AEAD
		"DHE-RSA-AES128-SHA256",           // TLSv1.2 Kx=DH         Au=RSA   Enc=AES(128)    Mac=SHA256
		"DHE-DSS-AES128-SHA256",           // TLSv1.2 Kx=DH         Au=DSS   Enc=AES(128)    Mac=SHA256
		"DHE-RSA-AES128-SHA",              // SSLv3   Kx=DH         Au=RSA   Enc=AES(128)    Mac=SHA1
		"DHE-DSS-AES128-SHA",              // SSLv3   Kx=DH         Au=DSS   Enc=AES(128)    Mac=SHA1
		"ECDH-RSA-AES128-GCM-SHA256",      // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AESGCM(128) Mac=AEAD
		"ECDH-ECDSA-AES128-GCM-SHA256",    // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AESGCM(128) Mac=AEAD
		"ECDH-RSA-AES128-SHA256",          // TLSv1.2 Kx=ECDH/RSA   Au=ECDH  Enc=AES(128)    Mac=SHA256
		"ECDH-ECDSA-AES128-SHA256",        // TLSv1.2 Kx=ECDH/ECDSA Au=ECDH  Enc=AES(128)    Mac=SHA256
		"ECDH-RSA-AES128-SHA",             // SSLv3   Kx=ECDH/RSA   Au=ECDH  Enc=AES(128)    Mac=SHA1
		"ECDH-ECDSA-AES128-SHA",           // SSLv3   Kx=ECDH/ECDSA Au=ECDH  Enc=AES(128)    Mac=SHA1
		"AES128-GCM-SHA256",               // TLSv1.2 Kx=RSA        Au=RSA   Enc=AESGCM(128) Mac=AEAD
		"AES128-SHA256",                   // TLSv1.2 Kx=RSA        Au=RSA   Enc=AES(128)    Mac=SHA256
		"AES128-SHA",                      // SSLv3   Kx=RSA        Au=RSA   Enc=AES(128)    Mac=SHA1
		NULL
	};
#endif

	// поддерживаемый список шифров
	QList<QSslCipher> cipher_list;

	const char** ciphers = AVALON_CIPHERS;
	while ((*ciphers) != NULL)
	{
		QSslCipher cipher(*ciphers, QSsl::SslV3);
		if (cipher.isNull() == false)
			cipher_list.append(cipher);

		ciphers++;
	}

	// задание конфигурации ssl по умолчанию
	QSslConfiguration ssl_default = QSslConfiguration::defaultConfiguration();

#if QT_VERSION < 0x050000
	// до версии 5.0.x поддерживается только TLSv1
	// т.о. для отключения SSLv3 включаем принудительный TLSv1
	ssl_default.setProtocol(QSsl::TlsV1);
#else
	#if QT_VERSION < 0x050400
		// до версии 5.4.0 для QSsl::SecureProtocols используется
		// SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2
		// т.о. для отключения SSLv3 включаем принудительный TLSv1
		ssl_default.setProtocol(QSsl::TlsV1_0);
	#else
		// в версии 5.4.0 для QSsl::SecureProtocols используется
		// SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3
		// т.о. можем работать TLSv1+
		ssl_default.setProtocol(QSsl::SecureProtocols);
	#endif
#endif

	ssl_default.setCiphers(cipher_list);

	QSslConfiguration::setDefaultConfiguration(ssl_default);

	// значения по умолчанию
	AnonymousName = QString::fromUtf8("Аноним");
	DateFormat    = "dd.MM.yyyy hh:mm:ss";

	Me.ID = 0;

	reload();
}
//----------------------------------------------------------------------------------------------

AGlobal::~AGlobal ()
{
	g_global = NULL;
}
//----------------------------------------------------------------------------------------------

void AGlobal::reload ()
{
	QSettings settings;

#ifdef AVALON_USE_ZLIB
	Compression = settings.value("storage/compression", false).toInt();
#endif

	m_rsdn_proto = settings.value("rsdn/proto",  "https").toString().toLower();
	m_rsdn_host  = settings.value("rsdn/domain", "rsdn.org").toString().toLower();

	m_rsdn_url = QString("%1://%2").arg(m_rsdn_proto).arg(m_rsdn_host);

	// информация о текущем пользователе avalon
	Me.ID             = -1;
	Me.Name           = "(n/a)";
	Me.Nick           = "(n/a)";
	Me.RealName       = "(n/a)";
	Me.Email          = "(n/a)";
	Me.Homepage       = "(n/a)";
	Me.Specialization = "(n/a)";
	Me.WhereFrom      = "(n/a)";
	Me.Origin         = "(n/a)";

	std::auto_ptr<IAStorage> storage(AStorageFactory::getStorage());

	if (storage.get() != NULL)
	{
		Me.Name = settings.value("rsdn/login", "").toString();
		storage->whoAmI(Me);
	}
}
//----------------------------------------------------------------------------------------------
