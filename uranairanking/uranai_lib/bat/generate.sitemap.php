#!/usr/local/bin/php
<?php
/**
 * 各ページ用のサイトマップをそれぞれのロジックで作成します。
 */
require_once(dirname(__FILE__)."/../libadmin/sitemap.class.php"); //クラス読み込み
require_once(dirname(__FILE__).'/cron.tools.php');
require_once(dirname(__FILE__).'/../libadmin/config.php');
require_once(dirname(__FILE__)."/../libadmin/sitemap.conf.php"); //生成用コンフィグ読み込み

$log = new Log();
$log->start();
//----------------------------------
// 各トピック/日付/
// archive_top(_トピック).xml
//----------------------------------
foreach ($topic_start_dates as $topic => $date) {
	$archive_top = new UrlNode(BASE_URL, null);
	$archive_top->quiet();

	$start = strtotime($date);
	$end = strtotime("now");

	$topic_node = new UrlNode($topic, $archive_top);
	$topic_node->quiet();

	while ($start <= $end) {
		$curr_day = $start;
		$date_node = new UrlNode(date("Ymd", $curr_day), $topic_node);
		$date_node->setPriority("0.5");
		$date_node->setLastMod(date("Y-m-d", $curr_day));

		$start = strtotime('+1 day', $start);
		$topic_node->addChild($date_node);
	}
	$archive_top->addChild($topic_node);

	$xml = new Xml($archive_top);
	if(file_put_contents(dirname(__FILE__)."/../../www/sitemap/archive_top".($topic ? "_".$topic : "").".xml", $xml->make())){
		$log->add("サイトマップ生成:"."archive_top".($topic ? "_".$topic : "").".xml", "成功:OK");
	}else{
		$log->add("サイトマップ生成:"."archive_top".($topic ? "_".$topic : "").".xml", "ファイル書き込みに失敗しました。:ERR");
	}
}
//----------------------------------
// 各トピック/星座/ : 一年ごとURL集
// archive_(トピック_)20xx.xml
//----------------------------------
foreach($topic_start_dates as $topic => $date){


	$archive_top = new UrlNode(BASE_URL, null);
	$archive_top->quiet();

	$start = strtotime($date);
	$end = strtotime("now");

	$topic_node = new UrlNode($topic, $archive_top);
	$topic_node->quiet();
	$last_loop_year = null;

	while ($start <= $end) {

		$curr_day = $start;
		$year = date("Y", $curr_day);
		$date_node = new UrlNode(date("Ymd", $curr_day), $topic_node);
		$date_node->quiet();

		foreach ($en_num_star as $star) {
			$star_node = new UrlNode($star, $date_node);
			$star_node->setPriority("0.5");
			$star_node->setLastMod(date("Y-m-d", $curr_day));
			$date_node->addChild($star_node);
		}

		$topic_node->addChild($date_node);

		if((date("ymd",$curr_day) == date("ymd") || date("md",$curr_day) == "1231") && $last_loop_year != null){
			$archive_top->addChild($topic_node);
			$xml = new Xml($archive_top);

			//reset
			$archive_top = new UrlNode(BASE_URL, null);
			$archive_top->quiet();
			$topic_node = new UrlNode($topic, $archive_top);
			$topic_node->quiet();

			if(file_put_contents(dirname(__FILE__)."/../../www/sitemap/archive_".($topic ? $topic."_" : "").$year.".xml", $xml->make())){
				$log->add("サイトマップ生成:"."archive_".($topic ? $topic."_" : "").$year.".xml", "成功:OK");
			}else{
				$log->add("サイトマップ生成:"."archive_".($topic ? $topic."_" : "").$year.".xml", "ファイル書き込みに失敗しました。:ERR");
			}
		}
		$last_loop_year = $year;
		$start = strtotime('+1 day', $start);
	}
	//endwhile
}
//----------------------------------
// 各トピック : 月間年間URL集
// past(_トピック).xml
//----------------------------------
foreach ($topic_start_dates as $topic => $date) {
	$past_root = new UrlNode(BASE_URL, null);
	$past_root->quiet();

	//集計を行うのは開始月の翌月から(10/11開始なら 10月分のページはなく11月分からのページが最初)
	$start = strtotime('+1 month', strtotime($date));
	if ($topic == "") { $start = strtotime($date); } //総合は2016年1月も出す
	$end = strtotime("now");

	$topic_node = new UrlNode($topic, $past_root);
	$topic_node->quiet();

	while ($start <= $end) {
		$curr_day = $start;
		$year = date("Y", $curr_day);
		$month = date("m", $curr_day);

		$year_node = new UrlNode("ranking".$year, $topic_node);
		$year_node->quiet();
		$month_node = new UrlNode($month_past_formatB[$month], $year_node);
		$month_node->setPriority("0.5");
		$month_node->setLastMod(date("Y-m-d", strtotime("first day of next month",$curr_day) ));

		//今月分はまだ出さない
		if($year == date("Y") && $month == date("m")){
			$month_node->quiet();
		}

		$year_node->addChild($month_node);
		$topic_node->addChild($year_node);

		$start = strtotime('+1 month', $start);
	}

	$past_root->addChild($topic_node);
	$xml = new Xml($past_root);

	if(file_put_contents(dirname(__FILE__)."/../../www/sitemap/past".($topic ? "_".$topic : "").".xml", $xml->make())){
		$log->add("サイトマップ生成:"."past".($topic ? "_".$topic : "").".xml", "成功:OK");
	}else{
		$log->add("サイトマップ生成:"."past".($topic ? "_".$topic : "").".xml", "ファイル書き込みに失敗しました。:ERR");
	}
}

