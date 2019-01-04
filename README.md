[![Build Status](https://secure.travis-ci.org/abbat/avalon.png?branch=master)](http://travis-ci.org/abbat/avalon)

# Avalon

Кросс-платформенный клиент для offline работы с форумами [RSDN](https://rsdn.org/projects/janus/article/article.xml), аналогичный проекту [RSDN@Home](https://rsdn.org/projects/janus/article/article.xml) команды RSDN под Windows.

# Загрузка / Установка

* [Debian, Ubuntu](http://software.opensuse.org/download.html?project=home:antonbatenev:avalon&package=avalon2)
* [Fedora, openSUSE, CentOS](http://software.opensuse.org/download.html?project=home:antonbatenev:avalon&package=avalon2)
* [Ubuntu PPA](https://launchpad.net/~abbat/+archive/ubuntu/avalon) - `ppa:abbat/avalon`
* [Windows](https://yadi.sk/d/W9kZi-GLqTN7G)

# FAQ

## Установил avalon, нажал кнопку синхронизации - он зациклился и не отвечает

Во время первой синхронизации avalon загружает полный список пользователей и сообщений из подписанных форумов. Поскольку веб-сервис RSDN имеет ограничение на количество данных, отдаваемых за одну операцию (1000 строк), то для загрузки ~75 тыс. пользователей потребуется ~75 запросов к веб-сервису. Т.о. первая синхронизация может занять длительное время в течении которого может "мигать" индикация синхронизации - запаситесь терпением.

## Не отправляются/не получаются сообщения - соединение отваливается по таймауту

С RSDN есть странная периодическая проблема с прохождением IP пакетов - по предварительной версии один из маршрутизаторов в районе сервера отбрасывает пакеты более определенного размера не сумев договориться с соседом о размере фрагмента. Решение состоит в установке [TOR клиента](http://ru.wikipedia.org/wiki/TOR), настройке конфига `torrc`:

```
SocksPort 9050
SocksListenAddress 127.0.0.1
```

и прописывании SOCKS-5 прокси на адрес 127.0.0.1 и порт 9050 в avalon (меню `"Сервис / Настройки"` вкладка `"Сеть"`).

## Не работает подсветка кода

Скачайте [highlight.js](https://highlightjs.org) и распакуйте по пути `~/.avalon/highlight`.

## Настройка отображения сообщений

Поместите файл [style.css](https://github.com/abbat/avalon/blob/master/src/style.css) по пути `~/.avalon/avalon.css` и переопределите необходимые параметры для размера шрифта, цвета и т.д.
