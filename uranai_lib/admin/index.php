<?php
error_reporting(E_ALL & ~E_NOTICE);

require_once dirname(__FILE__) . "/../libs/Smarty.class.php" ;
require_once dirname(__FILE__) . "/../libadmin/config.php" ;
require_once dirname(__FILE__) . "/../libadmin/function.makelink.php" ;
require_once dirname(__FILE__) . "/../libadmin/function.xinput.php" ;
require_once dirname(__FILE__) . "/../libadmin/block.xmakelink.php" ;
require_once dirname(__FILE__) . "/../libadmin/common.php" ;
require_once dirname(__FILE__) . "/../libadmin/function.sitelink.php";
require_once dirname(__FILE__) . "/../libadmin/utils.smarty.php";
session_start();

// PAGINATOR MASTER SETTING >>>
require_once dirname(__FILE__) . "/../libadmin/paginator.class.php" ;
Paginator::setPagingFor('admin-comments', 20); // records per page
// <<<

//benchmark for local testing >>>
// add simon 2015-11-30
if(!IS_SERVER) {
	$start_time = microtime(true);
}
// <<<

$smarty = new Smarty;
$smarty->setTemplateDir(dirname(__FILE__) . "/../templates/admin/"); // Smarty はカレントディレクトリしか探さない
$smarty->setCompileDir(dirname(__FILE__). "/../templates_c/");

// main 値
$smarty->assign('TITLE', TITLE);

//======================================================================
# セキュリティ

$is_security = false;
if ($_SESSION["user"]) {
	// default 画面
	$is_security = true;
	$mode = "log";
	$action = "listing";
} else {
	$mode = "login";
	$action = "go";
}

//======================================================================
# パラメータ

if ($_POST["mode"]) {
	$mode = $_POST["mode"];
}

if ($_POST["action"]) {
	$action = $_POST["action"];
}

if ($is_security) {
	$template_page = "{$mode}-{$action}.tpl";
} else {
	$template_page = "login-go.tpl";
}

$id = $_POST["id"];