//----------------------------------
// 固定ページ
// static_pages.xml
//----------------------------------
$static_pages = "";
$root = new UrlNode(BASE_URL, null);
$root->setPriority("1.0");
$root->setLastMod(date("Y-m-d"));

foreach($topic_start_dates as $topic => $date){

	$start = strtotime($date);
	$end = strtotime("now");

	$topic_node = new UrlNode($topic, $root);
	$topic_node->setPriority("0.5");

	if($topic == ""){
		$topic_node->quiet();
	}

	foreach($en_num_star as $star){
		$star_node = new UrlNode($star, $topic_node);
		$star_node->setPriority("0.9");
		$star_node->setLastMod(date("Y-m-d"));
		$root->addChild($star_node);
	}

	$root->addChild($topic_node);

}
foreach($static_pages_arr as $page){
	$page_node = new UrlNode($page, $root);
	$page_node->setPriority("0.5");
	$root->addChild($page_node);
}

$xml = new Xml($root);
$static_pages .= $xml->make();

if(file_put_contents(dirname(__FILE__)."/../../www/sitemap/static_pages.xml", $static_pages)){
	$log->add("サイトマップ生成:static_pages.xml", "成功:OK");
}else{
	$log->add("サイトマップ生成:static_pages.xml", "ファイル書き込みに失敗しました。:ERR");
}

//----------------------------------
// サイト中継ページ
// site_description.xml
//----------------------------------
$root = new UrlNode(BASE_URL, null);
$root->quiet();

$sql = "SELECT site_id";
$sql.= " FROM site";
$sql.= " WHERE 1";
$sql.= "  AND is_delete = 0";
$sql.= "  AND is_execute = 1";

$site_desc_root_node = new UrlNode("site-description", $root);
$site_desc_root_node->quiet();

$res = $conn->query($sql);
while ($row = mysqli_fetch_assoc($res)) {
	$site_page_node = new UrlNode($row['site_id'], $site_desc_root_node);
	$site_page_node->setPriority("0.5");
	$site_desc_root_node->addChild($site_page_node);
}
$root->addChild($site_desc_root_node);
$xml = new Xml($root);

if(file_put_contents(dirname(__FILE__)."/../../www/sitemap/site_description.xml", $xml->make())){
	$log->add("サイトマップ生成:site_description.xml", "成功:OK");
}else{
	$log->add("サイトマップ生成:site_description.xml", "ファイル書き込みに失敗しました。:ERR");
}
//----------------------------------
// サイトマップインデックスファイル
// sitemap-index.xml
//----------------------------------
$sitemap_index = "";
$sitemap_files = array_filter(scandir(dirname(__FILE__)."/../../www/sitemap/"), function($item) {
	return !is_dir(dirname(__FILE__)."/../../www/sitemap/" . $item);
});
$sitemap_index .= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
$sitemap_index .= "<sitemapindex xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";

foreach($sitemap_files as $file){
	$sitemap_index .= "\t<sitemap>\n";
	$sitemap_index .= "\t\t<loc>".BASE_URL."sitemap/".$file."</loc>\n";
	$sitemap_index .= "\t</sitemap>\n";
}
$sitemap_index .= "</sitemapindex>";

if(file_put_contents(dirname(__FILE__)."/../../www/sitemap-index.xml", $sitemap_index)){
	$log->add("サイトマップ生成:sitemap-index.xml", "成功:OK");
}else{
	$log->add("サイトマップ生成:sitemap-index.xml", "ファイル書き込みに失敗しました。:ERR");
}
$log->stop();
