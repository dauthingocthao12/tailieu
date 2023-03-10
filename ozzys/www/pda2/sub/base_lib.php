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
function page_view($count,$page,$page_all) {

	$html = "";
	$INPUTS = array();
	$page_max_len = 6;

	$page_html = "";
	if ($count) {
		$page_html .= "該当数：".number_format($count)."件 ".number_format($page)."/".number_format($page_all)."ページ<br />\n";
		if ($page > 1) {
			$back_page = $page - 1;
			if ($page != 2) { $urls .= "/index".$back_page.".html"; }
			$page_html .= "<input type=\"submit\" name=\"back_page\" value=\"&lt;\" />\n";
		}

		$check_s_page = $page - $page_max_len / 2;
		if ($check_s_page > 0) {
			$s = $check_s_page;
		} else {
			$s = 1;
		}
		$e = $s + $page_max_len;
		if ($e > $page_all) { $e = $page_all; }
		$s = $e - $page_max_len;
		if ($s < 1) { $s = 1; }
		for ($i=$s; $i<=$e; $i++) {
			if ($page == $i) {
				$page_html .= "<b>".$i."</b>\n";
			} else {
				$page_html .= "<input type=\"submit\" name=\"s_page\" value=\"".$i."\" />\n";
			}
		}

		if ($page < $page_all) {
			$page_html .= "<input type=\"submit\" name=\"next_page\" value=\"&gt;\" />\n";
		}
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