<?PHP
//	商品画像一覧表示プログラム

//	ookawara 2009/09/10

	//	サブルーチンフォルダー
	$SUB_DIR = "./sub";
	//	アフィリエイト関連フォルダー
	$AF_DIR = "./affiliate";

	include("../cone.inc");
	include("$SUB_DIR/setup.inc");
	include("$SUB_DIR/array.inc");
	include("$SUB_DIR/goods_disp_list.php");

	$PHP_SELF = $_SERVER['PHP_SELF'];

	$VALUE = array();
	$CHECK = array();
	$VC = array();

	if ($_GET['c']) {
		$VC = explode("<>",$_GET['c']);
	} else {
		exit;
	}

	$html = goods_disp_list($VALUE,$CHECK,$VC);

	echo("$html");

	if ($conn_id) { mysqli_close($conn_id); }

	exit;
?>
