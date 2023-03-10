<?PHP
//	エラーサブルーチン
function ERROR($ERROR) {

	$html = <<<WAKABA
<br>
<b><font color="#ff0000">エラー</font></b><br>

WAKABA;

	foreach ($ERROR AS $VAL) {
		$html .= "・$VAL<br>\n";
	}

	return $html;

}
?>
