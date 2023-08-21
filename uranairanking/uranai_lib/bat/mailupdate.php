#!/usr/local/bin/php
<?php

include(dirname(__FILE__).'/../libadmin/config.php');
include(dirname(__FILE__).'/cron.tools.php');
include(dirname(__FILE__).'/../libadmin/snsapi.class.php');
include(dirname(__FILE__).'/../libadmin/snsapi-mailupdate.class.php');

//======================================================================
// ___ _   _ ___ _____ 
//|_ _| \ | |_ _|_   _|
// | ||  \| || |  | |  
// | || |\  || |  | |  
//|___|_| \_|___| |_|  
//                     
//======================================================================

mysqli_query($conn, "set names 'utf8'");

$log = new Log();
$log->start();
MailUpdateAPI::setLogObject($log);

//======================================================================
//  ____ ____   ___  _   _ 
// / ___|  _ \ / _ \| \ | |
//| |   | |_) | | | |  \| |
//| |___|  _ <| |_| | |\  |
// \____|_| \_\\___/|_| \_|
//                         
// ____   _    ____      _    __  __ _____ _____ _____ ____  ____  
//|  _ \ / \  |  _ \    / \  |  \/  | ____|_   _| ____|  _ \/ ___| 
//| |_) / _ \ | |_) |  / _ \ | |\/| |  _|   | | |  _| | |_) \___ \ 
//|  __/ ___ \|  _ <  / ___ \| |  | | |___  | | | |___|  _ < ___) |
//|_| /_/   \_\_| \_\/_/   \_\_|  |_|_____| |_| |_____|_| \_\____/ 
//                                                                 
//======================================================================

// テストの為に、パラメターがあれば、プラグインを使用する
$param_name = "";
$params = array(
	'test' => false,
	'now' => false,
	'user' => 0
);
foreach($argv as $entry_) {

	// parameter names
	if($entry_ === '--user') {
		$param_name = $entry_;
		continue;
	}

	// testモード
	if($entry_ === '--test') {
		$params['test'] = true;
		$log->add('INIT', "テストモード");
		continue;
	}

	// 今すぐモード
	if($entry_ === '--now') {
		$params['now'] = true;
		$log->add('INIT', "今すぐモード");
		continue;
	}

	// 上はキー名、下は値です
	// ======================

	// パラメター値
	if($param_name=='--user') {
		$params['user'] = $entry_;
		$log->add('INIT', "ユーザID $entry_ 使用");
		continue;
	}
}

//======================================================================
// _     ___   ____ ___ ____ 
//| |   / _ \ / ___|_ _/ ___|
//| |  | | | | |  _ | | |    
//| |__| |_| | |_| || | |___ 
//|_____\___/ \____|___\____|
//======================================================================

$mailUpdate = new MailUpdateAPI();
$mailUpdate->test_mode = $params['test'];
$mailUpdate->test_user = $params['user'];
$mailUpdate->force_now = $params['now'];

$ok = $mailUpdate->publish('');

// === THE END ===
mysqli_close($conn);
$log->stop();
