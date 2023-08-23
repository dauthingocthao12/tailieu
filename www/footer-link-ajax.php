<?php
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/config.php");
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/common.php");
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/uranairanking.class.php");

if($_GET['action'] == 'footer-link'){
	$today = date('Ymd');
	$html .= "<div class=\"modal fade\" id=\"sampleModal\" tabindex=\"-1\">";
	$html .= "<div class=\"modal-dialog\">";
	$html .= "<div class=\"modal-content\">";
	$html .= "<table class=\"footer-link \">";
	$i = 0;
	$day_link = "";
	if($_GET['data_type']){
		$day_link .= "/".$_GET['data_type'];
	}
	if($_GET['date'] != $today){
		$day_link .= "/".$_GET['date'];
	}
	foreach($en_star as $jp_name => $eng){
		$i = $i + 1;
		if($i%3 == 1 && $i != 1){
			if($i == 1){
				$html .= "<tr>";
			}else{
				$html .= "</tr><tr>";
			}
		}
		$html .= "<td class=\"".$eng."\"><a href=\"".$day_link."/".$eng."\">".$jp_name."</a></td>";
	}
	$html .= "</tr>";
	$html .= "<tr><td colspan=\"3\" class=\"rank-list-link\"><a href=\"".$day_link."/\">ランキング一覧へ</a></td></tr>";
	$html .= "<tr><td colspan=\"3\" data-dismiss=\"modal\" class=\"p-hand\">閉じる</td></tr>";
	$html .= "</table>";
	$html .= "</div></div></div>";
	echo $html;
}
