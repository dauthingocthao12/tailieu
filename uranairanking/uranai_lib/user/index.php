<?php
error_reporting(E_ALL ^ E_NOTICE);
session_start();

// マスター設定
require_once(dirname(__FILE__) . "/../libadmin/config.php");
// 共有リブ
require_once(dirname(__FILE__) . "/../libadmin/common.php");
// ランキング用
require_once(dirname(__FILE__) . "/../libadmin/uranairanking.class.php");
require_once(dirname(__FILE__) . "/../libadmin/totalscore.class.php");//月間・年間 集計しないバージョン
// 広告用
require_once(dirname(__FILE__) . "/../libadmin/ad.php");
// アカウント用
require_once(dirname(__FILE__) . "/../libadmin/mail.class.php");
require_once(dirname(__FILE__) . "/../libadmin/account.class.php");
// smarty用
require_once(dirname(__FILE__) . "/../libs/Smarty.class.php");
require_once(dirname(__FILE__) . "/../libadmin/function.sitelink.php");
require_once(dirname(__FILE__) . "/../libadmin/insert.affiliate.php");
require_once(dirname(__FILE__) . "/../libadmin/utils.smarty.php");
require_once(dirname(__FILE__) . "/../libadmin/news.class.php");
require_once(dirname(__FILE__) . "/../libadmin/calendar.class.php");//add hirose 2017/03/21
require_once(dirname(__FILE__) . "/../libadmin/user_agent_browser.php");//add hirose 2017/06/12

require_once(dirname(__FILE__) . "/../libadmin/event.php");//add hirose 2017/06/22
require_once(dirname(__FILE__) . "/../libadmin/suggestion.class.php");

// サイトコメントライブラリー
require_once(dirname(__FILE__) . "/../libadmin/site_comment.class.php");
require_once(dirname(__FILE__) . "/../libadmin/history.class.php");

// PAGINATOR MASTER SETTING >>>
require_once dirname(__FILE__) . "/../libadmin/paginator.class.php" ;
Paginator::setPagingFor('site-comments', 10); // records per page

require_once dirname(__FILE__) . "/../libadmin/custom_message.class.php" ;
// <<<

//benchmark for local testing >>>
// add simon 2015-11-30
if(!IS_SERVER) {
	$start_time = microtime(true);
}
// <<<
//パンくずリスト用 2017/3/8 yamaguchi//
require_once(dirname(__FILE__) . "/../libadmin/breadcrumb.class.php");


// 期間イベント用
require_once(dirname(__FILE__) . "/../libadmin/SeasonEvent.php");


// カスタームヘッダー追加 >>>
header("X-Uranai-User-LoggedIn: ".(Account::userInfos()?'yes':'no'));
// <<<

$smarty = new Smarty;
$smarty->setTemplateDir(dirname(__FILE__) . "/../templates/user"); // Smarty はカレントディレクトリしか探さない
$smarty->setCompileDir(dirname(__FILE__). "/../templates_c/");
// smarty 設定　（テンプレート用） >>>
$appType = preg_match("/Azet (?P<OS>.*) App v\.(?P<version>.*)/", $_SERVER['HTTP_USER_AGENT'],$appInfo);
$appNeedsBottomMenu = app_botton_menu_check($appInfo['OS'], $appInfo['version']);
$config = array(
	'date_today' => date('Ymd')
	,'isIosApp' => preg_match("/Azet iOS App/", $_SERVER['HTTP_USER_AGENT'])
	,'isAndroidApp' => preg_match("/Azet Android App/", $_SERVER['HTTP_USER_AGENT'])
	,'isApp' => preg_match("/Azet .* App/", $_SERVER['HTTP_USER_AGENT'])
	,'AppOS'=> $appType ? $appInfo['OS'] : null
	,'AppVersion'=> $appType ? $appInfo['version'] : null
	,'plateform' => preg_match("/Azet .* App/iu", $_SERVER['HTTP_USER_AGENT'])?'アプリ':'サイト'
	,'cache_date' => CACHE_DATE
	,'apple_browser' => preg_match("/iPhone|iPad/", $_SERVER['HTTP_USER_AGENT'])
	,'android_browser' => preg_match("/Android/", $_SERVER['HTTP_USER_AGENT'])
	,'windowsphone_browser' => preg_match("/Windows Phone/", $_SERVER['HTTP_USER_AGENT'])
	,'windows_browser' => preg_match("/Windows/", $_SERVER['HTTP_USER_AGENT'])
	,'mac_browser' => preg_match("/Macintosh/", $_SERVER['HTTP_USER_AGENT'])
	,'browser_name' => browser_name($_SERVER['HTTP_USER_AGENT'])
	,'is_server' => IS_SERVER
	,'base_url' => BASE_URL
	,'appNeedsBottomMenu' => $appNeedsBottomMenu
);


$smarty->assign('config', $config);
// debug($config);
// <<<

$template_page = "404.tpl";

// URL parameters
//======================================================================

$_q = trim($_GET['q'], '/');
$history = new History();
$history->add($_q);
// echo "ログイン前:".$history->getPageBeforeLogin();
//debug($history->getList());
$history->clearPageBeforeLogin(); //ログイン前に訪問したページ情報をクリアする
$Q = explode("/", $_q);
// debug($Q);