# 処理
if ($mode == "login") {
	//======================================================================
	// login用のロジック
	//======================================================================
	include(dirname(__FILE__) . "/../libadmin/login.php");
	if ($action == "check") {
		$data = login_check();
		if ($data["status"] == "OK") {
			header("Location: index.php");
		}
	} elseif ($action == "go") {
		$data["message"] = "ユーザ名とパスワードを入力してください。";
		// pass
	}
}
elseif ($mode == "logout") {
	//======================================================================
	// logout用のロジック
	//======================================================================
	include(dirname(__FILE__) . "/../libadmin/logout.php");
	if ($action == "check") {
		// pass
	} elseif ($action == "go") {
		$data = logout_go();
	}
}
elseif ($mode == "site" && $is_security) {
	//======================================================================
	// site用のロジック
	//======================================================================
	include(dirname(__FILE__) . "/../libadmin/site.php");
	require dirname(__FILE__).'/../libadmin/site_details.class.php';

	// 一覧
	if ($action == "listing") {
		// post情報リセット
		$_SESSION['site_post'] = null;
		$data = site_listing();
	}
	// 情報表示
	elseif ($action == "detail") {
		$data = site_edit($id);
	}
	// 登録・変更
	elseif($action == 'input') {
		if($_SESSION['site_post']) {
			$data = array('status' => 'OK', 'db' => $_SESSION['site_post']);
		}
		else {
			$data = site_edit($id);
		}
	}
	// 登録確認画面
	elseif ($action == "check") {
		// $dataにはcheckとdb情報を追加されていた
		// dbはpostの情報出す
		// 登録エラー確認
		$data = site_check($_POST);
		if($data['status']!='OK') {
			// FORM ERROR!
			$template_page = "site-input.tpl";
		}
		else {
			// confirm details
			$template_page = "site-detail.tpl";
		}
		// sessionで保存する
		$_SESSION['site_post'] = $data['db'];
	}
	// 情報を保存する
	elseif ($action == "update") {
		//pre($_SESSION['site_post']);
		$data = site_save($_SESSION['site_post']);
	}
	// サイトを削除
	elseif ($action == "delete") {
		$data = site_delete($id);
	}
	elseif ($action == "delete-do") {
		$data = site_delete_do($id);
	}
	elseif ($action=='batch') {
		// CRONの処理
		$data = batch_run($_POST['id']);
	}
	elseif ($action=='batch-test') { // 未使用？ならば削除しよう！ シモン 2018-10-01
		// CRONの処理
		$data = batch_run_test($_POST['id']);
	}
	elseif ($action=='batch-topic') {
		// CRONの処理
		$data = batch_Topic_run($_POST['id']);
	}
	elseif ($action=='site_desc' && isset($_POST['id'])) {
		// form
		$data = SiteDetails::getById($_POST['id']);
	}
	elseif ($action == "details_update") {
		//pre($_SESSION['site_post']);
		$ok = SiteDetails::save($_POST);
		if(!$ok) {
			$data['message'] =  "保存できませんでした、データを確認してください。";
			$data = $_POST;
		}
		else {
			$data = SiteDetails::getById($_POST['id']);
			$data['message'] = "サイト説明文を保存しました。";
		}
		// form template
		$template_page = "site-site_desc.tpl";
	}
} elseif ($mode == "ad" && $is_security) {
	//======================================================================
	// AD処理
	//======================================================================
	include(dirname(__FILE__) . "/../libadmin/ad.php");
	if ($action == "listing") {
		// post情報リセット
		$_SESSION['ad_post'] = null;
		if($_POST['filter']) {
			$_SESSION['ad_filter'] = $_POST['filter'];
		}
		if(!$_SESSION['ad_filter']) $_SESSION['ad_filter'] = 'active';
		$data = ad_listing($_SESSION['ad_filter']);
	} elseif ($action == "detail") {
		// detail
		$data = ad_edit($id);
	} elseif ($action == "input") {
		// input
		if($_SESSION['ad_post']) {
			$data = array('status' => 'OK', 'db' => $_SESSION['ad_post']);
		}
		else {
			$data = ad_edit($id);
		}
	} elseif ($action == "check") {
		// $dataにはcheckとdb情報を追加されていた
		// dbはpostの情報出す
		// 登録エラ確認
		$data = ad_check($_POST);
		if($data['status']!='OK') {
			// FORM ERROR!
			$template_page = "ad-input.tpl";
		}
		else {
			// confirm details
			$template_page = "ad-detail.tpl";
		}
		// sessionで保存する
		$_SESSION['ad_post'] = $data['db'];
	} elseif ($action == "update") {
		// 更新処理
		$data = ad_save($_SESSION['ad_post']);
	} elseif ($action == "delete") {
		$data = ad_delete($id);
	} elseif ($action == "delete-do") {
		$data = ad_delete_do($id);
	}

	//------------------------------------------------------------
	//広告グループ管理
	//2019/05
	//------------------------------------------------------------
	if ($action == "group-listing") {
		// post情報リセット
		$_SESSION['ad_group_post'] = null;
		$data = ad_group_listing();
		$smarty->assign("ad_files", grep_ad());
	} elseif ($action == "group-detail") {
		// detail
		$data = ad_group_edit($id);
		$data['ad_name_list'] = get_all_ad_names(); //広告マスタ取得
	} elseif ($action == "group-input") {
		// input
		if($_SESSION['ad_group_post']) {
			$data = array('status' => 'OK', 'db' => $_SESSION['ad_group_post']);
		}
		else {
			$data = ad_group_edit($id);
		}
		$data['ad_name_list'] = get_all_ad_names(); //広告マスタ取得
	} elseif ($action == "group-check") {
		$data = ad_group_check($_POST);
		$data['ad_name_list'] = get_ad_names($_POST['ad_ids']);
		if($data['status']!='OK') {
			// FORM ERROR!
			$template_page = "ad-group-input.tpl";
		}
		else {
			// confirm details
			$template_page = "ad-group-detail.tpl";
		}
		// sessionで保存する
		$_SESSION['ad_group_post'] = $data['db'];
	} elseif ($action == "group-update") {
		// 更新処理
		$data = ad_group_save($_SESSION['ad_group_post']);
	} elseif ($action == "group-delete") {
		$data = ad_group_delete($id);
	} elseif ($action == "group-delete-do") {
		$data = ad_group_delete_do($id);
	}

} elseif ($mode == 'log') {
	//======================================================================
	// LOG処理
	//======================================================================
	require dirname(__FILE__).'/../libadmin/log.php';
	if($action=='listing') {
		$data = log_listing($_POST['file'], $_POST['filter']);
	}

} elseif ($mode == 'batch-job-status') {
	//======================================================================
	// 実行履歴処理
	//======================================================================
	require dirname(__FILE__).'/../libadmin/batch_job_status.class.php';
	if($action=='listing') {
		$status = new BatchJobStatus;
		$data = $status->load()->getSiteStatus();

		$smarty->assign('active_plugin_count', $status->getActivePluginCountAll());
		$smarty->assign('active_plugin_count_sougo', $status->getActivePluginCountSougo());
		$smarty->assign('active_plugin_count_topic', $status->getActivePluginCountTopic());
		$smarty->assign('success_count', $status->getSuccessCount());
		$smarty->assign('fail_count', $status->getFailCount());
		$smarty->assign('pending_count', $status->getPendingCount());
	}

} elseif ($mode == 'news') {
	//======================================================================
	// NEWS処理
	//======================================================================
	require dirname(__FILE__).'/../libadmin/news.class.php';

	if($action=='input') {
		// 登録
		if($_POST['id']) {
			// form
			$data = News::getById($_POST['id']);
		}
		else {
			$data['default_release_date'] = date('Y-m-d'); // current date
			$data['default_promote_date'] = date('Y-m-d', strtotime('+7 day', time())); // one week later
		}
	}
	else if($action=='delete' && $_POST['id']) {
		// 削除 確認
		$data = News::getById($_POST['id']);
		$data['news_content_fetch'] = $smarty->fetch('string:'.$data['news_content']);
	}
	else if($action=='delete-do' && $_POST['id']) {
		$data = News::delete($_POST['id']);
	}
	else if($action=='update') {
		// 更新
		$ok = News::save($_POST);
		if(!$ok) {
			$data = $_POST;
			$data['message'] =  "保存できませんでした、データを確認してください。";
			// form template
			$template_page = "news-input.tpl";
		}
		else {
			$data['message'] = "新着が保存されました。";
			$smarty->assign('message', $message);
		}
	}
	else {
		// 一覧
		$data = News::getListAdmin(1, -1);
	}
}
elseif($mode=='user') {
	//======================================================================
	// ユーザ処理
	//======================================================================
	require dirname(__FILE__).'/../libadmin/user.php';
	if($action=='listing') {
		$data = user_listing($_POST['user_sort_column'], $_POST['user_order']);
	}
	if($action=='detail') {
		$data = user_detail($id);
	}
	if($action=='delete') {
		$data = user_delete($id);
	}
	if($action=='delete-do') {
		$data = user_delete_do($id);
	}
}
elseif($mode == 'comment'){
	// ======================================================================
	// コメント処理
	// ======================================================================
	require_once dirname(__FILE__).'/../libadmin/mail.class.php';
	require_once dirname(__FILE__).'/../libadmin/site_comment.class.php';
	require_once dirname(__FILE__).'/../libadmin/site_comment_report.class.php';
	require_once dirname(__FILE__).'/../libadmin/admin_comment.ctrl.php';
	// print_r($_POST);

	// set message templates data
	$smarty->assign('admin_comment_msg_templates', SiteComment::$ADMIN_TEMPLATES);

	// 一覧画面からコメントを公開
	if($action == 'publish') {
		$status = comment_publish($id);
		if($status) {
			$message = array(
				'status' => 'success',
				'content' => '保存できました'
			);
		}
		else {
			$message = array(
				'status' => 'danger',
				'content' => '保存できませんでした'
			);
		}
		$smarty->assign('message', $message);

		// 一覧表示
		$action = 'listing';
		$template_page = 'comment-listing.tpl';
	}

	// コメント一覧
	if($action == 'listing') {
		$data = comments_listing();
	}

	// コメントの違反報告
	if($action == 'report_listing') {
		$data = SiteCommentReport::getReportsForComment($id);
	}
	if($action == 'report_read') {
		SiteCommentReport::readReport($id);
	}

	// 管理のアクションを保存
	if($action == 'save') {
		$status = siteComment::adminSave($_POST);
		if($status) {
			$message = array(
				'status' => 'success',
				'content' => '保存できました'
			);
		}
		else {
			$message = array(
				'status' => 'danger',
				'content' => '保存できませんでした'
			);
		}
		$smarty->assign('message', $message);

		// display form
		$action = 'input';
		$template_page = 'comment-input.tpl';
	}

	// コメント変更
	if($action == 'input') {
		$data = comment_input($id);
	}

	// debug($data);
}
elseif($mode == 'site_check'){
	// ======================================================================
	// コメント処理
	// ======================================================================
	require_once dirname(__FILE__).'/../libadmin/check_rank_data.php';
	$data = site_check();
}
// elseif($mode == 'analysis'){ }
elseif($mode == 'analysis'){

	require_once dirname(__FILE__).'/../libadmin/analysis.class.php';
	$week = array('日', '月', '火', '水', '木', '金', '土');

	$smarty->assign("analysis_data_jpn", $analysis_data_jpn);

	$analysis_date1_start = date("Y-m-d", strtotime('monday 2 weeks ago'));
	$analysis_date1_end = date("Y-m-d", strtotime('sunday 2 weeks ago'));

	$analysis_date2_start = date("Y-m-d", strtotime('monday last week'));
	$analysis_date2_end = date("Y-m-d", strtotime('sunday last week'));

	if($_POST['analysis_date1_start']){ $analysis_date1_start = date("Y-m-d", strtotime($_POST['analysis_date1_start'])); }
	if($_POST['analysis_date1_end']){ $analysis_date1_end = date("Y-m-d", strtotime($_POST['analysis_date1_end'])); }

	if($_POST['analysis_date2_start']){ $analysis_date2_start = date("Y-m-d", strtotime($_POST['analysis_date2_start'])); }
	if($_POST['analysis_date2_end']){ $analysis_date2_end = date("Y-m-d", strtotime($_POST['analysis_date2_end'])); }

	$smarty->assign("date1_start", $analysis_date1_start);
	$smarty->assign("date1_end", $analysis_date1_end);
	$smarty->assign("date2_start", $analysis_date2_start);
	$smarty->assign("date2_end", $analysis_date2_end);

	$date_set = [
		[$analysis_date1_start, $analysis_date1_end],
		[$analysis_date2_start, $analysis_date2_end],
	];

	foreach ($date_set as $key => $dates) {

		$analysis = new Analysis($dates[0], $dates[1]);

		//メトリクスとディメンション一覧 ===> https://developers.google.com/analytics/devguides/reporting/core/dimsmets#cats=session,user
		//フィルターとか ===> https://developers.google.com/analytics/devguides/reporting/core/v3/reference?hl=ja

		$get_by_date_option = array( 
			"dimensions" => 'ga:date', //ディメンション：区切り 日付ごとに
			"sort" => "ga:date" //ソート 日付順
		);
		//日別セッション
		// $general_info_by_dates = $analysis->get_ga_data("ga:sessions, ga:users, ga:newUsers", $get_by_date_option);
		$general_info_by_dates = $analysis->get_ga_data("ga:sessions", $get_by_date_option); //userを日別にとって合算すると重複をとってしまう
		//ユーザーは別に取得
		$ga_user_info = $analysis->get_ga_data("ga:users, ga:newUsers")[0];
		//PV上位ページ
		$ga_pv_info = $analysis->get_ga_data("ga:pageviews", ['dimensions' => 'ga:pagePath', "sort" => "-ga:pageviews" , "max-results" => 5]);

		//合計セッション
		$ga_session = array_reduce($general_info_by_dates, function($sum, $v) {
			$sum += $v[1]; //1:session
			return $sum;
		}, 0);
		//期間最高セッション
		$ga_max_session_info = array_reduce($general_info_by_dates, function($acc, $v) {
			if ($acc['max'] < $v[1]) {
				$acc = [
					'max' => $v[1],
					'date' => $v[0]
				];
			}
			return $acc;
		}, ['max' => 0, 'date' => 0]);

		$ga_max_session = $ga_max_session_info['max'];
		$ga_max_session_w = $week[date("w", strtotime($ga_max_session_info['date']))];
		$ga_weekday_session = $analysis->get_weekday_session($general_info_by_dates);

		$ga_user = $ga_user_info[0];
		$ga_new_user = $ga_user_info[1];
		$ga_existing_user = $ga_user - $ga_new_user;

		$res_twitter = $analysis->get_ga_data("ga:users", ["dimensions" => "ga:source", "filters" => "ga:source=~^t.co"]);
		$res_fb = $analysis->get_ga_data("ga:users", ["dimensions" => "ga:source", "filters" => "ga:source=~.*facebook"]);
		$res_mstdn = $analysis->get_ga_data("ga:users", ["dimensions" => "ga:source", "filters" => "ga:source=~pawoo.net"]);

		$tw_users = $analysis::sum_referrer_result($res_twitter);
		$fb_users = $analysis::sum_referrer_result($res_fb);
		$mstdn_users = $analysis::sum_referrer_result($res_mstdn);

		$data[$key] = [
			 "date_start" => $dates[0]
			,"date_end" => $dates[1]
			,"ga_session" => $ga_session
			,"ga_weekday_session" => $ga_weekday_session
			,"ga_week_avg_session" => floor($ga_session / 7)
			,"ga_weekday_avg_session" => floor($ga_weekday_session / 7)
			,"ga_max_session" => $ga_max_session
			,"ga_max_session_date" => date("Y-m-d", strtotime($ga_max_session_info['date']))
			,"ga_max_session_day" => $ga_max_session_w
			,"ga_new_user_percentage" => round(($ga_new_user / $ga_user) * 100, 2)
			,"ga_existing_user_percentage" => round(($ga_existing_user / $ga_user) * 100, 2)
			,"ga_new_user" => $ga_new_user
			,"ga_existing_user" => $ga_existing_user
			,"ga_user" => $ga_user
			,"registed_user" => $analysis->registed_user()
			,"deleted_user" => $analysis->deleted_user()
			,"all_user" => $analysis->all_user()
			,"plugins" => $analysis->plugins()
			,"ga_tw_users" => $tw_users
			,"ga_fb_users" => $fb_users
			,"ga_mstdn_users" => $mstdn_users
			,"ga_pv_top1" => $ga_pv_info[0][0]
			,"ga_pv_top1_value" => $ga_pv_info[0][1]
			,"ga_pv_top2" => $ga_pv_info[1][0]
			,"ga_pv_top2_value" => $ga_pv_info[1][1]
			,"ga_pv_top3" => $ga_pv_info[2][0]
			,"ga_pv_top3_value" => $ga_pv_info[2][1]
			,"ga_pv_top4" => $ga_pv_info[3][0]
			,"ga_pv_top4_value" => $ga_pv_info[3][1]
			,"ga_pv_top5" => $ga_pv_info[4][0]
			,"ga_pv_top5_value" => $ga_pv_info[4][1]
		];

	}
	
	if($action == "export"){

    header("Content-Type: application/octet-stream");
    header("Content-Disposition: attachment;filename=".$analysis_date_start."_to_".$analysis_date_end.".csv");
    header("Content-Transfer-Encoding: binary");

		$csv = "";
		foreach($data as $key => $val){
			$csv.= $analysis_data_jpn[$key].",";
		}
		$csv = rtrim($csv, ",");
		$csv.= "\n";

		foreach($data as $key => $val){
			$csv.= $val.",";
		}
		$csv = rtrim($csv, ",");

		echo $csv;
		exit;
	}
}
elseif($mode == 'send_mail'){
	// ======================================================================
	// コメント処理
	// ======================================================================
	require_once dirname(__FILE__).'/../libadmin/mail.class.php';
	require_once dirname(__FILE__).'/../libadmin/send_mail_questionnaire.php';

	// 一覧画面からコメントを公開
	if($action == 'send') {
		$data = send_mail_questionnaire();
	}

	// コメント一覧
	if($action == 'listing') {
		$data = mail_kakunin();
	}
	
	if($action == 'send_result'){
		$data = mail_result();
	}
	$template_page = 'site_check-listing.tpl';

}

