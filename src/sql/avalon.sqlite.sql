/**
 *
 * Схема хранилища SQLite
 *
 **/

BEGIN TRANSACTION;

PRAGMA page_size = 32768;
PRAGMA encoding = "UTF-8";
PRAGMA auto_vacuum = 2;
PRAGMA incremental_vacuum(10);

CREATE TABLE `broken`
(
	--
	-- список поломанных топиков/сообщений
	--
	`id_message` INTEGER NOT NULL PRIMARY KEY,   -- id топика/сообщения
	`is_topic`   INTEGER NOT NULL                -- флаг того, что это топик, а не сообщение
);

CREATE TABLE `forum`
(
	--
	-- список форумов
	--
	`id`         INTEGER      NOT NULL PRIMARY KEY,   -- id
	`id_group`   INTEGER      NOT NULL,               -- id группы форумов
	`short_name` VARCHAR(64)  NOT NULL,               -- короткое имя форума
	`name`       VARCHAR(128) NOT NULL,               -- полное имя форума
	`rated`      INTEGER      NOT NULL,               -- оценивается ли форум
	`in_top`     INTEGER      NOT NULL,               -- участвует ли оценки этого форума в топе
	`rate_limit` INTEGER      NOT NULL                -- лимит оценки в форуме
);

CREATE TABLE `group`
(
	--
	-- список групп форумов
	--
	`id`         INTEGER      NOT NULL PRIMARY KEY,   -- id
	`name`       VARCHAR(100) NOT NULL,               -- наименование группы
	`sort_order` INTEGER      NOT NULL                -- порядок сортировки
);

CREATE TABLE `message`
(
	--
	-- сообщения
	--
	`id`               INTEGER      NOT NULL PRIMARY KEY,   -- id сообщения
	`id_topic`         INTEGER      NOT NULL,               -- id темы
	`id_parent`        INTEGER      NOT NULL,               -- id родительского сообщения
	`id_user`          INTEGER      NOT NULL,               -- id автора
	`id_forum`         INTEGER      NOT NULL,               -- id форума
	`subject`          VARCHAR(128) NOT NULL,               -- тема сообщения
	`message_name`     VARCHAR(160) NOT NULL,               -- имя сообщения
	`user_nick`        VARCHAR(100) NOT NULL,               -- имя автора сообщения
	`message`          BLOB         NOT NULL,               -- текст сообщения
	`id_article`       INTEGER      NOT NULL,               -- id статьи, если сообщение является статьей или 0
	`message_date`     INTEGER      NOT NULL,               -- дата создания сообщения
	`update_date`      INTEGER      NOT NULL,               -- дата обновления сообщения или 0
	`user_role`        VARCHAR(50)  NOT NULL,               -- статус автора сообщения
	`user_title`       VARCHAR(100) NOT NULL,               -- повязка пользователя
	`user_title_color` INTEGER      NOT NULL,               -- цвет повязки пользователя
	`last_moderated`   INTEGER      NOT NULL,               -- дата последнего переноса сообщения
	`closed`           INTEGER      NOT NULL,               -- флаг закрытия темы
	`has_child`        INTEGER      NOT NULL,               -- флаг наличия дочерних сообщений
	`compressed`       INTEGER      NOT NULL                -- флаг сжатия тела сообщения
);

CREATE INDEX `message_ix_id_topic`              ON `message` (`id_topic`);
CREATE INDEX `message_ix_id_parent`             ON `message` (`id_parent`);
CREATE INDEX `message_ix_id_user`               ON `message` (`id_user`);
CREATE INDEX `message_ix_id_forum`              ON `message` (`id_forum`);
CREATE INDEX `message_ix_message_date`          ON `message` (`message_date`);
CREATE INDEX `message_ix_id_forum_message_date` ON `message` (`id_forum`, `message_date`);

CREATE TABLE `message2send`
(
	--
	-- сообщения к отправке
	--
	`id`        INTEGER      NOT NULL PRIMARY KEY,   -- id сообщения (локальный)
	`id_parent` INTEGER      NOT NULL,               -- id родителя
	`id_forum`  INTEGER      NOT NULL,               -- id форума
	`subject`   VARCHAR(128) NOT NULL,               -- тема
	`message`   TEXT         NOT NULL,               -- текст сообщения
	`date`      INTEGER      NOT NULL,               -- дата и время занесения
	`draft`     INTEGER      NOT NULL                -- флаг черновика
);

