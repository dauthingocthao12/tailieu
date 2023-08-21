-- ユーザアカウントのテーブル：
-- ==================

CREATE TABLE `users` (
  `user_id` int(10) unsigned NOT NULL,
  `email` varchar(200) NOT NULL,
  `password` varchar(45) NOT NULL,
  `handlename` varchar(45) DEFAULT NULL,
  `gender` varchar(10) DEFAULT NULL,
  `birthday` date DEFAULT NULL,
  `notification1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notification2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notification3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notification4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notification5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notification6` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notification0` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notificationHour` varchar(5) NOT NULL DEFAULT '01:00',
  `notificationHolidays` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `notificationStar` tinyint(3) unsigned DEFAULT NULL COMMENT '未使用',
  `date_create` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_update` timestamp NULL DEFAULT NULL,
  `date_delete` timestamp NULL DEFAULT NULL,
  `ip` varchar(45) NOT NULL,
  `user_agent` varchar(255) NOT NULL,
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `email_UNIQUE` (`email`)	★取り消し★
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `users`
ADD COLUMN `birthdayStar` INT UNSIGNED NOT NULL DEFAULT 0 AFTER `birthday`;

ALTER TABLE `users`
ADD COLUMN `who_create` VARCHAR(45) NULL AFTER `date_create`,
ADD COLUMN `who_update` VARCHAR(45) NULL AFTER `date_update`,
ADD COLUMN `who_delete` VARCHAR(45) NULL AFTER `date_delete`,
ADD COLUMN `is_delete` TINYINT UNSIGNED NOT NULL DEFAULT 0 AFTER `who_delete`;

-- add okabe 2016/06/07
ALTER TABLE  `users` ADD  `activationKey` VARCHAR( 32 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL AFTER  `user_agent`
ALTER TABLE  `users` ADD  `activationDate`  `activationDate` TIMESTAMP NULL DEFAULT NULL AFTER  `activationKey`

-- add okabe 2016/06/21
ALTER TABLE  `users` ADD  `notificationSw` TINYINT( 3 ) NOT NULL DEFAULT  '0' AFTER  `notification0`


-- -----------------------------------------------------------------------
-- -------------------- 2016/06/24時点の 開発DB --------------------------

--
-- テーブルの構造 `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(10) unsigned NOT NULL auto_increment,
  `email` varchar(200) NOT NULL,
  `password` varchar(45) NOT NULL,
  `handlename` varchar(45) default NULL,
  `gender` varchar(10) default NULL,
  `birthday` date default NULL,
  `birthdayStar` int(10) unsigned NOT NULL default '0',
  `notification1` tinyint(3) unsigned NOT NULL default '0',
  `notification2` tinyint(3) unsigned NOT NULL default '0',
  `notification3` tinyint(3) unsigned NOT NULL default '0',
  `notification4` tinyint(3) unsigned NOT NULL default '0',
  `notification5` tinyint(3) unsigned NOT NULL default '0',
  `notification6` tinyint(3) unsigned NOT NULL default '0',
  `notification0` tinyint(3) unsigned NOT NULL default '0',
  `notificationSw` tinyint(3) NOT NULL default '0',
  `notificationHour` varchar(5) default NULL,
  `notificationHolidays` tinyint(3) unsigned NOT NULL default '0',
  `notificationStar` tinyint(3) unsigned default NULL COMMENT '未使用',
  `date_create` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `who_create` varchar(45) default NULL,
  `date_update` timestamp NULL default NULL,
  `who_update` varchar(45) default NULL,
  `date_delete` timestamp NULL default NULL,
  `who_delete` varchar(45) default NULL,
  `is_delete` tinyint(3) unsigned NOT NULL default '0',
  `ip` varchar(45) NOT NULL,
  `user_agent` varchar(255) NOT NULL,
  `activationKey` varchar(32) NOT NULL,
  `activationDate` timestamp NULL default NULL,
  PRIMARY KEY  (`user_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=48 ;


--
-- テーブルの構造 `temp_ansmail`
--

CREATE TABLE IF NOT EXISTS `temp_ansmail` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `md5id` varchar(36) NOT NULL,
  `mail` varchar(200) default NULL,
  `date_create` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `date_expire` datetime default NULL,
  PRIMARY KEY  (`id`),
  KEY `md5id` (`md5id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='空メールの送信元メールアドレスの一時保管用' AUTO_INCREMENT=59 ;


-- ---------------------------------------
-- 都道府県設定 を user テーブルに追加する
ALTER TABLE  `users` ADD  `prefecture` TINYINT( 3 ) NOT NULL DEFAULT  '0' AFTER  `birthday`


-- --------------------------------------
-- サイトコメント＋評価についた
-- --------------------------------------
-- 2018-04-02

CREATE TABLE `site_details` (
	`site_id` INT(10) UNSIGNED NOT NULL,
	`description` TEXT NOT NULL,
	`presentation` TEXT NOT NULL,
	`visible` TINYINT(4) NOT NULL DEFAULT '1',
	`date_update` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	PRIMARY KEY (`site_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;

CREATE TABLE `site_comment` (
	`site_comment_id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
	`site_id` INT NOT NULL DEFAULT '0',
	`user_id` INT NOT NULL DEFAULT '0',
	`evaluation` TINYINT NOT NULL DEFAULT '0',
	`comment` TEXT NOT NULL,
	`status` VARCHAR(50) NOT NULL DEFAULT '0',
	`is_delete` TINYINT NOT NULL DEFAULT '0',
	`date_create` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`date_delete` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`site_comment_id`),
	INDEX `site_id_user_id` (`site_id`, `user_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;

-- 2018-04-06
ALTER TABLE `site_comment`
	ADD COLUMN `parent_revision` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `user_id`;

-- 2018-04-11
-- サイトコメントに更新日追加
ALTER TABLE `site_comment`
	ADD COLUMN `date_update` DATETIME NULL DEFAULT NULL AFTER `date_create`;


CREATE TABLE `site_comment_favorite` (
	`favorite_id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`user_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`comment_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`is_delete` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`date_create` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`date_delete` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`favorite_id`),
	INDEX `comment_user` (`comment_id`, `user_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;

-- 2018-04-12
-- サイトのコメントの管理データを追加
ALTER TABLE `site_comment`
  CHANGE COLUMN `status` `status` VARCHAR(50) NOT NULL DEFAULT '0' AFTER `comment`,
  ADD COLUMN `admin_memo` TEXT NULL DEFAULT NULL AFTER `status`,
  ADD COLUMN `admin_date` DATETIME NULL DEFAULT NULL AFTER `admin_memo`;

CREATE TABLE `site_comment_admin` (
  `admin_id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  `comment_id` INT(10) UNSIGNED NOT NULL,
  `action` VARCHAR(100) NOT NULL,
  `comment_content` TEXT NOT NULL,
  `mail_sent` TINYINT(4) NOT NULL DEFAULT '0',
  `mail_content` TEXT NULL,
  `date_create` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`admin_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;


-- 2018-04-13
-- コメント管理テーブルにインデクスを追加
ALTER TABLE `site_comment_admin`
  ADD INDEX `comment_id` (`comment_id`);

-- 2018-05-07
-- ユーザのコメント機能の受信設定
ALTER TABLE `users`
  ADD COLUMN `notificationCommentPublished` TINYINT(1) UNSIGNED NOT NULL DEFAULT '0' AFTER `notificationStar`,
  ADD COLUMN `notificationCommentRejected` TINYINT(1) UNSIGNED NOT NULL DEFAULT '0' AFTER `notificationCommentPublished`;

-- 2018-05-08
-- コメントの違反報告テーブル
CREATE TABLE `site_comment_report` (
	`comment_report_id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
	`comment_id` INT UNSIGNED NOT NULL DEFAULT '0',
	`violation_category` INT UNSIGNED NOT NULL DEFAULT '0',
	`violation_comment` TEXT NOT NULL DEFAULT '',
	`status` VARCHAR(50) NOT NULL DEFAULT '',
	`date_create` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`date_update` DATETIME NULL DEFAULT NULL,
	PRIMARY KEY (`comment_report_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;

-- 2018-05-09
ALTER TABLE `site_comment_report`
	ADD INDEX `comment_id` (`comment_id`, `status`);

-- 2018-05-10
ALTER TABLE `site_comment_report`
	ADD COLUMN `reporter_name` VARCHAR(50) NULL DEFAULT NULL AFTER `status`,
	ADD COLUMN `reporter_company` VARCHAR(50) NULL DEFAULT NULL AFTER `reporter_name`,
	ADD COLUMN `reporter_email` VARCHAR(100) NOT NULL DEFAULT '' AFTER `reporter_company`;

-- 2018-05-17
-- user avatar
ALTER TABLE `users`
  ADD COLUMN `avatar` VARCHAR(100) NOT NULL DEFAULT '' AFTER `handlename`;

-- 2018-10-01
-- change index on log table (add site_id)
ALTER TABLE `log`
 DROP INDEX `day`,
 ADD INDEX `day` (`is_delete`, `day`, `site_id`);

-- change index on topic_log table (add site_id)
ALTER TABLE `topic_log`
 DROP INDEX `day`,
 ADD INDEX `day` (`is_delete`, `day`, `site_id`);

-- 2019-05-14
-- ad_group
CREATE TABLE `ad_group` (
  `ad_group_id` int(10) NOT NULL,
  `ad_group_name` text,
  `date_create` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_update` datetime DEFAULT NULL,
  `date_delete` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='広告グループ';

-- Indexes for table `ad_group`
ALTER TABLE `ad_group` ADD PRIMARY KEY (`ad_group_id`);

-- AUTO_INCREMENT for table `ad_group`
ALTER TABLE `ad_group` MODIFY `ad_group_id` int(10) NOT NULL AUTO_INCREMENT;

-- ad_group_ad
CREATE TABLE `ad_group_ad` (
  `ad_group_id` int(10) NOT NULL,
  `ad_id` int(10) NOT NULL,
  `date_create` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_update` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Indexes for table `ad_group_ad`
ALTER TABLE `ad_group_ad` ADD KEY `ad_group_ad_index` (`ad_group_id`,`ad_id`);

-- 2021-02-16
-- Admin flag for user talbe
ALTER TABLE `users`
  ADD `is_admin` TINYINT(4) NOT NULL DEFAULT '0' COMMENT '管理者フラグ' AFTER `activationDate`;

-- 2021-07-20 
-- create table 相互リンク(全てのサイト)
CREATE TABLE `sougo_sites_nominated` (
  `id` int(11) NOT NULL PRIMARY AUTO_INCREMENT ,
  `site_name` varchar(255) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `confirmed` tinyint(4) DEFAULT '0',
  `management_number` int(11) DEFAULT '0',
  `is_denied` tinyint(4) NOT NULL
) ENGINE=InnoDB AUTO_INCREMENT=100001 DEFAULT CHARSET=utf8mb4;

-- create table 相互リンクを承認されるサイト
CREATE TABLE `sougo_sites` (
  `id` int(11) NOT NULL PRIMARY AUTO_INCREMENT,
  `site_name` varchar(255) DEFAULT NULL,
  `their_link` varchar(255) NOT NULL,
  `us_link` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `is_delete` tinyint(4) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- sougo_sitesにフリガナのカラム追加
ALTER TABLE `sougo_sites` ADD `site_name_kana` VARCHAR(45) NOT NULL COMMENT 'サイト名フリガナ' AFTER `site_name`;

-- カスタム文言
CREATE TABLE `custom_message` (
	`id` INT(11) NOT NULL AUTO_INCREMENT,
	`message_id` VARCHAR(255) NULL DEFAULT NULL COLLATE 'utf8_general_ci',
	`content` TEXT NULL DEFAULT NULL COLLATE 'utf8mb4_unicode_ci',
	PRIMARY KEY (`id`) USING BTREE,
	INDEX `message_id` (`message_id`) USING BTREE
)
COLLATE='utf8mb4_unicode_ci'
;

