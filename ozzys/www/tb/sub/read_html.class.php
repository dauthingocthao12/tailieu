<?PHP
/*

	テンプレート読込・変換クラス

	Azet ookawara

*/

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

		if (!$this->file) { $html = "NO NET FILE_NAME"; return $html; }

		$path = $this->dir . $this->file;
		if (file_exists($path)) {
			$html = file_get_contents($path);

			preg_match_all("/<!--(\w+)-->/",$html,$KEY);
			if ($KEY[1]) {
				foreach ($KEY[1] AS $key => $val) {
					if ($val) {
						$this->KEYWORD[$val] = $val;
					}
				}
			}

			preg_match_all("/<!--\/(\w+)-->/",$html,$DEL_KEY);
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

		if ($this->KEYWORD) {
			foreach ($this->KEYWORD AS $val) {
				$parts = "";
				if (preg_match("/PHP_SELF/",$val)) {
					$parts = $_SERVER['PHP_SELF'];
				} elseif (!preg_match("/_/",$val)) {
					if ($this->rep_cmd[$val] != "") {
						$parts = $this->rep_cmd[$val];
					}
				}
				if ($val && $parts != "") {
					$html = mb_ereg_replace("<!--$val-->",$parts,$html);
				}
			}
		}

		if ($this->DEL_KEYWORD) {
			foreach ($this->DEL_KEYWORD AS $val) {
				if ($this->del_cmd[$val]) {
					$html = mb_ereg_replace("<!--$val-->(.+?)<!--\/$val-->","",$html);
				}
			}
		}

		return $html;
	}
}
?>