global $conn;
Breadcrumb::add("TOP", "/");

//カスタム文言
$custom_message = new CustomMessage($conn);

//======================================================================
//                  _   _
//  _ __ ___  _   _| |_(_)_ __   __ _
// | '__/ _ \| | | | __| | '_ \ / _` |
// | | | (_) | |_| | |_| | | | | (_| |
// |_|  \___/ \__,_|\__|_|_| |_|\__, |
//                              |___/
//======================================================================


// special URLs
if(preg_match("/\d{8}/", $Q[0]) && !isset($Q[1])) {
	// RANK
	//=======
	// 例: /20160114 => /rank/20160114
	$mode = "rank";
	$_GET['d'] = $Q[0];

	Breadcrumb::add(Breadcrumb::Date_Convert($Q[0]), "/{$Q[0]}/");
}
elseif(preg_match("/\d{8}/", $Q[0]) && isset($Q[1])) {
	// DETAIL
	//=========
	// 例: /20160114/leo
	$mode = "detail";
	$_GET['d'] = $Q[0];
	$_GET["star"] = array_search($Q[1], $en_num_star);
	if ($_GET["star"] === FALSE) {
		$_GET["star"] = 1;
	}
	$detail_star = $Q[1];//add hirose 2017/03/22
	Breadcrumb::add(Breadcrumb::Date_Convert($Q[0]), "/{$Q[0]}/");
	Breadcrumb::add(Breadcrumb::Star_Convert($Q[1],$en_star), "/{$Q[0]}/{$Q[1]}/");
}
elseif(array_search($Q[0], $en_num_star) && !isset($Q[1])) {
	// TODAY DETAIL
	//===============
	// 例： /leo
	$mode = "detail";
	$_GET['d'] = date('Ymd');
	$_GET["star"] = array_search($Q[0], $en_num_star);
	if ($_GET["star"] === FALSE) {
		$_GET["star"] = 1;
	}
	$detail_star = $Q[0];//add hirose 2017/03/22

	Breadcrumb::add(Breadcrumb::Star_Convert($Q[0],$en_star), "/{$Q[0]}/");
}
elseif(preg_match("/(love|work|money)/", $Q[0]) && !isset($Q[1])) { //add yamaguchi 2017/06/28
	// TODAY TOPIC RANK
	//===============
	// 例： /love
	$mode = "rank";
	$_GET['d'] = date('Ymd');
	$data_type = $Q[0];
	$topic_jp = $topic_Jp[$data_type];

	Breadcrumb::add($topic_jp , "/{$Q[0]}/");
}
elseif(preg_match("/(love|work|money)/", $Q[0]) && preg_match("/\d{8}/", $Q[1]) && !isset($Q[2])) { //add yamaguchi 2017/06/28
	// TOPIC RANK
	//===============
	// 例： /love/20170704
	$mode = "rank";
	$data_type = $Q[0];
	$topic_jp = $topic_Jp[$data_type];
	$_GET['d'] = $Q[1];
	Breadcrumb::add($topic_jp , "/{$Q[0]}/");
	Breadcrumb::add(Breadcrumb::Date_Convert($Q[1]), "/{$Q[1]}/");
}
elseif(preg_match("/(love|work|money)/", $Q[0]) && array_search($Q[1], $en_num_star)) { //add yamaguchi 2017/06/28
	// TODAY TOPIC  DETAIL
	//===============
	// 例：/love/leo
	$mode = "detail";
	$_GET['d'] = date('Ymd');
	$data_type = $Q[0];
	$topic_jp = $topic_Jp[$data_type];
	$_GET["star"] = array_search($Q[1], $en_num_star);
	if ($_GET["star"] === FALSE) {
		$_GET["star"] = 1;
	}
	$detail_star = $Q[1];//add hirose 2017/03/22
	Breadcrumb::add($topic_jp , "/{$Q[0]}/");
	Breadcrumb::add(Breadcrumb::Star_Convert($Q[1],$en_star), "/{$Q[1]}/");
}
elseif(preg_match("/(love|work|money)/", $Q[0]) && preg_match("/\d{8}/", $Q[1]) && array_search($Q[2], $en_num_star)) { //add yamaguchi 2017/06/28
	// TOPIC  DETAIL
	//===============
	// 例：/love/20170704/leo
	$mode = "detail";
	$data_type = $Q[0];
	$topic_jp = $topic_Jp[$data_type];
	$_GET['d'] = $Q[1];
	$_GET["star"] = array_search($Q[2], $en_num_star);
	if ($_GET["star"] === FALSE) {
		$_GET["star"] = 1;
	}
	$detail_star = $Q[1];//add hirose 2017/03/22
	Breadcrumb::add($topic_jp , "/{$Q[0]}/");
	Breadcrumb::add(Breadcrumb::Date_Convert($Q[1]), "{$Q[0]}/{$Q[1]}/");
	Breadcrumb::add(Breadcrumb::Star_Convert($Q[2],$en_star), "/{$Q[2]}/");
}
//add kimura 2017/03/02 年間月間ページURL設定
elseif(preg_match('/ranking([0-9]{4})/',$Q[0],$match)) {
	$mode="ranking-past";
	$year_past = $match[1];
	if(preg_match('/(^[a-z]+$)/',$Q[1],$match)){ //add kimura 2017/03/06
		$month_past = $num_month[$match[1]];
	}
}elseif(preg_match('/ranking-past/',$Q[0],$match)){ // add kimura 2017/03/07 ranking-pastページ初期表示の値
	$mode="ranking-past";
	$year_past = date('Y');
	$month_past = date("m",strtotime("first day of last month"));//先月の月 //strtotime("-1 month") だと31日に当月が返っておかしくなります！

	//月が1月だったら年を前年に設定
	if(date("m") == "01"){
		$year_past = date('Y',strtotime("-1 year"));
	}
//他のトピックに切り替えた場合の初期表示
}elseif(preg_match("/(love|work|money)/", $Q[0]) && preg_match("/ranking-past/", $Q[1])){
	$mode="ranking-past";
	$data_type = $Q[0];
	$topic_jp = $topic_Jp[$data_type];
	$year_past = date('Y');
	$month_past = date("m",strtotime("first day of last month"));//先月の月

	//月が1月だったら年を前年に設定
	if(date("m") == "01"){
		$year_past = date('Y',strtotime("-1 year"));
	}
}elseif(preg_match("/(love|work|money)/", $Q[0]) && preg_match("/ranking([0-9]{4})/", $Q[1],$match)){
	$mode="ranking-past";
	$data_type = $Q[0];
	$topic_jp = $topic_Jp[$data_type];
	$year_past = $match[1];
	if(preg_match('/(^[a-z]+$)/',$Q[2],$match)){
		$month_past = $num_month[$match[1]];
	}
}
//add hirose start 2017/03/08
elseif($Q[0]=='whatnew') {
	$mode = 'whatnew';
	$whatnew_page = $_SESSION['whatnew_page'];

	if($Q[1] && preg_match('/page([0-9]*)/',$Q[1],$match)){ // 新着情報ページ遷移時
		// page logic
		$whatnew_page = $match[1];
		$_SESSION['whatnew_page'] = $whatnew_page;
	}
	else if($Q[1] && preg_match('/(\d{8})/',$Q[1],$match)) {
		// 詳細
		$whatnew_details = $match[1];
		if($Q[2] && preg_match('/(\d+)/',$Q[2],$match)){
			$news_id = $match[1];
		}
	}

	if(!$whatnew_page) {
		$whatnew_page = 1;
	}
}
elseif($Q[0]=='site-description') {
	// サイトの詳細ページ
	$mode = $Q[0];
	// site id in URL
	$site_id = $Q[1];
	// comments paging

	if(isset($Q[2]) && preg_match('/page(\d+)/',$Q[2],$match)) {
		Paginator::setCurrentPageFor('site-comments', $match[1]);
	}
	else {
		Paginator::setCurrentPageFor('site-comments', 1);
	}
}
elseif($Q[0]=='howtouse') {
	// サイトの使い方
	$mode = $Q[0];
	// 下層ページデータ
	$action = $Q[1];
}
elseif($Q[0] =='step'){
	$mode = $Q[0];
}

