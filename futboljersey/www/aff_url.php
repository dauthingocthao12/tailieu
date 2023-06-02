<?PHP
//	アフィリエイトURL表示プログラム

	//	サブルーチンフォルダー
	$SUB_DIR = "./sub";
	//	アフィリエイト関連フォルダー
	$AF_DIR = "./affiliate";

	include("../cone.inc");
	include("$SUB_DIR/setup.inc");
	include("$SUB_DIR/array.inc");
	include("$AF_DIR/aff_url_.inc");

	session_cache_limiter('nocache');
	session_start();

	if ($_GET['page']) {
		$CHECK['page'] = $_GET['page'];
	}

	$html = aff_url($CHECK);

	echo($html);

	exit;

?>
