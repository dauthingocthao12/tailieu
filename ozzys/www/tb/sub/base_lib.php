<?PHP
/*

	ozzys様
	PDA商品管理システム

	set_request()	文字コード変換

*/

//	文字コード変換
function set_request($strfrom) {

	if (isset($_REQUEST)) {
		mb_convert_variables(mb_internal_encoding(), $strfrom, $_REQUEST);
	}

	//	モード・アクションセット
	if ($_POST['mode']) {
		$mode = $_POST['mode'];
	} elseif ($_GET['mode']) {
		$mode = $_GET['mode'];
	} elseif ($_SESSION['SET']['mode']) {
		$mode = $_SESSION['SET']['mode'];
	}
	if ($_POST['action']) {
		$action = $_POST['action'];
	} elseif ($_GET['action']) {
		$action = $_GET['action'];
	} elseif ($_SESSION['SET']['action']) {
		$action = $_SESSION['SET']['action'];
	}
	unset($_SESSION['SET']);
	if ($mode) {
		$_SESSION['SET']['mode'] = $mode;
		define("MODE",$mode);
	}
	if ($action) {
		$_SESSION['SET']['action'] = $action;
		define("ACTION",$action);
	}

}

//	ページ処理
function page_view($count) {
	$html = "";
	$INPUTS = array();
	$page_max_len = 6;

	$page_html = "";
	if ($count>100) {
		$page_html .= "該当数：".number_format($count)."件 <br>";
	}elseif ($count) {
		$page_html .= "該当数：".number_format($count)."件 <br>";
		if (!$page_html) { return $html; }
	} else {
		$page_html .= "該当する商品はございません。<br />\n";
	}

	$INPUTS['SETPAGE'] = $page_html;

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(INCLUDE_DIR);
	$make_html->set_file(TEMP_PAGE);
	$make_html->set_rep_cmd($INPUTS);
	$html = $make_html->replace();

	return $html;
}

function pre($val) {
	echo "<pre>";
	print_r($val);
	echo "</pre>";
	return;
}
?>