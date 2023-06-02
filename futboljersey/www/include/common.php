<?PHP
/*

	ネイバーズスポーツ　システム共通ルーチン

*/

//	テンプレート読込・変換クラス
class read_html {

	var $dir = "./";
	var $file = "";
	var $rep_cmd = array();
	var $del_cmd = array();

	function set_dir($dir) {
		$this->dir = $dir;
	}
	function set_file($file) {
		$this->file = $file;
	}
	function set_rep_cmd($rep_cmd) {
		$this->rep_cmd = $rep_cmd;
	}
	function set_del_cmd($del_cmd) {
		$this->del_cmd = $del_cmd;
	}

	function read() {
		$html = "";
		$KEY = array();
		$KEYWORD = array();
		$DEL_KEY = array();
		$DEL_KEYWORD = array();

		if (!$this->file) {
			$html = "NO NET FILE_NAME";
			return $html;
		}

		$path = $this->dir . $this->file;
		if (file_exists($path)) {
			$html = file_get_contents($path);

			preg_match_all("/<!--(\w+)-->/", $html, $KEY);
			if ($KEY[1]) {
				foreach ($KEY[1] AS $key => $val) {
					if ($val) {
						$this->KEYWORD[$val] = $val;
					}
				}
			}

			preg_match_all("/<!--\/(\w+)-->/", $html, $DEL_KEY);
			if ($DEL_KEY[1]) {
				foreach ($DEL_KEY[1] AS $key => $val) {
					if ($val) {
						$this->DEL_KEYWORD[$val] = $val;
					}
				}
			}
		} else {
			$html = "NO TEMPLATE : $path";
		}
		return $html;
	}


	function replace(){
		$html = read_html::read();
		if (!$this->KEYWORD && !$this->DEL_KEYWORD) { return $html; }

		if ($this->DEL_KEYWORD) {
			foreach ($this->DEL_KEYWORD AS $val) {
				if ($this->del_cmd[$val] > 0) {
					$html = mb_ereg_replace("<!--$val-->(.+?)<!--\/$val-->", "", $html);
				}
			}
		}

		if ($this->KEYWORD) {
			foreach ($this->KEYWORD AS $val) {
				$parts = "";
				if (preg_match("/PHP_SELF/", $val)) {
					$parts = $_SERVER['PHP_SELF'];
				} elseif (!preg_match("/_/", $val)) {
					//if ($this->rep_cmd[$val] != "") {
						$parts = $this->rep_cmd[$val];
					//}
				}
				//if ($val && $parts != "") {
					$html = preg_replace("/<!--$val-->/", $parts, $html);
				//}
			}
		}

		return $html;
	}
}


//	ファイル読込
function read_file($file_name) {

	$html = "";
	if (file_exists($file_name)) {
		$html = file_get_contents($file_name);
	}

	return $html;
}


//	値表示
function pre($VALUE) {

	echo "<pre>\n";
	print_r($VALUE);
	echo "\n";
	echo "</pre>\n";

}



//	ブランド名抽出
function get_brand_name($b_file) {

	$B_LINE = array();
	if (file_exists($b_file)) {
		$B_LIST = file($b_file);
		foreach ($B_LIST AS $val) {
			list($b_num_,$b_name_,$del_) = explode("<>",$val);
			$B_LINE[$b_num_] = $b_name_;
		}
	}

	return $B_LINE;
}



//	エラー
function error_html($ERROR) {

	$html = "";
	$error_msg = "";
	foreach ($ERROR AS $val) {
		$val = trim($val);
		if (!$val) { continue; }
		$error_msg .= "・".$val."<br />\n";
	}

	if ($error_msg) {

		$html .= "<section class=\"error-alert\">\n";
		$html .= "  <span class=\"error-message\">\n";
		$html .= 		$error_msg."\n";
		$html .= "  </span>\n";
		$html .= "</section>\n";

	}

	return $html;
}
?>
