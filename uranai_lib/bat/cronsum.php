#!/usr/local/bin/php -q
<?php
error_reporting(E_ALL ^ E_NOTICE);

// マスター設定
require_once(dirname(__FILE__) . "/../libadmin/config.php");
require_once(dirname(__FILE__).'/cron.tools.php');
require_once(dirname(__FILE__) . "/../libadmin/uranairanking.class.php");
// ======================================================================
// logs objects
$log = new Log();
$log->start();
UranaiRanking::setLogObject($log);
// ======================================================================
$dt = date('Y-m-d');
$dt_unix = strtotime($dt);
echo date('Y-m-d H:i:s')." start:".DB_USER."\n";
//=========
//週間
//=========
$yb = date('w'); //曜日判定用 (0:日,1:月,...,6:土)
if ($yb == 1) { //月曜日であること
	UranaiRanking::compileTopicLogs($dt,'weekly');
	UranaiRanking::compileLogs($dt,'weekly');
}
//=========
//月間
//=========
$tdy = date('d'); //日付,日のみ
if ($tdy == 1) { //月の最初の日であること
	$last_month = date('Y-m-d',strtotime('first day of last month'));//先月の一日
	UranaiRanking::compileTopicLogs($last_month,'monthly');
	UranaiRanking::compileLogs($last_month,'monthly');
}
//=========
//年間 
//=========
$tdy = date('m-d'); //何月-何日
if ($tdy == "01-01") {//1月1日であること
	$last_year = date('Y-01-01',strtotime('last year',$dt_unix));//前年度
	UranaiRanking::compileTopicLogs($last_year,'yearly');
	UranaiRanking::compileLogs($last_year,'yearly');
}
//mysql切断
mysqli_close($conn);
echo date('Y-m-d H:i:s')." finished\n";
