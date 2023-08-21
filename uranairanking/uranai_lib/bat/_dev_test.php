<?php
error_reporting(E_ALL);

// 必要なライブラリー
include(dirname(__FILE__).'/../libadmin/config.php');
include(dirname(__FILE__).'/cron.tools.php');

# このプログラムは CRON が実行することを前提としている。

date_default_timezone_set('Asia/Tokyo');
set_time_limit(0);

// logs (PluginUranaiオブジェクトに必要)
$log = new Log();
$log->start();

// 新規プラグインクラス
UranaiPlugin::setLogObject($log);
UranaiPlugin::setConnObject($conn);

// TESTING NEW GLOBAL CHILD PLUGIN!
include('plugins/000101_t.php');

$plugin = new Zodiac000101(101, 8); // site_id, parent_id

// running plugins
$data = $plugin->run(null);
print_r($data);

print PHP_EOL;
print "---";
print PHP_EOL;

$data_topic = $plugin->topic_run(null);
print_r($data_topic);

print PHP_EOL;
print "END";

$log->stop();