elseif($Q[0]=='app-link' && $Q[1]) {
	// アプリで恋愛＋仕事のメニューのリンクは固定ですので、
	// 前のページの履歴と比較した後に恋愛・仕事ページに繊維する
	$history->redirectAppTo($Q[1]);
}
elseif($Q[0]=='test' && !IS_SERVER) {
	// テスト用のテンプレートページ
	$mode = $Q[0];
	$template_page = "test.tpl";
}
//add hirose end
else {
	// ふつうのパラメター
	//====================
	$_GET["mode"] = $Q[0];
	$_GET["d"] = $Q[1];	// cleared by some modes below
	$_GET["star"] = $Q[2];	// cleared by some modes below

	$mode = $_GET["mode"];

	if (!is_numeric($_GET["star"])) {
		$_GET["star"] = array_search($_GET["star"], $en_num_star);
		if ($_GET["star"] === FALSE) {
			$_GET["star"] = 1;
		}
	}
}

// ======================================================================
//  _             _
// | | ___   __ _(_) ___
// | |/ _ \ / _` | |/ __|
// | | (_) | (_| | | (__
// |_|\___/ \__, |_|\___|
//          |___/
// ======================================================================


// DEFAULTS

//======================================================================
if ($mode == "") {
	$mode = "rank";
	#$mode="detail";
}
// SPECIFIC
if ($mode == "about") {
	// サイトについて
	$template_page = "about.tpl";
	Breadcrumb::add($config['plateform']."について", "/about/");
}

if ($mode == "company") {
	// 会社内容
	$template_page = "company.tpl";
	Breadcrumb::add("運営会社概要", "/company/");
}

if ($mode == "policy") {
	// プライバシーポリシー
	$template_page = "policy.tpl";
	//add okabe start 2016/06/23
	$action = $Q[1];
	if ($action =="regist") {
		$smarty->assign("modoru_link", smarty_function_sitelink(array('mode' => "/account/intro/")));
	}
	//add okabe end 2016/06/23
	Breadcrumb::add("プライバシーポリシー", "/policy/");
}

