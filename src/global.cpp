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

	// белый список шифров (в порядке предпочтения)
	static const char* AVALON_WHITELIST_CIPHERS[] =
	{
#if QT_VERSION < 0x050400
		// белый список шифров tls1 (см. ниже про версии Qt и поддержку)
		"ECDHE-ECDSA-AES128-SHA",   // TLSv1 Kx=ECDH Au=ECDSA Enc=AES(128) Mac=SHA1
		"ECDHE-RSA-AES128-SHA",     // TLSv1 Kx=ECDH Au=RSA   Enc=AES(128) Mac=SHA1
		"ECDHE-ECDSA-AES256-SHA",   // TLSv1 Kx=ECDH Au=ECDSA Enc=AES(256) Mac=SHA1
		"ECDHE-RSA-AES256-SHA",     // TLSv1 Kx=ECDH Au=RSA   Enc=AES(256) Mac=SHA1
		"AES128-SHA",               // SSLv3 Kx=RSA  Au=RSA   Enc=AES(128) Mac=SHA1
		"AES256-SHA",               // SSLv3 Kx=RSA  Au=RSA   Enc=AES(256) Mac=SHA1
#else
		// белый список шифров tls1+ (см. ниже про версии Qt и поддержку)
		"TLS_AES_128_GCM_SHA256",          // TLSv1.3 Kx=any  Au=any   Enc=AESGCM(128) Mac=AEAD
		"TLS_AES_256_GCM_SHA384",          // TLSv1.3 Kx=any  Au=any   Enc=AESGCM(256) Mac=AEAD
		"ECDHE-ECDSA-AES128-GCM-SHA256",   // TLSv1.2 Kx=ECDH Au=ECDSA Enc=AESGCM(128) Mac=AEAD
		"ECDHE-RSA-AES128-GCM-SHA256",     // TLSv1.2 Kx=ECDH Au=RSA   Enc=AESGCM(128) Mac=AEAD
		"ECDHE-ECDSA-AES128-SHA256",       // TLSv1.2 Kx=ECDH Au=ECDSA Enc=AES(128)    Mac=SHA256
		"ECDHE-RSA-AES128-SHA256",         // TLSv1.2 Kx=ECDH Au=RSA   Enc=AES(128)    Mac=SHA256
		"ECDHE-ECDSA-AES128-SHA",          // TLSv1   Kx=ECDH Au=ECDSA Enc=AES(128)    Mac=SHA1
		"ECDHE-RSA-AES128-SHA",            // TLSv1   Kx=ECDH Au=RSA   Enc=AES(128)    Mac=SHA1
		"ECDHE-ECDSA-AES256-GCM-SHA384",   // TLSv1.2 Kx=ECDH Au=ECDSA Enc=AESGCM(256) Mac=AEAD
		"ECDHE-RSA-AES256-GCM-SHA384",     // TLSv1.2 Kx=ECDH Au=RSA   Enc=AESGCM(256) Mac=AEAD
		"ECDHE-ECDSA-AES256-SHA384",       // TLSv1.2 Kx=ECDH Au=ECDSA Enc=AES(256)    Mac=SHA384
		"ECDHE-RSA-AES256-SHA384",         // TLSv1.2 Kx=ECDH Au=RSA   Enc=AES(256)    Mac=SHA384
		"ECDHE-ECDSA-AES256-SHA",          // TLSv1   Kx=ECDH Au=ECDSA Enc=AES(256)    Mac=SHA1
		"ECDHE-RSA-AES256-SHA",            // TLSv1   Kx=ECDH Au=RSA   Enc=AES(256)    Mac=SHA1
		"AES128-GCM-SHA256",               // TLSv1.2 Kx=RSA  Au=RSA   Enc=AESGCM(128) Mac=AEAD
		"AES128-SHA256",                   // TLSv1.2 Kx=RSA  Au=RSA   Enc=AES(128)    Mac=SHA256
		"AES128-SHA",                      // SSLv3   Kx=RSA  Au=RSA   Enc=AES(128)    Mac=SHA1
		"AES256-GCM-SHA384",               // TLSv1.2 Kx=RSA  Au=RSA   Enc=AESGCM(256) Mac=AEAD
		"AES256-SHA256",                   // TLSv1.2 Kx=RSA  Au=RSA   Enc=AES(256)    Mac=SHA256
		"AES256-SHA",                      // SSLv3   Kx=RSA  Au=RSA   Enc=AES(256)    Mac=SHA1
#endif
		NULL
	};

	// конфигурация ssl по умолчанию
	QSslConfiguration ssl_config = QSslConfiguration::defaultConfiguration();
	// список шифров по умолчанию
	QList<QSslCipher> default_ciphers = ssl_config.ciphers();

	// поддерживаемый список шифров (пересечение белого списка и умолчаний)
	QList<QSslCipher> cipher_list;

	const char** whitelist_ciphers = AVALON_WHITELIST_CIPHERS;
	while ((*whitelist_ciphers) != NULL)
	{
		QString whitelist_cipher = QString(*whitelist_ciphers);
		for (int i = 0; i < default_ciphers.count(); i++)
		{
			if (default_ciphers[i].name() == whitelist_cipher) {
				cipher_list.append(default_ciphers[i]);
				break;
			}
		}

		whitelist_ciphers++;
	}

#if QT_VERSION < 0x050000
	// до версии 5.0.x поддерживается только QSsl::TLSv1
	// т.о. для отключения SSLv3 включаем принудительный TLSv1
	ssl_config.setProtocol(QSsl::TlsV1);
#else
	#if QT_VERSION < 0x050400
		// до версии 5.4.0 для QSsl::SecureProtocols используется
		// SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2
		// т.о. для отключения SSLv3 включаем принудительный TLSv1
		ssl_config.setProtocol(QSsl::TlsV1_0);
	#else
		// в версии 5.4.0 для QSsl::SecureProtocols используется
		// SSLv23_client_method и SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3
		// т.о. можем работать TLSv1+
		ssl_config.setProtocol(QSsl::SecureProtocols);
	#endif
#endif

	ssl_config.setCiphers(cipher_list);

	QSslConfiguration::setDefaultConfiguration(ssl_config);

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

	m_rsdn_proto = settings.value("rsdn/proto", "https").toString().toLower();
	m_rsdn_host  = settings.value("rsdn/host",  "rsdn.org").toString().toLower();

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
