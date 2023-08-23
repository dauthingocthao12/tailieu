<?php
error_reporting(E_ALL & ~E_NOTICE);

require_once dirname(__FILE__)."/../../uranai_lib/libs/Smarty.class.php" ;
require_once dirname(__FILE__)."/../../uranai_lib/libadmin/config.php" ;
require_once dirname(__FILE__)."/../../uranai_lib/libadmin/common.php" ;

// facebook api
require_once dirname(__FILE__).'/../../uranai_lib/bat/cron.tools.php';
require_once dirname(__FILE__).'/../../uranai_lib/libadmin/snsapi.class.php';
require_once dirname(__FILE__).'/../../uranai_lib/libadmin/snsapi-facebook.class.php';
require_once dirname(__FILE__).'/../../uranai_lib/libadmin/uranairanking.class.php';

//session_start();

$smarty = new Smarty;
$smarty->setTemplateDir(dirname(__FILE__) . "/../../uranai_lib/templates/admin-fb/");
$smarty->setCompileDir(dirname(__FILE__). "/../../uranai_lib/templates_c/");

// LOGIN >>>
if($_SERVER['PHP_AUTH_USER']!='azet00001' || $_SERVER['PHP_AUTH_PW']!='azet0711') {
	// LOGIN!
	header('WWW-Authenticate: Basic realm="uranairanking.jp"');
	header('HTTP/1.0 401 Unauthorized');

	die('Please provide correct login/password.');
}
// <<<


if($_GET['action']=='logout') {
	// LOGOUT!
	header('HTTP/1.1 401 Unauthorized');
	$_SERVER['PHP_AUTH_USER'] = '';
	$_SERVER['PHP_AUTH_PW'] = '';
	$smarty->display('logout.tpl');
	exit;
}
else if($_GET['action']=='post-facebook') {
	// posting page


	date_default_timezone_set('Asia/Tokyo');

	mysqli_query($conn, "set names 'utf8'");

	$today_arr = UranaiPlugin::getToday();
	$today = $today_arr['month'].'月'.$today_arr['day'].'日';
	$now = date("H:i");
	if(date("H") == "00"){
		$type =array( 'en' => '', 'jp' => '総合運');
	}else{
		$type = UranaiRankingEx::randomDataType();
	}

	$ranking = new UranaiRankingEX($today_arr['year'].'-'.$today_arr['month'].'-'.$today_arr['day'],$type['en']);
	$ranking_arr = $ranking->getRanks();

	// SNSプラグイン用の共有メッセージ
	$rand_rank = rand(0, 11);
	$random_ranking = "ちなみに「{$ranking_arr[$rand_rank]['name']}」の「{$type['jp']}」は{$ranking_arr[$rand_rank]['num']}位。";
	$msg_base = "{$today}の12星座占いランキングが{$now}に更新されました！";
	$facebookmsg = "{$msg_base}\n{$random_ranking}";
	$smarty->assign("facebookmsg", $facebookmsg);

	$smarty->display('post.tpl');
	exit;
}
else if($_GET['action'] == 'post-facebook-go') {

	$facebookmsg = $_POST['facebookmsg'];

	$log = new Log();
	$log->start();
	FacebookAPI::setLogObject($log);
	$facebook = new FacebookAPI();
	$ok = $facebook->publish($facebookmsg);

	$smarty->assign('success', $ok);
	$smarty->display('post-done.tpl');

} else {
	// default page
	$smarty->display('index.tpl');
	exit;
}