if ($mode == "site-list") {
	// サイト一覧
	$template_page = "site-list.tpl";
	require_once(dirname(__FILE__) . "/../libadmin/all_site_links.php");
	$sitelinks=array();
	list($sitelinks,$itirancount)=AllSiteLinks();
	$smarty->assign("sitelinks", $sitelinks);
	$smarty->assign("count", $itirancount);
	Breadcrumb::add("サイト一覧", "/site-list/");

	// それぞれのサイトのランキングデータ >>>
	$sites_ranking = SiteComment::compileSitesEvaluationData();
	$smarty->assign('sites_ranking', $sites_ranking);
	// <<<
}

if ($mode == "mypage") {
	if(Account::userGetId()) {
		// ユーザアカウントのページ
		$history->setPageBeforeLogin();
		$template_page = "mypage.tpl";
		Breadcrumb::add("マイページ", "/mypage/");
	}
	else {
		// ログアウトした後にマイページが見られません
		header("location:/account/login");
		exit;
	}
}


//add okabe start 2016/06/23
if ($mode == "kiyaku") {
	$template_page = "kiyaku.tpl";
	//add okabe start 2016/06/23
	$action = $Q[1];
	if ($action =="regist") {
		$smarty->assign("modoru_link", smarty_function_sitelink(array('mode' => "/account/intro/")));
	}
	//add okabe end 2016/06/23
	Breadcrumb::add("利用規約", "/kiyaku/");
}
//add hirose start 2017/05/22
if ($mode == "howtouse") {
	$template_page = "howtouse.tpl";
	$action = $Q[1];

	$button = array("scorpio-mini" => "TOPページについて","taurus-mini" => "各星座詳細ページについて","leo-mini" => "年間・月間ページについて","pisces-mini" => "コメント機能について","aquarius-mini" => "ログイン・会員登録について","libra-mini" => "その他");
	$bttn_link = array("TOPページについて" => "top","各星座詳細ページについて" => "detail","年間・月間ページについて" => "year","コメント機能について" => "comment","ログイン・会員登録について" => "login","その他" => "other",);
	$smarty -> assign("bttn_link",$bttn_link);
	$smarty -> assign("button",$button);
	Breadcrumb::add($config['plateform']."の使い方", "/howtouse/");
}
// add hirose end 2017/05/22
if ($mode == "announcement") {
	$template_page = "announcement.tpl";
	Breadcrumb::add("イベント告知", "/announcement/");
}

//======================================================================

if ($mode == "account") {
	// require
	require dirname(__FILE__)."/../../www/formCheck/formCheckRule.class.php";
	require dirname(__FILE__)."/../../www/formCheck/formCheckGroup.class.php";

	// account/<register-form>
	unset($_GET['d']);
	$action = $Q[1];
	$activationKey = $Q[2];	//add okabe 2016/06/08
	if($action == "login" || $action == "intro" || $action == "password-lost"){
		Breadcrumb::add("ログイン・会員登録", "/account/login/");
	}elseif($action == "logout"){
		Breadcrumb::add("ログアウト", "/account/logout/");
	}else{
		Breadcrumb::add("マイページ", "/mypage/");
	}
	if($action == "password-lost"){
		Breadcrumb::add("パスワードをお忘れの場合", "/account/password-lost/");
	}
	if($action == "intro"){
		Breadcrumb::add("新規登録のご案内", "/account/intro/");
	}
	if($action == "form"){
		Breadcrumb::add("登録情報変更", "/account/form/");
		if(!Account::userGetId() ){
			Breadcrumb::deleteAll();
		}
	}
	if($action == "comment"){
		Breadcrumb::add("コメント管理", "/account/comment/");
		if(!Account::userGetId() ){
			Breadcrumb::deleteAll();
		}
	}
	if($action == "unregist"){
		Breadcrumb::add("ユーザー削除", "/account/unregist/");
	}
}
//======================================================================


//add okabe start 2016/03/25
$extpara = "";
//↓add hirose start 2017/03/06 星座別ページ開発用
if ($mode == "whatnew") {

	if($whatnew_details && $news_id) {
		// 詳細ページ
		$news_details = News::getNewsDetailsByDate($whatnew_details,$news_id);
		$smarty->assign('news_details', $news_details);
		$template_page = "whatnew-details.tpl";
		Breadcrumb::add("新着情報一覧", smarty_function_sitelink( array('mode' => 'whatnew/page'.$whatnew_page)) );
		Breadcrumb::add($news_details['news_title'], $_q);
	}
	else {
		// 一覧ページ
		$smarty->assign('whatnew_page', $whatnew_page);//現在のページを取得

		// お知らせの一覧
		$news_list = News::getList($whatnew_page, NEWS_PER_PAGE);

		// page count
		$page_count = News::countPages(NEWS_PER_PAGE);
		$smarty->assign('news_page_count',$page_count);

		//件数表示
		$smarty->assign('news_list',$news_list);
		$template_page = "whatnew-list.tpl";
		Breadcrumb::add("新着情報一覧", smarty_function_sitelink( array('mode' => 'whatnew/page'.$whatnew_page)) );
	}
}
//add end hirose