else if($mode == "sougolink"){
	require_once dirname(__FILE__).'/../libadmin/sougoulink-listing_action.php';
	$template_page = "sougolink-listing.tpl";
	if($action == "confirm"){
		$confirmed_url = $_POST["sougo_url"];
		$site_name_kana = $_POST["site_name_kana"];
		$id = $_POST["confirm_id"];
		confirmed($id, $confirmed_url, $site_name_kana);
	}else if($action == "delete"){
		$id = $_POST["delete_id"];
		deleted($id);
	}else if($action = "update"){
		$id = $_POST["update_id"];
		$name = $_POST["update_name"];
		$name_kana = $_POST["update_site_name_kana"];
		$url = $_POST["update_their_url"];
		$mail = $_POST["update_email"];
		update($id, $name, $name_kana, $url, $mail);
	}
	$data = getNominatedSites();
	$sougo_confirmed_list = getConfirmedSites();
}
if (DEBUG) {
	echo "<pre>SESSION\n", print_r($_SESSION);
	echo "TEMPLATE\n$template_page\n";
	echo "DATA\n", print_r($data);
	echo "POST\n", print_r($_POST);
	echo "</pre>";
}

//debug用
if(!IS_SERVER) {
	pre("Mode: $mode");
	pre("Action: $action");
}

# 表示
$smarty->assign("sougo_confirmed_list", $sougo_confirmed_list);
$smarty->assign("data", $data);
$smarty->display($template_page);

//benchmark for local testing >>>
// add simon 2015-11-30
if(!IS_SERVER) {
	$end_time = microtime(true);
	$spent_time = $end_time - $start_time;
	print "exec time: $spent_time sec";
}

// debug data display
if(isset($GLOBALS['debug'])) {
	foreach($GLOBALS['debug'] as $msg) {
		print "<div class='alert alert-debug'>";
		print $msg;
		print "</div>";
	}
}
// <<<
