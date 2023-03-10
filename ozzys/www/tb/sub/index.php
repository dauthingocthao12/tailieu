<?PHP
/*

	ozzys様
	PDA商品管理システム
		TOPページ表示

*/

function make_html() {

	$html = default_html();

	return $html;

}


//
function default_html() {

	$html = "";
	$INPUTS = array();


//	$INPUTS['CATEGORYTITLE'] = array(result=>'plane', 'value'=>$l1_name);
//	$INPUTS['CATEGORYLIST'] = array(result=>'plane', 'value'=>$category_list);

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(INCLUDE_DIR);
	$make_html->set_file(TEMP_INDEX);
	$make_html->set_rep_cmd($INPUTS);
	$html = $make_html->replace();

	return $html;
}
?>