//add okabe end 2016/03/25
if($mode == "ranking-past"){ //add kimura 2017/03/02 年間月間ランキング
	require_once(dirname(__FILE__) . "/../libadmin/ranking_past.php");
	$template_page = "ranking-past.tpl";
	//$yearNav = YearNav($month_past,$data_type);
	//$yearNav2 = YearNav2($month_past,$data_type);
	//$select = Select_Fortune($month_past,$year_past,$data_type);
	//$ts = new TotalScore;
	$rankingP = array();
	$current_date = $year_past.$month_past."01";
	$current_date = date('Ymd',strtotime($current_date));
	//print $current_date. DB_DATE_DTL.$data_type;
	if($month_past == 'total'){
		if($data_type == 'money'&& $year_past < date('Y',strtotime(DB_DATE_DTL_M))){

		}else{
		$rankingP = UranaiRanking::getYearly($year_past,$data_type);
		}
	}else{
		if($data_type == 'money'&& $current_date < DB_DATE_DTL_M){
		}elseif($data_type && $data_type != 'money' && $current_date < DB_DATE_DTL){
		}else{
			$rankingP = UranaiRanking::getMonthly($year_past,$month_past,$data_type);
		}
	}
	if($month_past == 'total'){
		$yearNav = YearNav::Get_Year_Link($year_past,$data_type);
		$select = SelectFortune::Select_Fortune_Year($year_past,$month_past,$data_type);
	}else{
		$yearNav = YearNav::Get_Month_Link($year_past,$data_type);
		$select = SelectFortune::Select_Fortune_Month($year_past,$month_past,$data_type);
	}
	$smarty->assign('select',$select);
	$smarty->assign('yearnav',$yearNav);
	$smarty->assign('year_past',$year_past);
	$smarty->assign('month_past',$month_past);
	$smarty->assign('t_jp',$topic_Jp_name);
	$smarty->assign('rankingP',$rankingP);
	if($data_type){
		$fortune = $topic_Jp["$data_type"];
		Breadcrumb::add($fortune , "/{$Q[0]}/");
	}
	Breadcrumb::add("年間・月間ランキングTOP", "/ranking-past/");
	$smarty->assign('sns_on',true);
}
if($mode == "comment-violation-form"){
	$template_page = "comment-violation-form.tpl";
}
// rankings
$rankings = array();

// 日付確認
$date = $_GET["d"];
if ($date == "") {
	$date = date("Ymd");
}

//最古の日付
if($data_type == 'money'){
	$old_date = PREV_DATE_DTL_M;
}elseif($data_type){
	$old_date = PREV_DATE_DTL;
}else{
	$old_date = PREV_DATE;
}

//サイト数集計
if($data_type){
	$sites_count = UranaiRanking::countTopicLogsSites($date,$data_type);
}else{
	$sites_count = UranaiRanking::countLogsSites($date);
}
//======================================================================
if(!$data_type){
	$data_type = "";
}

// 共有オブジェクト >>>
$ranking = new UranaiRankingEx($date,$data_type);
$user = Account::userInfos();
if($user) {
	$smarty->assign('user', $user);
}
// <<<

