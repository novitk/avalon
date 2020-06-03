#!/usr/bin/env bash

set -e

# версия qt
if [ -z "${QT_SELECT}" ]; then
	export QT_SELECT=5
fi

# имя проекта
PROJECT_NAME="avalon"

# очистка
if [ -f "Makefile" ]; then
	make clean
fi

# опции (модули) qt
QT_OPTS="network sql"
if [ "${QT_SELECT}" -eq "4" ]; then
	QT_OPTS="${QT_OPTS} webkit"
else
	QT_OPTS="${QT_OPTS} core widgets webkitwidgets"
fi

# создание pro-файла
BUILD_DIR=$(pwd)
qmake -project -recursive -Wall -nopwd -o ${PROJECT_NAME}.pro \
    "CONFIG      += debug_and_release"                        \
    "QT          += ${QT_OPTS}"                               \
    "INCLUDEPATH += src"                                      \
    "LIBS        += -laspell -lz"                             \
    "macx { QMAKE_CXX = clang }"                              \
    "macx { ICON = icons/avalon.icns }"                       \
    "${BUILD_DIR}/src"

# создание make-файлов
qmake ${PROJECT_NAME}.pro

# создание бинарника
make $1

# удаление символов
strip -s ${PROJECT_NAME}

# запуск
if [ -z "${TRAVIS}" ]; then
	./${PROJECT_NAME}
fi
