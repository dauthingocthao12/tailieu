<?php
//12星座有料アプリ用プログラム

error_reporting(E_ALL ^ E_NOTICE);
// マスター設定
require_once(dirname(__FILE__) . "/../libadmin/config.php");
// 共有ライブラリ
require_once(dirname(__FILE__) . "/../libadmin/common.php");
// ランキング用クラス
require_once(dirname(__FILE__) . "/../libadmin/uranairanking.class.php");
// smarty用
require_once(dirname(__FILE__) . "/../libs/Smarty.class.php");
require_once(dirname(__FILE__) . "/../libadmin/function.sitelink.php");
require_once(dirname(__FILE__) . "/../libadmin/utils.smarty.php");
// smarty 設定　
$smarty = new Smarty;
$smarty->setTemplateDir(dirname(__FILE__) . "/../templates/12star_app"); 
$smarty->setCompileDir(dirname(__FILE__). "/../templates_c/");
//コンフィグ設定
$appType = preg_match("/Azet (?P<OS>.*) (?P<star>.*) App v\.(?P<version>.*)/", $_SERVER['HTTP_USER_AGENT'],$appInfo);
$config = array(
	'date_today' => date('Ymd')
	//,'isIosApp' => preg_match("/Azet iOS App/", $_SERVER['HTTP_USER_AGENT'])
	//,'isAndroidApp' => preg_match("/Azet Android App/", $_SERVER['HTTP_USER_AGENT'])
	,'AppOS'=> $appType ? $appInfo['OS'] : null
	,'AppVersion'=> $appType ? $appInfo['version'] : null
	,'AppStar'=> $appType ? $appInfo['star'] : null
	,'cache_date' => CACHE_DATE
	,'is_server' => IS_SERVER
);
$smarty->assign('config', $config);
//======================================================================
//                                      _                
// _ __   __ _ _ __ __ _ _ __ ___   ___| |_ ___ _ __ ___ 
//| '_ \ / _` | '__/ _` | '_ ` _ \ / _ \ __/ _ \ '__/ __|
//| |_) | (_| | | | (_| | | | | | |  __/ ||  __/ |  \__ \
//| .__/ \__,_|_|  \__,_|_| |_| |_|\___|\__\___|_|  |___/
//|_|                                                    
//======================================================================
//GETからパラメータ設定
//日付の設定
if($_GET['date']){
	$date = $_GET['date'];
}else{
	$date = date('Ymd');
}
//運勢タイプの設定
if($_GET['data_type']){
	$data_type = $_GET['data_type'];
}else{
	$data_type = "";
}
//画面モードの設定
if($_GET['mode']){
	$mode = $_GET['mode'];
}else{
	$mode = "default";
}

if($_GET["debug"] == TRUE){
	$debug_args .= "&debug=true";

	if($_GET['star']){
		$star_num = $_GET['star'];
	}else{
		$star_num = 1;
	}
	$debug_args .= "&star=".$star_num;
	$smarty->assign("debug_args",$debug_args);
}else{
	//モバイルデバイスが確認できたら星座を設定
	if($config['AppOS'] !== null){
		if ($mode == "default") {
			//星座番号の設定
			$star = mb_strtolower($config['AppStar']);
			$star_num = array_search($star, $en_num_star);
			//星座が取得できなければエラー画面へ
			if($star_num === FALSE){
				$mode = "app_error";
			}
		}
		//アプリ以外からのアクセス
	}else{
		//プライバシーモード以外は本サイトへ
		if($mode != "privacy_policy"){
			header('Location:'.BASE_URL);
		}
	}
}
$smarty->assign('mode',$mode);
//======================================================================
//  _             _      
// | | ___   __ _(_) ___ 
// | |/ _ \ / _` | |/ __|
// | | (_) | (_| | | (__ 
// |_|\___/ \__, |_|\___|
//          |___/        
// ======================================================================
//トピック運の日本語名を設定（"恋愛運","仕事運",...）
$topic_jp = $topic_Jp[$data_type];
//共有オブジェクト 
$ranking = new UranaiRankingEx($config['date_today'],$data_type);
//======================================================================
//画面モードごとの分岐設定
//一覧画面
if ($mode == "default") {
	$template_page = "default.tpl";
	$date_print = formatDateJpn($date);
	$plugin_time = $ranking->checkEarliestPluginTime();
	$is_log = $ranking->isTodaysLog( $data_type );
	$plugin_run = false;
	//最も早いプラグインの実行時間を過ぎていたら
	if($plugin_time < date("H:i:s")){
		$plugin_run = true;
	}

	$today_all_ranks = $ranking->getAppDetailsForStarRank($star_num,$data_type);

	if(is_array($today_all_ranks)){
		$ranks = getRanksExist($today_all_ranks);//存在する順位一覧
		$best_rank_sites = $today_all_ranks[$ranks[0]];//最高順位のサイト一覧

		$i = 1; //順位配列のキー

		//最高順位のサイト数が表示最低数に達していない
		while(count( $best_rank_sites ) < MIN_BEST_SITES){
			$next_rank_sites = $today_all_ranks[$ranks[$i]];

			//次の順位のサイト一覧を全て代入
			foreach($next_rank_sites as $ns){
				$best_rank_sites[] = $ns;
			}
			$i++;
			//次の順位が存在しなければ終わり
			if(!array_key_exists($i,$ranks)){
				break;
			}
		}
	}

	$smarty->assign('plugin_run',$plugin_run); //最も早いプラグインが実行されたか
	$smarty->assign('is_log',$is_log); //今日付けのlogレコードが存在するか
	$smarty->assign('star_num',$star_num);
	$smarty->assign('data_start',PREV_DATE);
	$smarty->assign('best_rank', $best_rank);
	$smarty->assign('best_rank_sites', $best_rank_sites);
	$smarty->assign('star_name', $name["star".$star_num]);
	$smarty->assign('previous_link', previous_date($date));
	$smarty->assign('next_link', next_date($date));
	$smarty->assign('date_print',$date_print);
	$smarty->assign('date_num',$date);
	$smarty->assign('star_name_eng',$en_num_star[$star_num]);
	$smarty->assign('data_type',$data_type);
	$smarty->assign('topic_name',$topic_jp);
	//規約画面
} else if($mode == "kiyaku") {
	$smarty->assign('star_name_eng',$en_num_star[$star_num]);
	$template_page = "kiyaku.tpl";
	//プライバシーポリシー
} else if($mode == "privacy_policy"){
	$template_page = "privacy_policy.tpl";
	//星座取得エラー
} else if($mode == "app_error"){
	$template_page = "app_error.tpl";
}
//Display
$smarty->display($template_page);
