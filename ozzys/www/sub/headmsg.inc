<?PHP
//	ヘッドメッセージ HEADMSG
function headmsg($LOGDATA_DIR,$headmsg_file) {

	$h_file = "$LOGDATA_DIR/$headmsg_file";
	if (file_exists($h_file)) {
		$fp = fopen ($h_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$headmsg .= fgets($fp, 4096);
			}

		}
	}
	$headmsg = nl2br($headmsg);

	return $headmsg;

}
?>