//add yamaguchi end 20170309
//======================================================================
// 一覧モード
$topic_jp = $topic_Jp[$data_type];
if ($mode == "rank" && $date <= date('Ymd') && $date >= $old_date) {
	$template_page = "ranking-index.tpl";
	$rankings = $ranking->getRanks();
	//add hirose 2016/03/10
	if($date == ""){
		$day = date('Y-m-d');
	}else{
		$day = date('Y-m-d',strtotime($date));
	}
	$smarty->assign('calendar',linkCalendar(array($day,$data_type)));
	$smarty->assign('previous_link', previous_date($day));
	$smarty->assign('next_link', next_date($day));
	$smarty->assign('previous_month',previous_month_date($day,$data_type));//add hirose 2017/03/21
	$smarty->assign('next_month',next_month_date($day));//add hirose 2017/03/21
	$smarty->assign('topic_Jp', $topic_Jp);

	$first_rank = $ranking->first_rank($day,$data_type);
	$smarty->assign('first_str_con',$first_rank);

	$smarty->assign('sns_on',true);//add hirose 2017/03/16
	$smarty->assign('adg_on',true);
} elseif ($mode == "detail"&& $date <= date('Ymd') && $date >= $old_date) {
	//======================================================================

	// それぞれのサイトのランキングデータ >>>
	$sites_ranking = SiteComment::compileSitesEvaluationData();
	$smarty->assign('sites_ranking', $sites_ranking);
	// <<<

	// テンプレート設定
	$template_page = "ranking-detail.tpl";

	$star = $_GET["star"];

	$current_rank = $ranking->getRankForStar($star);
	// test >>>
	// version all ranks (headings listing)
	$allRanks = $ranking->getDetailsForStarRank($star,$data_type);
	$smarty->assign('allRanks', $allRanks);
	// plus data for AJAX requests
	$smarty->Assign('details_date', $date);
	$smarty->Assign('details_star', $star);

	$smarty->assign('previous_link', previous_date($date));
	$smarty->assign('next_link', next_date($date));
	$smarty->assign('adg_on',true);
	// <<<
	//add okabe start 2016/05/26
	$smarty->assign('allEnStars', $en_num_star);
	$smarty->assign('allJpStars', $name);
	$smarty->assign('topic_Jp', $topic_jp);
	$star_dates_j = array();
	foreach ($star_dates as $k => $v) {
		$v1a = explode("-", $v['from']);
		$v1 = intVal($v1a[0])."月".intVal($v1a[1])."日";
		$v2a =  explode("-", $v['to']);
		$v2 = intVal($v2a[0])."月".intVal($v2a[1])."日";
		$star_dates_j[$k] = $v1."～".$v2;
	}
	$smarty->assign('allStarDate', $star_dates_j);
	if (strlen($date) > 0) {
		$smarty->Assign('selectDate', $date."/");
	} else {
		$smarty->Assign('selectDate', "");
	}
	//add okabe end 2016/05/26

	//カスタムメッセージ
	$custom_message->loadMessages([
		"SPEECH_BUBBLES"
	]);

	// 期間イベント用
	// $season_event = new SpringEvent; // comment out uenishi 2023/04/27
    // $smarty->assign('event_detail_data', $season_event->getEventSignData($_GET['star']));



	$smarty->Assign('seiza_comment', getSeizaDetailMessage($star) );	//add okabe 2016/07/09
	$smarty->assign('graph_data',$ranking->outputGraph(date('Y-m-d'),$star,$data_type)); //グラフ出力
	$updown_rank = $ranking->getUpdown();//add hirose 2017/03/09
	foreach($updown_rank as $index => $udranks){

		foreach($udranks as $strnmu => $udrank){
		//	print($star.'/');
		//	print($strnmu);
			if($strnmu == $star){
			$updown = $udrank;
	//		print($updown);
			}
		}
	}
	$smarty->assign("updown_rank",$updown);//add hirose 2017/03/09
	$smarty->assign('sns_on',true);//add hirose 2017/03/16
	$smarty->assign('calendar',linkCalendar(array($date,$data_type),$star));
	$smarty->assign('previous_month',previous_month_date($date,$data_type));//add hirose 2017/03/21
	$smarty->assign('next_month',next_month_date($date));//add hirose 2017/03/21

} elseif($mode==='account') {
	// ユーザ用
	//======================================================================
	if(!$action) {
		$action = "intro";
	}

	//add okabe start 2016/06/09
	if($action == "regist") {
		// moved into account-regist.ctrl.php
		require dirname(__FILE__)."/../libadmin/account-regist.ctrl.php";
		exit;
	}
	//add okabe end 2016/06/09

	// default template file pattern
	$template_page = "account-{$action}.tpl";

	//add okabe start 2016/06/21
	if ($action == "intro") {
		$smarty->assign("mailtoData", ACCOUNT_REGIST_MAILTO_MASKING);
		$smarty->assign("mailtoDataClear", "mailto:".ACCOUNT_REGIST_MAILTO);
	}
	//add okabe end 2016/06/21

	//add okabe start 2016/06/24 アンケート付きのユーザー削除ページ
	if ($action == "unregist") {
		$smarty->assign("registed_mail", $user['email']);
		$smarty->assign('hideLoginBtn', true);
	}
	//add okabe end 2016/06/24

	// logicは別ファイルする
	require dirname(__FILE__)."/../libadmin/account.ctrl.php";

	if(($action=='login' && $_POST) || $action=='logout') {
		$smarty->assign('hideLoginBtn', true);
	}
	if($bread_del){
		Breadcrumb::deleteAll();
	}

//会員様限定ランキングリスト
}elseif($mode == 'registered-person'){
	$template_page = "registered-person-limited.tpl";
	if($user){
		$user_id = Account::userGetId();
		$user_star = get_user_bd($user_id);
		foreach($topic_Jp as $data_type_ => $data_name){
			if(!$data_type_){
				$data_type = 'defolt';
			}else{
				$data_type = $data_type_;
			}
			$ranking = new UranaiRankingEx($date,$data_type_);
			$rankings = $ranking->getRanks();
			if(!$rankings){
				for($sn = 1; $sn < 13; $sn++){
				$updown_list[$sn][$data_type]['ranking'] = "集計中";
				$updown_list[$sn][$data_type]['mark'] = "集計中";
				}
			}else{
				//print_r ($rankings);
				foreach($rankings as $ranking_dtl){
					$updown_list[$ranking_dtl['star_num']][$data_type]['ranking'] = $ranking_dtl['num']."位";
				}
				$updown_rank = $ranking->getUpdown();
				$updown = $updown_rank['mark'];
				foreach($updown as $sn => $mark){
					//$updown_list[$data_type][$sn]['star_jp'] = $jpn_num_star[$sn];
					$updown_list[$sn][$data_type]['mark'] = $mark;
				}
			}
		}
		//print_r($updown_list);
		$open_day = date("n月j日 G時");
		//print $open_day;
		$smarty->assign("open_day",$open_day);
		$smarty->assign("jpn_num_star",$jpn_num_star);
		$smarty->assign("user_star",$user_star);
		$smarty->assign("updown_rank",$updown_list);
		$smarty->assign("login_ok",true);
		Breadcrumb::add("マイページ", "/mypage/");
		Breadcrumb::add("順位一覧", "/registered-person/");
	}else{
		Breadcrumb::deleteAll();
	}
}
// ======================================================================
if ($mode == "site-description") {
	// require
	require_once(dirname(__FILE__) . "/../libadmin/site_details.class.php");
	require_once(dirname(__FILE__) . "/../libadmin/site_comment_report.class.php");
	$template_page = "site-description.tpl";

	$site_details = SiteDetails::getById($site_id);
	// debug($site_details);

	$img_charcter_key = array_rand($en_num_star);
	$img_charcter = $en_num_star[$img_charcter_key];
	$smarty->assign('img_charcter', $img_charcter);

	if(!$site_details) {
		// サイトがない（削除された？）
		$template_page = "404.tpl";
	}
	else {
		// site details data
		$smarty->assign('site_details', $site_details);
		// comment violations data
		$smarty->assign('violations', SiteCommentReport::getViolationsForUser());

		// site comments controller (sub file)
		include(dirname(__FILE__) . "/../libadmin/site_comment.ctrl.php");

		Breadcrumb::add("サイト一覧", "/site-list/");
		Breadcrumb::add($site_details['site_name'], "/site-list/{$site_id}");
	}
}

