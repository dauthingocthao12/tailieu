<?PHP
//	メニュー MENU
function menu($LOGDATA_DIR,$menu_file) {

	$m_file = "$LOGDATA_DIR/$menu_file";
	if (file_exists($m_file)) {
		$fp = fopen ($m_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$menu .= fgets($fp, 4096);
			}
		}
	}

	return $menu;

}
?>
