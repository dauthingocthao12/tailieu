<?PHP
//	ヘッダ
	//	サブルーチンフォルダー
	$SUB_DIR = "./sub";

	include ("../cone.inc");
	include ("$SUB_DIR/menu.inc");
	include("$SUB_DIR/setup.inc");
	include("$SUB_DIR/array.inc");
	session_cache_limiter('nocache');
	session_start();
	$_SESSION["idpass"];
	$_SESSION["customer"];
	$_SESSION["opt"];
	$_SESSION["blurl"];

	$file = "./sub/template/head_menu.html";
	if (file_exists($file)) {
		$LIST = file($file);
		foreach ($LIST as $val) {
			$html .= $val;
		}
	}
##	$html = eregi_replace("<!--TITLE-->",$title,$html);
	$html = preg_replace("/<!--TITLE-->/",$title,$html);

	echo($html);

	exit;

?>
