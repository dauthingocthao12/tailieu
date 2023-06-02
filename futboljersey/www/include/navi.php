<?PHP
/*

	ネイバーズスポーツ　ナビルーチン

*/

function read_navi() {

	$INPUTS = array();
	$DEL_INPUTS = array();

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("/navi.htm");
	//$make_html->set_rep_cmd($INPUTS);
	//$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return $html;
}



//	header 404 
function header404() {

	header("HTTP/1.0 404 Not Found");
	exit;

}
?>