if ($mode == "howtouse") {
	if($action == "comment") {
		// require
		$template_page = "howtouse-comment.tpl";
		Breadcrumb::add("コメント機能の使い方", "/howtouse/comment");
	}
}

if ($mode == "digest") {

	$date = date('Ymd');//年だけの値が$dateに入っているためリンクがおかしくなる。当日に修正
	//URL https://uranairanking.jp/digest/YYYY の年yyyyがなければ表示しない
	if (empty($Q[1])) {
	  $template_page = "404.tpl";

	//年yyyyが存在すればページを出す
	} else {

	  $digest_year = $Q[1]; //年
	  $template_page = "{$digest_year}_digest.tpl"; //テンプレート
	  $digest_content_path = __DIR__ . "/../data/{$digest_year}_digest.json"; //サイト一覧jsonのパス

	  //テンプレートファイルが有るかチェック
	  if (!file_exists(__DIR__ . "/../templates/user" . "/" . $template_page)) {
		$template_page = "404.tpl";
	  }

	  //json読み込み
	  $digest_list = [];
	  if (file_exists($digest_content_path)) {
		$json_ = file_get_contents($digest_content_path);
		$digest_list = json_decode($json_, true);
	  }

	  //アサイン
	  $smarty->assign('digest_list', $digest_list);
	  Breadcrumb::add("{$digest_year}年の占いサイトまとめ", "/digest");
	}
}
// 相互リンク Tracking 
if ($mode == "step") {
	include(dirname(__FILE__) . "/../libadmin/user_tracking.php");
	include(dirname(__FILE__) . "/../libadmin/sougoulink-listing_action.php");
	
	$sougolink_sites_list = getConfirmedSitesWithKey();
	

	$url_id = $Q[1];
	
	$referer = "";
	
	// 相互リンクサイト名を獲得
	if(array_key_exists($url_id,$sougolink_sites_list)){
		$referer = $sougolink_sites_list[$url_id]["site_name"];
	}

	// もし ユーザーURLが不明だったら、
	if(empty($referer)){
		$referer = "anonymous". "-" . $_SERVER['HTTP_REFERER'];
	}
	
	user_tracking($referer);
}

if ($mode == "contact") {
	include(dirname(__FILE__) . "/../libadmin/contact.class.php");
	$action = $Q[1];
	$template_page = 'contact.tpl';

	$contact = new ContactFormHandler;
	if ($action != 'send') {
		$contact->saveTokenToSession();
		$token = $contact->getToken(); //生成されたトークンを取り出す
		$smarty->assign('contactToken',$token);
	}

	//送信されたとき
	if ($action == 'send') {
		$a = $contact->getTokenFromSession();
		if($_POST['contact_token']==$a){
	
			//送信内容をオブジェクトにセット
			$contact->setUserName($_POST['name'])
				->setFrigana($_POST['furigana'])
				->setEmail($_POST['email'])
				->setConfirmEmail($_POST['confirm_email'])
				->setComment($_POST['comments']);
			//バリデーション
			$errors = $contact->validate();
			$smarty->assign('errors', $errors);

			// バリデーションのエラーがあってもトークンは画面に埋める
			$smarty->assign('contactToken', $a);
			
			//エラーがなければサンクスページを表示
			if (!$errors) {
				$template_page = 'contact-thanks.tpl';
				$contact->sendEmailToAdmin();
				$contact->sendEmailToUser();
				$contact->deleteToken();
			}
		}else{
			//トークンが違ったらトップへ飛ばす
			header('Location: /');
			exit;
		}
	}
	Breadcrumb::add('お問い合わせ', '/contact/');
	
}



if($template_page == "404.tpl"){
	header("HTTP/1.0 404 Not Found", true, 404);
	Breadcrumb::deleteAll();
	Breadcrumb::add("404エラー","/");
	$smarty->assign("notfoundpage", true);
}


// 星座名
$star_name = "star".($star);
$star_name_kanji = "star_kanji".($star);

//リンク用
$today = date('Ymd');
//print $mode;
//print $date;
// debug($topic_Jp_name);
$link_basic = make_menu_link($topic_Jp_name ,$date,$mode,$en_star[$name[$star_name]],$data_type);

//print_r ($link_basic);
// debug($link_basic);


//ogpのランダム表示
$week_num = date('w');
if($week_num == '1' || $week_num == '4'){
	$ogp = '1';
}elseif($week_num == '2'){
	$ogp = '2';
}elseif($week_num == '6'){
	$ogp = '3';
}elseif($week_num == '3'){
	$ogp = '4';
}else{
	$ogp = '5';
}
$smarty->assign('ogp_num', $ogp);

$suggest_msg = UnseiSuggestion::suggest_html($data_type);

