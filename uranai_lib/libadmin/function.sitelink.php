<?php

/**
 * リンクを作成
 *
 * @author Azet
 * @param array $params
 *  パラメターの例
 *  [mode] => 'rank' など
 *  [d] => '20160116'
 *  [star] => '7' など (config.phpにある$en_num_star使用)
 * @return string
 */
function smarty_function_sitelink($params)
{
	global $en_num_star;

	$url = "";
	$param_list=array('mode', 'topic' , 'd', 'star' );

	if($params['mode'] == ""){ return "/"; } //モード無しはルートへ
	foreach($param_list as $k) {
		if (!array_key_exists($k, $params)) continue;

		// $kはパラメター名
		$v = $params[$k];
		
		// 一目のパラメターは、rankの時に使いません
		if($v=='rank') continue;

		// detailの場合は、同じ
		if($v=='detail') {
			continue;
		}

		if($k=='topic' && $v== "") {
			continue;
		}

		// date?
		if($k=='d' && $v==date('Ymd')) {
			continue;
		}

		if($k == "star") {
			$v = $en_num_star[$v];
		}
		$url .= "/$v";
	}
	// final / is important!
	return $url.'/';
}

?>
