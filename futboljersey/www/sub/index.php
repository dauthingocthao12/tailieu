<?php

function start() {

	$title = "サッカー用品 サッカースパイク サッカーユニフォームのサッカーショップネイバーズスポーツ -- NBS --";
	$html = default_html();

	return array($html,$title);
}

function default_html() {

	$file = "./sub/template/home.html";
	if (file_exists($file)) {
		$LIST = file($file);
		foreach ($LIST as $val) {
			$html .= $val;
		}
	}

	return $html;
}

?>
