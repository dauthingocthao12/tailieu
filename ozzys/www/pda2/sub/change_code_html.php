<?PHP
/*

	ozzys様
	PDA商品管理システム

	表示変換設定


*/

function change_code_html($html,$strto) {

	//	共通置換
	if (defined("MODE")) { $mode = MODE; }
	if (defined("ACTION")) { $action = ACTION; }
	$DEFAULT_KEY = array(
					 "MODE" => $mode
					,"ACTION" => $action
					);
	foreach ($DEFAULT_KEY AS $key => $val) {
		$key = addslashes($key);
		$html = preg_replace("/<!--{$key}-->/",$val,$html);
	}

	//	未変換削除
	$html = mb_ereg_replace("<!--(.+?)-->","",$html);

	return $html;

}

//	出力
function output_html($html) {

	$charset = DISPLAY_SET_ENCODE;
//	header('Content-Type: text/html; charset:$charset');

	echo $html;
//	ob_start("mb_output_handler");

}
?>