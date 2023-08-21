<?php
/**
 *======================================================================
 * 広告をクリックする時にクリック数を更新するスクリプト
 *======================================================================
 * @author Azet
 * @date 2016-03-10 
 */

require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/config.php");
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/common.php");
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/uranairanking.class.php");

// args
$date = $_GET['date'];
$star = $_GET['star'];
$rank = $_GET['rank'];

// args tests
if(!$date) {
	$date = date('Ymd');
}

$html = "";

$ranking = new UranaiRanking($date);
$logs = $ranking->getDetailsForStarRank($star, $rank);

if(count($logs)>1) {
	foreach($logs as $rank => $log) {
		$html .= "<h3>$rank</h3>\n";
		$html .= makeOneRank($log);
	}

} else {
	$html .= makeOneRank($logs[$rank]);
}


die($html);


// ======================================================================
function makeOneRank($line_) {
	$html = "<ul>\n";

	for($i=0; $i<count($line_); ++$i) {
		$line = $line_[$i];
		$html .= "<li><a href='{$line['site_url']}' target='_blank'>{$line['site']}</a></li>\n";
	}
	$html .= "</ul>\n";

	return $html;
}