CREATE TABLE `message_topic`
(
	--
	-- список топиков
	--
	`id_message`   INTEGER NOT NULL PRIMARY KEY,   -- id сообщения
	`id_forum`     INTEGER NOT NULL,               -- id форума
	`message_date` INTEGER NOT NULL,               -- дата создания сообщения
	FOREIGN KEY (`id_message`) REFERENCES `message` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX `message_topic_ix_id_forum` ON `message_topic` (`id_forum`);

CREATE TABLE `moderate`
(
	--
	-- таблица бомбочек
	--
	`id_message` INTEGER NOT NULL,   -- id сообщения
	`id_topic`   INTEGER NOT NULL,   -- id топика
	`id_user`    INTEGER NOT NULL,   -- id пользователя
	`id_forum`   INTEGER NOT NULL,   -- id форума
	`created`    INTEGER NOT NULL,   -- дата
	PRIMARY KEY (`id_message`, `id_user`)
);

CREATE TABLE `moderate2send`
(
	--
	-- модерилки к отправке
	--
	`id`           INTEGER     NOT NULL PRIMARY KEY,   -- id (локальный)
	`id_message`   INTEGER     NOT NULL,               -- id сообщения
	`action`       VARCHAR(32) NOT NULL,               -- действие {MoveMessage, DeleteMessage, DeleteThread, DeleteErrorMessage, SplitThread, CloseTopic, OpenTopic}
	`id_forum`     INTEGER     NOT NULL,               -- id форума для перемещения
	`description`  TEXT        NOT NULL,               -- описание
	`as_moderator` INTEGER     NOT NULL,               -- флаг "как модератор"
	`date`         INTEGER     NOT NULL                -- дата добавления
);

CREATE TABLE `rating`
(
	--
	-- таблица рейтинга
	--
	`id_message`  INTEGER NOT NULL,   -- id сообщения
	`id_topic`    INTEGER NOT NULL,   -- id топика
	`id_user`     INTEGER NOT NULL,   -- id пользователя
	`user_rating` INTEGER NOT NULL,   -- рейтинг пользователя
	`rate`        INTEGER NOT NULL,   -- оценка
	`rate_date`   INTEGER NOT NULL,   -- дата
	`rate_type`   INTEGER NOT NULL,   -- тип оценки (0 - смайл, 1 - +/-, 2 - балл)
	PRIMARY KEY (`id_message`, `id_user`, `rate_type`)
);

CREATE TABLE `rating2send`
(
	--
	-- рейтинг к отправке
	--
	`id`         INTEGER NOT NULL PRIMARY KEY,   -- id (локальный)
	`id_message` INTEGER NOT NULL,               -- id сообщения
	`rate`       INTEGER NOT NULL,               -- рейтинг
	`date`       INTEGER NOT NULL                -- дата добавления
);

CREATE TABLE `row_version`
(
	--
	-- версии данных
	--
	`key`   VARCHAR(100) NOT NULL PRIMARY KEY,   -- ключ версии
	`value` VARCHAR(100) NOT NULL                -- значение версии
);

CREATE TABLE `subscribed`
(
	--
	-- список форумов, на которые подписан пользователь
	--
	`id_forum` INTEGER NOT NULL PRIMARY KEY,   -- id форума
	`is_first` INTEGER NOT NULL                -- была ли раньше подписка на форум true - нет, false - да
);

CREATE TABLE `unread`
(
	--
	-- список непрочитанных сообщений
	--
	`id_message`     INTEGER NOT NULL PRIMARY KEY,   -- id непрочитанного сообщения
	`id_parent`      INTEGER NOT NULL,               -- id родительского сообщения
	`id_forum`       INTEGER NOT NULL,               -- id форума
	`id_topic`       INTEGER NOT NULL,               -- id топика
	`id_parent_user` INTEGER NOT NULL,               -- id пользователя, которому идет ответ
	`message_date`   INTEGER NOT NULL                -- дата отправки сообщения
);

CREATE TABLE `user`
(
	--
	-- список пользователей
	--
	`id`             INTEGER      NOT NULL PRIMARY KEY,   -- id пользователя
	`name`           VARCHAR(60)  NOT NULL,               -- логин пользователя
	`nick`           VARCHAR(100) NOT NULL,               -- псевдоним пользователя
	`real_name`      VARCHAR(80)  NOT NULL,               -- реальное имя пользователя
	`email`          VARCHAR(60)  NOT NULL,               -- публичный email пользователя
	`homepage`       VARCHAR(120) NOT NULL,               -- домашняя страница
	`specialization` VARCHAR(100) NOT NULL,               -- специализация
	`where_from`     VARCHAR(100) NOT NULL,               -- откуда
	`origin`         VARCHAR(258) NOT NULL                -- подпись
);

CREATE INDEX `user_ix_name` ON `user` (`name`);

CREATE TABLE `version`
(
	--
	-- версии хранилища (по факту не используется)
	--
	`key`   VARCHAR(100) NOT NULL PRIMARY KEY,   -- ключ версии
	`value` VARCHAR(100) NOT NULL                -- значение версии
);

ANALYZE;

COMMIT;
