<?php
include("../cone.inc");
include("./sub/setup.inc");
include("./sub/array.inc");
// =====================================

/**
 * main xml line generator
 * @param string $path_ (partial url starting with / )
 * @return string
 */
function makeXMLline($path_) {
	return "<url><loc>https://ozzys.jp$path_</loc></url>";
}

// ======================================


// static pages
// ------------
$staticlinks_xml = "";

$staticlinks_xml .= makeXMLline("/");
$staticlinks_xml .= makeXMLline("/tenpo/");
$staticlinks_xml .= makeXMLline("/link/");
$staticlinks_xml .= makeXMLline("/riyou/");
$staticlinks_xml .= makeXMLline("/riyou/kaiketsu.htm");
$staticlinks_xml .= makeXMLline("/tuhan.htm");
$staticlinks_xml .= makeXMLline("/privacypolicy.htm");


// news data!
// ---------
$news_xml = "";

$LIST = file("$LOGDATA_DIR/$news_file");
foreach($LIST as $prod) {
	if(!$prod || !preg_match("/^n/", $prod)) continue;
	$news_xml .= "\n".makeXMLline("/goods/".trim($prod)."/");
}

// main categories
// ---------------

function sitemapMainlist() {
	global $db,$CLASS_L,$LOG_DIR,$m_cate_file,$index;

	$sitemap_lines = "";
	$URL = "/goods";
	
	//	表示確認
	$CLASS_M_L = array();
	$HYOUJI = array();
	$sql  = "SELECT class_m, COUNT(class_m) AS count FROM list" .
			" WHERE display='2'" .
			" AND state!='1'".		//	2009/04/25	add ookawara
			" GROUP BY class_m;";
	if ($result = pg_query($db,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$class_m = $list['class_m'];
			$count = $list['count'];
			$CLASS_M_L[$class_m] = $count;
			$num = floor($class_m / 100) * 100;
			if ($count > 0) { $HYOUJI[$num] = "ok"; }
		}
	}

	//	カテゴリー表示
	if ($CLASS_M_L) {
		//	分類読込 表示
		$sql  = "SELECT class_m, class_m_n FROM class ORDER BY class_m;";
		if ($result = pg_query($db,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$class_m = $list['class_m'];
				$class_m_n = $list['class_m_n'];
				$count = $CLASS_M_L[$class_m];
				$num = floor($class_m / 100) * 100;
				
				if ($cls == "" && $class_m > 900 && $count > 0) {
					$cls = $num;
				}
				
				if (!$HYOUJI[$num]) { continue; }
				
				if ($cls != $num && $num < 900) {
					$num_ = $num / 100;
					$sitemap_lines .= "\n".makeXMLline("$URL/$num/$index" );
					$cls = $num;
				}
				
				if ($count > 0) {
					$sitemap_lines .= "\n".makeXMLline("$URL/$num/s$class_m/$index");
				}
			}
		}
	}

	return $sitemap_lines;
}
$categories_xml = sitemapMainlist();


// Makers
// ------
function sitemapMakerlist() {
	global $db,$LOG_DIR,$m_maker_file,$index;

	$URL = "/goods";
	
	$sitemap_lines = "";
	
	$MAKER_L = array();
	$sql  = "SELECT maker_num, COUNT(maker_num) AS count FROM list" .
			" WHERE display='2'" .
			" AND state!='1'".		//	2009/04/25	add ookawara
			" GROUP BY maker_num;";
	if ($result = pg_query($db,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$maker_num = $list['maker_num'];
			$count = $list['count'];
			$MAKER_L[$maker_num] = $count;
		}
	}

	//	メーカー表示
	if ($MAKER_L) {
		//	メーカー読込 表示
		$sql  = "SELECT maker_num, maker_name FROM maker ORDER BY maker_name;";
		if ($result = pg_query($db,$sql)) {
			$bmaker = "";
			while ($list = pg_fetch_array($result)) {
				$maker_num = $list['maker_num'];
				$maker_name = $list['maker_name'];
				$count = $MAKER_L[$maker_num];
				if ($bmaker == $maker_name) { continue; }
				if ($count > 0) {
					$sitemap_lines .= "\n".makeXMLline("$URL/m$maker_num/$index");
				}
				$bmaker = $maker_name;
			}
		}
	}

	return $sitemap_lines;
}
$makers_xml = sitemapMakerList();


// XML generating
// --------------
$xml = <<<WAKABA
<?xml version="1.0" encoding="UTF-8"?>
<urlset xmlns="https://www.sitemaps.org/schemas/sitemap/0.9"> 
$staticlinks_xml
$news_xml
$categories_xml
$makers_xml
</urlset>
WAKABA;

// output
header("Content-Type: text/xml; charset=utf-8");
echo $xml;