//======================================================================
//                     _                 _       _           _
// ___ _ __ ___   __ _| |_ _   _    __ _| | ___ | |__   __ _| |
/// __| '_ ` _ \ / _` | __| | | |  / _` | |/ _ \| '_ \ / _` | |
//\__ \ | | | | | (_| | |_| |_| | | (_| | | (_) | |_) | (_| | |
//|___/_| |_| |_|\__,_|\__|\__, |  \__, |_|\___/|_.__/ \__,_|_|
//                         |___/   |___/
//
//__   ____ _ _ __ ___
//\ \ / / _` | '__/ __|
// \ V / (_| | |  \__ \
//  \_/ \__,_|_|  |___/
//======================================================================

//カスタム文言
$smarty->assign('custom_message', $custom_message);

//パンくずリスト用 add yamaguchi start 20170309
$breadlist=Breadcrumb::getAdd();
$smarty->assign('Breadcrumblist', $breadlist);

// >>>
// 日付表示
$weekday = array( "日", "月", "火", "水", "木", "金", "土" );
$w = $weekday[date("w", strtotime($date))];
$date_print = date('Y年n月j日', strtotime($date)) . "($w)";


$data = array(
	"status" => "OK"
	,"title" => "総合占いサイト"
	,"date" => $date_print
	,"date_num" => $date
	,"rank" => $rankings
	,"sites_count" => $sites_count
	,"current_rank" => $current_rank
	,"star_name" => $name[$star_name]
	,"star_name_kanji" => $name[$star_name_kanji]
	,"star_name_en" => $en_star[$name[$star_name]]
	,"extpara" => $extpara		//add okabe 2016/03/25 拡張パラメータ
	,"m_rank" =>$m_rankings //add yamaguchi 月間用ランキング
	,"m_sites_count" => $m_sites_count//add yamaguchi 月間用サイトカウント
	,"data_type" => $data_type//add yamaguchi 運勢種類パラメーター
	,"topic_name" => $topic_jp //add kimura トピック名　（恋愛運,仕事運,...）総合運の場合はNULL
	,"link_basic" => $link_basic //フッターメニューのURL用
);
// <<<

//SEO用文言
$seo = array(
	"date" => get_seo_date_string($date)
	,"topic_or_unsei" => $topic_jp ?: "運勢"
	,"topic_or_sougouun" => $topic_jp ?: "総合運"
	,"topic_or_empty" => $topic_jp ?: ""
	,"panel_title" => ($topic_jp ?  "「12星座占いランキング」<br>~".$topic_jp."~" : "12星座占いランキング～<br>毎日の占いを")
);
$smarty->assign("seo", $seo);

//echo "<pre>";
//print_r($data);
//echo "</pre>";

// 相互ランク
$sougo_list_count = ceil( count(sougolink_confirmed_sites()) / 2 );
$smarty->assign('sougo_list_count',$sougo_list_count );
$smarty->assign('sougo_link_list', sougolink_confirmed_sites() ); 

// move simon 2017-03-24 >>>
$smarty->assign("defaultStar", DEFAULT_STAR);
$smarty->assign('allEnStars', $en_num_star);
$smarty->assign('allJpStars', $name);
// <<<

// 今日の日付
$smarty->assign("date", array("now" => date("Y年n月j日") . "($w)"));		// XXX に依存
//イベント
$smarty->assign('design_name',  Event::getDesignName($mode) );
$smarty->assign('announcement', Event::getAnnouncement() );
//モード判定
$smarty->assign("page_mode", $mode);
// {$rank.url[$rank.num]}
$smarty->assign("data", $data);
$smarty->assign("suggest_msg", $suggest_msg);

//広告on/offパラメータ
$smarty->assign("ad_demo", $_POST['ad-demo']);

//トップページ説明文文章(省略形) ($dataを$smartyオブジェクトにアサインした後でないと中の変数が消える!)
$site_desc_page = $smarty->fetch("mainline.parts.sub.tpl");
$site_desc_abbr_ = strip_tags($site_desc_page, "<br>");
$site_desc_abbr = trim_msg($site_desc_abbr_, SITE_DESC_TEXT_LEN);
$smarty->assign("site_desc_abbr", $site_desc_abbr);

// 期間イベント用データ // comment out uenishi 2023/04/27
// $season_event_data = new SpringEvent;
// $smarty->assign('event_data',$season_event_data->getEventData() );
// $smarty->assign('modal_data',$season_event_data->ModalGeneration() );

// $sda[] = $season_event_data->ModalGeneration();
// foreach($sda as $key => $value){
// echo $value;
// }


$smarty->display($template_page);

//benchmark for local testing >>>
// add simon 2015-11-30
// if(!IS_SERVER) {
// 	$end_time = microtime(true);
// 	$spent_time = $end_time - $start_time;
// 	print "exec time: $spent_time sec";
// 	print "<span class='uadisplay'>{$_SERVER['HTTP_USER_AGENT']}</span>";

// 	// debug data display
// 	if(isset($GLOBALS['debug'])) {
// 		foreach($GLOBALS['debug'] as $msg) {
// 			print "<pre class='alert alert-debug'>";
// 			print $msg;
// 			print "</pre>";
// 		}
// 	}
// }

// <<<
// vim: foldmethod=marker
